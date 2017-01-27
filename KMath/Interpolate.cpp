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

#include "KMath.h"

namespace KayLib
{

    /**
     * Perform a cosine interpolation between the points.
     * @param y1 The first control point.
     * @param y2 The second control point.
     * @param x The point to interpolate.
     * @return The result of the interpolation.
     */
    double cosineInterpolate(double y1, double y2, double x)
    {
        double ft = x * 3.1415927f;
        double f = (1.0 - cos(ft)) * 0.5;
        return y1 * (1 - f) + y2 * f;
    }

    /**
     * Perform a cubic interpolation.
     * @param y1 The first control point.
     * @param y2 The second control point.
     * @param y3 The third control point.
     * @param y4 The fourth control point.
     * @param x The point to interpolate.
     * @return The result of the interpolation.
     */
    double cubicInterpolate(double y1, double y2, double y3, double y4, double x)
    {
        double x2 = x * x;
        double a1 = y4 - y3 - y1 + y2;
        double a2 = y1 - y2 - a1;
        double a3 = y3 - y1;
        double a4 = y2;

        return (a1 * x * x2 + a2 * x2 + a3 * x + a4);
    }

    /**
     * Perform a hermite interpolation
     * @param y1 The first control point.
     * @param y2 The second control point.
     * @param y3 The third control point.
     * @param y4 The fourth control point.
     * @param x The point to interpolate.
     * @param tension The tension applied to the interpolation (0 is normal)
     * @param bias The bias (twist) applied to the interpolation (positive is toward the first segment, negative away)
     * @return The result of the interpolation.
     */
    double hermiteInterpolate(double y1, double y2, double y3, double y4, double x, double tension, double bias)
    {
        double x2 = x * x;
        double x3 = x2 * x;
        double m1 = (y2 - y1) * (1 + bias) * (1 - tension) / 2;
        m1 += (y3 - y2) * (1 - bias) * (1 - tension) / 2;
        double m2 = (y3 - y2) * (1 + bias) * (1 - tension) / 2;
        m2 += (y4 - y3) * (1 - bias) * (1 - tension) / 2;
        double a1 = 2 * x3 - 3 * x2 + 1;
        double a2 = x3 - 2 * x2 + x;
        double a3 = x3 - x2;
        double a4 = -2 * x3 + 3 * x2;

        return (a1 * y2 + a2 * m1 + a3 * m2 + a4 * y3);
    }

}

