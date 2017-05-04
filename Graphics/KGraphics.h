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

        KPoint(const int nx, const int ny)
        {
            x = nx;
            y = ny;
        };
        int x;
        int y;

        bool operator==(const KPoint &p) const
        {
            return (x == p.x) && (y == p.y);
        }

        bool operator!=(const KPoint &p) const
        {
            return !(*this == p);
        }
    };

    struct KSize
    {

        KSize()
        {
            w = h = 0;
        }

        KSize(const int nw, const int nh)
        {
            w = nw;
            h = nh;
        }
        int w;
        int h;

        bool operator==(const KSize &s) const
        {
            return (w == s.w) && (h == s.h);
        }

        bool operator!=(const KSize &s) const
        {
            return !(*this == s);
        }
    };

    struct KRect
    {

        KRect()
        {
            x = y = w = h = 0;
        }

        KRect(const int nx, const int ny, const int nw, const int nh)
        {
            x = nx;
            y = ny;
            w = nw;
            h = nh;
        }
        int x, y;
        int w, h;

        bool operator==(const KRect &r) const
        {
            return (x == r.x) && (y == r.y) && (w == r.w) && (h == r.h);
        }

        bool operator!=(const KRect &r) const
        {
            return !(*this == r);
        }
    };

}

#endif /* KGRAPHICS_H */

