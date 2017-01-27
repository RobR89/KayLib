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

#ifndef SPHERICAL_H
#define SPHERICAL_H

namespace KayLib
{

    class Spherical
    {
    protected:
        double x, y, z, w;

    public:

        Spherical()
        {
            w = x = y = z = 0;
        }

        /**
         * Get the latitude, longitude, and heading of the current state.
         * @return A structure containing the Latitude, longitude, and heading in degrees.
         */
        LatLonHead get() const;

        /**
         * Get the latitude and longitude of the current state.
         * @return A structure containing the Latitude and longitude in degrees.
         */
        LatLon getLocation() const;

        /**
         * Get the heading of the current state.
         * @return The heading in degrees.
         */
        double getHeading() const;

        /**
         * Get the latitude, longitude, and heading of the current state.
         * @return A structure containing the Latitude, longitude, and heading in radians.
         */
        LatLonHead get_rad() const;

        /**
         * Get the latitude and longitude of the current state.
         * @return A structure containing the Latitude and longitude in radians.
         */
        LatLon getLocation_rad() const;

        /**
         * Get the heading of the current state.
         * @return The heading in radians.
         */
        double getHeading_rad() const;

        /**
         * Set the latitude, longitude, and heading of the state.
         * @param lat The latitude in degrees.
         * @param lon The longitude in degrees.
         * @param head The heading in degrees.
         */
        void set(double lat, double lon, double head);

        /**
         * Set the latitude, longitude, and heading of the state.
         * @param lat The latitude in radians.
         * @param lon The longitude in radians.
         * @param head The heading in radians.
         */
        void set_rad(double lat, double lon, double head);

        /**
         * Set the location of the state.
         * @param lat The latitude in degrees.
         * @param lon The longitude in degrees.
         */
        void setLocation(double lat, double lon);

        /**
         * Set the location of the state.
         * @param lat The latitude in radians.
         * @param lon The longitude in radians.
         */
        void setLocation_rad(double lat, double lon);

        /**
         * Set the heading of the state.
         * @param head The heading in degrees.
         */
        void setHeading(double head);

        /**
         * Set the heading of the state.
         * @param head The heading in radians.
         */
        void setHeading_rad(double head);

        /**
         * Get the range to a target in degrees of rotation.
         * @param Target The target to range.
         * @return The Range in degrees of rotation around the globe.
         */
        double getRange(const Spherical &target) const;

        /**
         * Get the range to a target in radians of rotation.
         * @param Target The target to range.
         * @return The Range in radians of rotation around the globe.
         */
        double getRange_rad(const Spherical &target) const;

        /**
         * Get the bearing to the target.
         * @param Target The target to get the bearing to.
         * @return The bearing in degrees. (positive is right/clockwise, negative is left/counterclockwise)
         */
        double getBearing(const Spherical &target) const;

        /**
         * Get the bearing to the target.
         * @param Target The target to get the bearing to.
         * @return The bearing in radians. (positive is right/clockwise, negative is left/counterclockwise)
         */
        double getBearing_rad(const Spherical &target) const;

        /**
         * Move a specified distance around the globe in degrees.
         * @param arc The distance in degrees.
         */
        void move(double arc);

        /**
         * Move a specified distance around the globe in radians.
         * @param arc The distance in radians.
         */
        void move_rad(double arc);

        /**
         * Turn a number of degrees to the right (relative to the direction of travel)
         * @param turn Degrees to turn.
         */
        void turn(double turn);

        /**
         * Turn a number of degrees to the right (relative to the direction of travel)
         * @param turn Degrees to turn.
         */
        void turn_rad(double turn);

    };

    class SphericalReference
    {

        /**
         * Create a spherical reference object for a sphere with radius 1.
         */
        SphericalReference()
        {
            setZeroElevationRadius(1);
        }

        /**
         * Create a spherical reference object with the specified diameter.
         * @param radius The radius of the sphere.
         * @note The unit size must match in all calls. For example, a sphere
         * with a radius of 1000 miles must then use miles as the unit in all calls to move.
         * That unit size will then be used for all return values.
         */
        SphericalReference(double radius)
        {
            setZeroElevationRadius(radius);
        }

        /**
         * Move a Spherical location by the specified distance
         * @param loc The location to move.
         * @param dist The distance to move.
         * @note The dist parameter must be in the same units specified for the radius of the reference.
         */
        void move(Spherical &loc, double dist)
        {
            loc.move(dist / arcDistScale);
        }

        /**
         * Get the range between two locations.
         * @param loc1 The first location.
         * @param loc2 The second location.
         * @return The distance between locations.  The unit size is the same as the radius unit size.
         */
        double getRange(const Spherical &loc1, const Spherical &loc2) const
        {
            return loc1.getRange(loc2) * arcDistScale;
        }

        /**
         * Set the diameter of the sphere.
         * @param radius The radius of the sphere.
         * @note The unit size must match in all calls. For example, a sphere
         * with a radius of 1000 miles must then use miles as the unit in all calls to move.
         * That unit size will then be used for all return values.
         */
        void setZeroElevationRadius(double radius)
        {
            arcDistScale = 2.0f * pi * radius;
            arcDistScale /= 360;
            if(arcDistScale == 0)
            {
                arcDistScale = 0.000000001f;
            }
        }

    protected:
        double arcDistScale;

    };

}

#endif /* SPHERICAL_H */

