/*
 * Copyright 2016 Robert Reinhart.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef EVENT_H
#define	EVENT_H

#include <vector>
#include <mutex>
#include <algorithm>
#include <condition_variable>

#include "KThread.h"

namespace KayLib {

template <typename T>
class EventDispatcher;

template <typename T>
class EventListener {
  friend EventDispatcher<T>;
public:

  EventListener() {
    std::unique_lock<std::mutex> uLock = getLock();
    if (allListeners == NULL) {
      allListeners.reset(new ListenerList());
    }
    allListeners->push_back(this);
  }

  virtual ~EventListener() {
    std::unique_lock<std::mutex> uLock = getLock();
    // Stop more dispatchers from connecting.
    if (allListeners != NULL) {
      typename ListenerList::iterator found = std::find(allListeners->begin(), allListeners->end(), this);
      if (found != allListeners->end()) {
        allListeners->erase(found);
      }
      // At this point this object will report not alive and should never allow actions.
      if (allListeners->empty()) {
        // Delete when empty as we have no other way to free the list and prevent memory leaks.
        allListeners.reset();
      }
    }
    // Get a copy of the dispatchers list as it will become invalid.
    typename EventDispatcher<T>::DispatcherList disp(dispatchers);
    dispatchers.clear();
    // Notify all waiting threads so they can exit!
    threadWait.notify_all();
    if (activeThread.get() != NULL && activeThread->isSelf()) {
      // Remove this thread from active threads.
      activeThread.reset();
    }
    // Release the lock so we don't cause unnecessary delays in other threads.
    uLock.unlock();
    // Remove self from all dispatchers.
    for (EventDispatcher<T> *dispatcher : disp) {
      dispatcher->removeListener(this);
    }
    uLock.lock();
    // We are not the active thread, wait for it to end.
    int triesLeft = 1000;
    while (activeThread.get() != NULL && triesLeft > 0) {
      // Check if our thread was completed.
      auto itr = std::find(deadCompletedThreads.begin(), deadCompletedThreads.end(), activeThread.get());
      if (itr != deadCompletedThreads.end()) {
        // It completed, finish up.
        deadCompletedThreads.erase(itr);
        activeThread.reset();
        continue;
      }
      // Is the active thread still alive?
      if (activeThread.get() != NULL && !activeThread->isAlive()) {
        // The thread died at some point, but not cleanly.
        activeThread.reset();
        continue;
      }
      // The thread is still alive give it time to finish.
      if (completeWait.wait_for(uLock, std::chrono::seconds(60)) == std::cv_status::timeout) {
        // This was a timeout.
        triesLeft--;
      }
    }
  }

protected:
  /**
   * Respond to an event.
   * @param sender The dispatcher that sent the event.
   * @param event The event that occurred.
   * @note If the dispatcher is instructed to do so this is called from a unique thread.
   * @note The client is responsible for insuring that the listener remains alive during onEvent.
   */
  virtual void onEvent(EventDispatcher<T> *sender, std::shared_ptr<T> event) = 0;

private:
  typedef std::vector<EventListener<T> *> ListenerList;
  typename EventDispatcher<T>::DispatcherList dispatchers;
  static std::shared_ptr<ListenerList> allListeners;
  static std::vector<KThread *> deadCompletedThreads;
  static std::condition_variable completeWait;
  std::condition_variable threadWait;
  std::auto_ptr<KThread> activeThread;

  bool isAlive() {
    if (allListeners != NULL) {
      typename ListenerList::iterator found = std::find(allListeners->begin(), allListeners->end(), this);
      return found != allListeners->end();
    }
    return false;
  }

  std::unique_lock<std::mutex> getLock() {
    static std::mutex lockPtr;
    return std::unique_lock<std::mutex>(lockPtr);
  }

  /**
   * The event thread was called.  Send the information to the listener.
   * @param arg The packaged information.
   */
  void startEvent(EventDispatcher<T> *dispatcher, std::shared_ptr<T> event) {
    std::unique_lock<std::mutex> uLock = getLock();
    if (isAlive()) {
      while (activeThread.get() != NULL) {
        if (threadWait.wait_for(uLock, std::chrono::seconds(30)) == std::cv_status::timeout) {
          if (!isAlive()) {
            // The object is dead we can do nothing more here.
            return;
          }
          // Is the active thread still alive?
          if (activeThread.get() != NULL && !activeThread->isAlive()) {
            // The thread died at some point, but not cleanly.
            activeThread.reset();
          }
        }
        if (!isAlive()) {
          // Make sure the object was not deleted while we waited.
          return;
        }
      }
      // We are clear to run.
      activeThread.reset(KThread::self());
      // Release the global lock so other threads can run in case the listener takes a long time.
      // And to prevent deadlocks.... Hopefully!  As we do not know what onEvent might do.
      uLock.unlock();
      try {
        // The client is responsible for insuring that the listener remains alive during onEvent.
        onEvent(dispatcher, event);
      } catch (const std::exception&) {
      }
      // Re-aquire the lock to remove the active thread.
      uLock.lock();
      if (isAlive()) {
        activeThread.reset();
        // Let the next thread dispatch it's event.
        threadWait.notify_one();
      } else {
        // Object was/is being deleted let the destructor know were done.
        deadCompletedThreads.push_back(activeThread.get());
        completeWait.notify_all();
      }
    }
  }

  /**
   * Add a reference to the dispatchers this listener is attached to.
   * @param dispatcher The dispatcher that attached this listener.
   * @return true if the listener is still alive.
   */
  bool addDispatcher(EventDispatcher<T> *dispatcher) {
    if (dispatcher == NULL) {
      return false;
    }
    std::unique_lock<std::mutex> uLock = getLock();
    if (isAlive()) {
      if (std::find(dispatchers.begin(), dispatchers.end(), dispatcher) == dispatchers.end()) {
        // This should only ever be called by the dispatcher so no need to call addListener.

        dispatchers.push_back(dispatcher);
      }
      return true;
    }
    return false;
  }

  /**
   * Remove a reference to the dispatchers this listener is attached to.
   * @param dispatcher The dispatcher that removed this listener.
   */
  void removeDispatcher(EventDispatcher<T> *dispatcher) {
    if (dispatcher == NULL) {
      return;
    }
    std::unique_lock<std::mutex> uLock = getLock();
    if (isAlive()) {
      typename EventDispatcher<T>::DispatcherList::iterator itr = std::find(dispatchers.begin(), dispatchers.end(), dispatcher);
      if (itr != dispatchers.end()) {
        // This should only ever be called by the dispatcher so no need to call removeListener.

        dispatchers.erase(itr);
      }
    }
  }

};

