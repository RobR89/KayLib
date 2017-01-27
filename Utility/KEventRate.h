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
        KEventRate();

        /**
         * Reset the counter.
         */
        void reset();
        /**
         * Count a event.
         * @returns The instantaneous event rate
         */
        double tick();
        /**
         * Returns the average rate for the last 'seconds'
         * This returns a more stable number than the instantaneous event rate
         * @param seconds The number of seconds in the past to search.
         * @return The event rate.
         */
        double getRateHistory(int seconds);
        /**
         * Get the total average event rate.
         * @return The average event rate since the last counter reset started
         */
        double getTotalRate();

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