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

/**
 A collection of interpolation functions.
 */

#ifndef INTERPOLATE_H
#define INTERPOLATE_H

#include <cmath>

namespace KayLib
{

    /**
     * Perform a linear interpolation between the points.
     * @param y1 The first control point.
     * @param y2 The second control point.
     * @param x The point to interpolate.
     * @return The result of the interpolation.
     */
    inline double linearInterpolate(double y1, double y2, double x)
    {
        return y1 * (1 - x) + y2 * x;
    }

    /**
     * Perform a cosine interpolation between the points.
     * @param y1 The first control point.
     * @param y2 The second control point.
     * @param x The point to interpolate.
     * @return The result of the interpolation.
     */
    double cosineInterpolate(double y1, double y2, double x);

    /**
     * Perform a cubic interpolation.
     * @param y1 The first control point.
     * @param y2 The second control point.
     * @param y3 The third control point.
     * @param y4 The fourth control point.
     * @param x The point to interpolate.
     * @return The result of the interpolation.
     */
    double cubicInterpolate(double y1, double y2, double y3, double y4, double x);

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
    double hermiteInterpolate(double y1, double y2, double y3, double y4, double x, double tension, double bias);

}

#endif /* INTERPOLATE_H */

