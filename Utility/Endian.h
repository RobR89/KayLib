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

#ifndef ENDIAN_H
#define ENDIAN_H

namespace KayLib
{

    /**
     * Utility to swap endianness.
     * T is the value type.
     * s is the number of bytes to swap.
     */
    template<typename T>
    union EndianSwap
    {
        T value;
        unsigned char bVal[sizeof (T)];

        /**
         * Swap the byte order.
         */
        void swap()
        {
            // Get the size;
            int s = sizeof (T);
            // Get end index.
            int e = s - 1;
            // Get the half size.
            int hs = s / 2;
            // Do the swap.
            for(int i = 0; i < hs; i++)
            {
                unsigned char tmp = bVal[i];
                bVal[i] = bVal[e - i];
                bVal[e - i] = tmp;
            }
        }
    };

}
#endif /* ENDIAN_H */

