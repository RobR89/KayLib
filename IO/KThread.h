/*
 * Copyright 2017 Robert Reinhart.
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

#ifndef KTHREAD_H
#define KTHREAD_H

#include <thread>
#include <signal.h>

namespace KayLib
{
    // @todo replace pthread calls with something platform independent.

    class KThread
    {
    private:

        KThread() { }

    public:

        /**
         * Creates a KThread object that represents the currently running thread.
         */
        static KThread *self()
        {
            KThread *self = new KThread();
#ifdef _WIN32
            self->thread = GetCurrentThread();
#else
            self->thread = pthread_self();
#endif
            return self;
        }

        /**
         * Is the thread alive?
         * @return True if the thread is still alive.
         */
        bool isAlive()
        {
#ifdef _WIN32
            if(thread == NULL)
            {
                return false;
            }
            if(WaitForSingleObject(thread, 0) == WAIT_TIMEOUT)
            {
                // thread is unsignaled so it's still running.
                return true;
            }
            DWORD exitCode;
            GetExitCodeThread(thread, &exitCode);
            return exitCode == STILL_ACTIVE;
#else
            if(thread == 0)
            {
                return false;
            }
            return pthread_kill(thread, 0) != ESRCH;
#endif
        }

        /**
         * Does this KThread object represent the calling thread?
         * @return True if yes.
         */
        bool isSelf()
        {
#ifdef _WIN32
            return thread == GetCurrentThread();
#else
            return thread == pthread_self();
#endif
        }

    private:
#ifdef _WIN32
        HANDLE thread = NULL;
#else
        pthread_t thread = 0;
#endif
    };

}

#endif /* KTHREAD_H */

