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

#ifndef KGRAPHICS_H
#define KGRAPHICS_H

namespace KayLib
{

    struct KPoint
    {

        KPoint()
        {
            x = y = 0;
        };

        KPoint(int nx, int ny)
        {
            x = nx;
            y = ny;
        };
        int x;
        int y;
    };

    struct KSize
    {

        KSize()
        {
            w = h = 0;
        }

        KSize(int nw, int nh)
        {
            w = nw;
            h = nh;
        }
        int w;
        int h;
    };

    struct KRect
    {

        KRect()
        {
            x = y = w = h = 0;
        }

        KRect(int nx, int ny, int nw, int nh)
        {
            x = nx;
            y = ny;
            w = nw;
            h = nh;
        }
        int x, y;
        int w, h;
    };

}

#endif /* KGRAPHICS_H */