template <typename T>
class EventDispatcher {
  friend EventListener<T>;
public:

  EventDispatcher() {
    std::unique_lock<std::mutex> uLock = getLock();
    if (allDispatchers == NULL) {

      allDispatchers.reset(new DispatcherList());
    }
    allDispatchers->push_back(this);
  }

  virtual ~EventDispatcher() {
    std::unique_lock<std::mutex> uLock = getLock();
    // Prevent more listeners from being added.
    if (allDispatchers != NULL) {
      typename DispatcherList::iterator found = std::find(allDispatchers->begin(), allDispatchers->end(), this);
      if (found != allDispatchers->end()) {
        allDispatchers->erase(found);
      }
      if (allDispatchers->empty()) {
        // Delete when empty as we have no other way to free the list and prevent memory leaks.
        allDispatchers.reset();
      }
    }
    // Get a copy of the listeners list as it will become invalid.
    typename EventListener<T>::ListenerList listen(listeners);
    listeners.clear();
    // Release the lock so we don't cause unnecessary delays in other threads.
    uLock.unlock();
    // Remove all listeners.
    for (EventListener<T> *listener : listen) {
      // Call remove listener so that the listener will be notified of the change.

      removeListener(listener);
    }
  }

  /**
   * Set weather the dispatcher should make new threads for each listener.
   * @param make True if new threads should be make.
   * @note Defaults to false.
   */
  void setMakeEventThread(bool make) {
    makeThread = make;
  }

public:

  /**
   * Add a listener to the dispatcher.
   * @param listener The listener to add.
   */
  void addListener(EventListener<T> *listener) {
    if (listener == NULL) {
      return;
    }
    std::unique_lock<std::mutex> uLock = getLock();
    if (isAlive()) {
      if (std::find(listeners.begin(), listeners.end(), listener) == listeners.end()) {
        // Listener not in list, add it.
        if (listener->addDispatcher(this)) {
          // The listener was alive so register it.

          listeners.push_back(listener);
        }
      }
    }
  }

  /**
   * Remove a listener from the dispatcher.
   * @param listener The listener to remove.
   */
  void removeListener(EventListener<T> *listener) {
    if (listener == NULL) {
      return;
    }
    std::unique_lock<std::mutex> uLock = getLock();
    typename EventListener<T>::ListenerList::iterator itr = std::find(listeners.begin(), listeners.end(), listener);
    if (itr != listeners.end()) {

      listeners.erase(itr);
    }
    uLock.unlock();
    listener->removeDispatcher(this);
  }

protected:

  /**
   * Dispatch an event to all listeners.
   * @param event The event to dispatch.
   * @note If the event is modifiable then not all listeners will necessarily get the exact same message.
   * @note The dispatcher will take ownership of the event, it will be deleted before the function returns.
   */
  void dispatch(T *event) {

    std::shared_ptr<T> evt(event);
    dispatch(evt);
  }

  /**
   * Dispatch an event to all listeners.
   * @param event The event to dispatch.
   * @note If the event is modifiable then not all listeners will necessarily get the exact same message.
   * @note The shared pointer keeps ownership of the event.
   */
  void dispatch(std::shared_ptr<T> event) {
    std::unique_lock<std::mutex> uLock = getLock();
    if (isAlive()) {
      typename EventListener<T>::ListenerList listen(listeners);
      uLock.unlock();
      for (EventListener<T> *listener : listen) {
        if (makeThread) {
          // Create a new thread.
          std::thread thread(&EventListener<T>::startEvent, listener, this, event);
          // Detach the thread so it does not terminate when it goes out of scope.
          thread.detach();
        } else {
          try {
            listener->startEvent(this, event);
          } catch (const std::exception &) {
          }
        }
      }
    }
  }

private:

  typedef std::vector<EventDispatcher<T> *> DispatcherList;
  typename EventListener<T>::ListenerList listeners;
  static std::shared_ptr<DispatcherList> allDispatchers;
  bool makeThread = false;

  bool isAlive() {
    if (allDispatchers != NULL) {
      typename DispatcherList::iterator found = std::find(allDispatchers->begin(), allDispatchers->end(), this);

      return found != allDispatchers->end();
    }
    return false;
  }

  std::unique_lock<std::mutex> getLock() {
    static std::mutex lockPtr;
    return std::unique_lock<std::mutex>(lockPtr);
  }

};

template<typename T>
std::shared_ptr<typename EventDispatcher<T>::DispatcherList> EventDispatcher<T>::allDispatchers;
template<typename T>
std::shared_ptr<typename EventListener<T>::ListenerList> EventListener<T>::allListeners;
template<typename T>
std::vector<KThread *> EventListener<T>::deadCompletedThreads;
template<typename T>
std::condition_variable EventListener<T>::completeWait;

}

#endif	/* EVENT_H */
