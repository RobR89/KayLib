#include "KEventRate.h"

namespace KayLib
{

    KEventRate::KEventRate()
    {
        reset();
    }

    void KEventRate::reset()
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

    double KEventRate::tick()
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

    double KEventRate::getRateHistory(int seconds)
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

    double KEventRate::getTotalRate()
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        return (double) totalTicks / std::chrono::duration<double>(currentTime - startTime).count();
    }

}
