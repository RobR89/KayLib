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

#ifndef KEVENTRATE_H
#define KEVENTRATE_H

#include <chrono>

namespace KayLib
{

    class KEventRate
    {
    public:

        KEventRate()
        {
            reset();
        }

        /**
         * Reset the counter.
         */
        void reset()
        {
            startTime = std::chrono::high_resolution_clock::now();
            lastTime = startTime;
            totalTicks = 0;
            nextRecord = 0;
            for(int i = 0; i < maxRecordLength; i++)
            {
                tickRecord[i] = startTime;
            }
        }

        /**
         * Count a event.
         * @returns The instantaneous event rate
         */
        double tick()
        {
            auto currentTime = std::chrono::high_resolution_clock::now();
            // Get the time since last update.
            std::chrono::duration<double> interval = (currentTime - lastTime);
            lastTime = currentTime;
            // Increment the total number of ticks counted.
            totalTicks++;
            // Save time in current record.
            tickRecord[nextRecord] = lastTime;
            // Increment to next record.
            nextRecord++;
            // Loop counter if past of record space.
            if(nextRecord >= maxRecordLength)
            {
                nextRecord = 0;
            }
            double li = interval.count();
            if(li > 0)
            {
                // Calculate and return the event rate of the current event tick.
                return 1.0 / li;
            }
            // Since less than one nanosecond has passed were getting at least 1,000,000,000 FPS.
            // return -1 as a metaphor for infinity.
            return -1;
        }

        /**
         * Returns the average rate for the last 'seconds'
         * This returns a more stable number than the instantaneous event rate
         * @param seconds The number of seconds in the past to search.
         * @return The event rate.
         */
        double getRateHistory(int seconds)
        {
            if(seconds <= 0 || totalTicks == 0)
            {
                // No ticks have been rendered.
                return 0;
            }
            // Time of the interval start
            auto currentTime = std::chrono::high_resolution_clock::now();
            auto scanTime = (currentTime - std::chrono::seconds(seconds));
            // Set to the start time if interval is longer than the counter has been running.
            if(scanTime < startTime)
            {
                scanTime = startTime;
            }
            // Search the maximum number of records.
            int maxRecords = maxRecordLength;
            // Reduce the max records if they all have not been used.
            if(maxRecords > totalTicks)
            {
                maxRecords = totalTicks;
            }
            // Start scanning at current record.
            int scanIndex = nextRecord;
            int records = 0;
            // Look for oldest tick that is at least 'seconds' old.
            for(int i = 0; i < maxRecords; i++)
            {
                scanIndex--;
                if(scanIndex < 0)
                {
                    scanIndex += maxRecordLength;
                }
                // If event record is before search time calculate FPS
                if(tickRecord[scanIndex] <= scanTime)
                {
                    break;
                }
                else
                {
                    records++;
                }
            }
            // Calculate event rate.
            return (double) records / std::chrono::duration<double>(currentTime - tickRecord[scanIndex]).count();
        }

        /**
         * Get the total average event rate.
         * @return The average event rate since the last counter reset started
         */
        double getTotalRate()
        {
            auto currentTime = std::chrono::high_resolution_clock::now();
            return (double) totalTicks / std::chrono::duration<double>(currentTime - startTime).count();
        }

    protected:
        static const int maxRecordLength = 1024;
        std::chrono::high_resolution_clock::time_point startTime;
        std::chrono::high_resolution_clock::time_point lastTime;
        std::chrono::high_resolution_clock::time_point tickRecord[maxRecordLength];
        // Total ticks counted.
        int totalTicks;
        // Current record index.
        int nextRecord;

    };

}

#endif