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

#ifndef KMATH_H
#define KMATH_H

#include <math.h>

namespace KayLib
{

    const double pi = M_PI;
    const double gravity_ft = -32.174;
    const double tol = 0.0000000001;
    const double piover180 = (pi / 180.0);
    const double one80overpi = (180.0 / pi);

    inline double sqr(double val)
    {
        return val * val;
    }

    /**
     * Convert the degrees to radians
     * @param deg The value in degrees to convert.
     * @return The value converted to radians.
     */
    inline double degreesToRadians(double deg)
    {
        return deg * piover180;
    }

    /**
     * Convert the radians to degrees.
     * @param rad The value in radians to convert.
     * @return The value converted to degrees.
     */
    inline double radiansToDegrees(double rad)
    {
        return rad * one80overpi;
    }

    // pre-define math classes.
    class Vector3D;
    class Vector4D;
    class Matrix;
    class Quaternion;

    class LatLon
    {
    public:
        double Latitude;
        double Longatude;

    };

    class LatLonHead
    {
    public:
        double Latitude;
        double Longatude;
        double Heading;

    };

}

/**
 * Include ALL KayLib Math library classes.
 */

#include "Interpolate.h"
#include "Vector3D.h"
#include "Vector4D.h"
#include "Matrix.h"
#include "Quaternion.h"
#include "Spherical.h"
#include "Spline.h"
#include "PerlinNoise.h"

#endif /* KMATH_H */
