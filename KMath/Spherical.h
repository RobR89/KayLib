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

        Spherical()
        {
            w = x = y = z = 0;
        }

        /**
         * Get the latitude, longitude, and heading of the current state.
         * @return A structure containing the Latitude, longitude, and heading in degrees.
         */
        LatLonHead get() const
        {
            LatLonHead llh = get_rad();
            llh.Latitude = radiansToDegrees(llh.Latitude);
            llh.Longatude = radiansToDegrees(llh.Longatude);
            llh.Heading = radiansToDegrees(llh.Heading);
            return llh;
        }

        /**
         * Get the latitude and longitude of the current state.
         * @return A structure containing the Latitude and longitude in degrees.
         */
        LatLon getLocation() const
        {
            LatLon ll = getLocation_rad();
            ll.Latitude = radiansToDegrees(ll.Latitude);
            ll.Longatude = radiansToDegrees(ll.Longatude);
            return ll;
        }

        /**
         * Get the heading of the current state.
         * @return The heading in degrees.
         */
        double getHeading() const
        {
            return radiansToDegrees(getHeading_rad());
        }

        /**
         * Get the latitude, longitude, and heading of the current state.
         * @return A structure containing the Latitude, longitude, and heading in radians.
         */
        LatLonHead get_rad() const
        {
            double la, lo, hd;
            double y1, y3, x1, z1;

            double wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;
            x2 = x + x;
            y2 = y + y;
            z2 = z + z;
            xx = x * x2;
            xy = x * y2;
            xz = x * z2;
            yy = y * y2;
            yz = y * z2;
            zz = z * z2;
            wx = w * x2;
            wy = w * y2;
            wz = w * z2;

            x1 = xz + wy;

            y3 = xy + wz;
            y2 = 1.0f - (xx + zz);
            y1 = yz - wx;

            z1 = 1.0f - (xx + yy);

            la = std::asin(y1);
            double clat = std::cos(la);

            // scale latatudnal ring to unit size
            lo = std::sqrt(1.0 - (y1 * y1));
            lo = x1 / lo;
            // calculate longitude
            lo = std::asin(lo);
            if(z1 < 0)
            {
                lo = pi - lo;
            }
            else if(x1 < 0)
            {
                lo = (2 * pi) + lo;
            }

            if(clat == 0)
            {
                y2 = 0;
            }
            else
            {
                y2 /= clat;
            }
            hd = y2;
            if(hd > 1)
            {
                hd = 1;
            }
            if(hd < -1)
            {
                hd = -1;
            }
            hd = std::acos(hd);
            if(y3 > 0)
            {
                hd = (2 * pi) - hd;
            }

            LatLonHead llh = LatLonHead();
            llh.Latitude = la;
            llh.Longatude = lo;
            llh.Heading = hd;
            return llh;
        }

        /**
         * Get the latitude and longitude of the current state.
         * @return A structure containing the Latitude and longitude in radians.
         */
        LatLon getLocation_rad() const
        {
            double la, lo;
            double y1, x1, z1;

            double wx, wy, xx, yy, yz, xz, x2, y2, z2;
            x2 = x + x;
            y2 = y + y;
            z2 = z + z;
            xx = x * x2;
            xz = x * z2;
            yy = y * y2;
            yz = y * z2;
            wx = w * x2;
            wy = w * y2;

            x1 = xz + wy;

            y1 = yz - wx;

            z1 = 1.0f - (xx + yy);

            la = std::asin(y1);

            // scale latitudinal ring to unit size
            lo = std::sqrt(1.0 - (y1 * y1));
            lo = x1 / lo;
            // calculate longitude
            lo = std::asin(lo);
            if(z1 < 0)
            {
                lo = pi - lo;
            }
            else if(x1 < 0)
            {
                lo = (2 * pi) + lo;
            }

            LatLon ll = LatLon();
            ll.Latitude = la;
            ll.Longatude = lo;
            return ll;
        }

        /**
         * Get the heading of the current state.
         * @return The heading in radians.
         */
        double getHeading_rad() const
        {
            double y1, y3;
            double wz, xx, xy, zz, x2, y2, z2, yz, wx;
            x2 = x + x;
            y2 = y + y;
            z2 = z + z;
            xx = x * x2;
            xy = x * y2;
            zz = z * z2;
            yz = y * z2;
            wx = w * x2;
            wz = w * z2;

            y1 = yz - wx;
            y3 = xy + wz;
            y2 = 1.0f - (xx + zz);

            double clat = std::sqrt(1 - (y1 * y1));

            if(clat == 0)
            {
                y2 = 0;
            }
            else
            {
                y2 /= clat;
            }
            double hd = y2;
            if(hd > 1)
            {
                hd = 1;
            }
            if(hd < -1)
            {
                hd = -1;
            }
            hd = std::acos(hd);
            if(y3 > 0)
            {
                hd = (2 * pi) - hd;
            }

            return hd;
        }

        /**
         * Set the latitude, longitude, and heading of the state.
         * @param lat The latitude in degrees.
         * @param lon The longitude in degrees.
         * @param head The heading in degrees.
         */
        void set(double lat, double lon, double head)
        {
            double hd = degreesToRadians(head);
            double la = degreesToRadians(lat);
            double lo = degreesToRadians(lon);
            set_rad(la, lo, hd);
        }

        /**
         * Set the latitude, longitude, and heading of the state.
         * @param lat The latitude in radians.
         * @param lon The longitude in radians.
         * @param head The heading in radians.
         */
        void set_rad(double lat, double lon, double head)
        {
            double w1, z1, w2, y2, w3, x3;
            double w23, x23, y23, z23;

            w1 = std::cos(-head / 2);
            z1 = std::sin(-head / 2);
            w2 = std::cos(lon / 2);
            y2 = std::sin(lon / 2);
            w3 = std::cos(-lat / 2);
            x3 = std::sin(-lat / 2);

            w23 = w2 * w3;
            x23 = w2 * x3;
            y23 = y2 * w3;
            z23 = -y2 * x3;

            w = w23 * w1 - z23 * z1;
            x = x23 * w1 + y23 * z1;
            y = y23 * w1 - x23 * z1;
            z = w23 * z1 + z23 * w1;
        }

        /**
         * Set the location of the state.
         * @param lat The latitude in degrees.
         * @param lon The longitude in degrees.
         */
        void setLocation(double lat, double lon)
        {
            set(lat, lon, getHeading());
        }

        /**
         * Set the location of the state.
         * @param lat The latitude in radians.
         * @param lon The longitude in radians.
         */
        void setLocation_rad(double lat, double lon)
        {
            set_rad(lat, lon, getHeading_rad());
        }

        /**
         * Set the heading of the state.
         * @param head The heading in degrees.
         */
        void setHeading(double head)
        {
            LatLon ll = getLocation();
            set(ll.Latitude, ll.Longatude, head);
        }

        /**
         * Set the heading of the state.
         * @param head The heading in radians.
         */
        void setHeading_rad(double head)
        {
            LatLon ll = getLocation_rad();
            set_rad(ll.Latitude, ll.Longatude, head);
        }

        /**
         * Get the range to a target in degrees of rotation.
         * @param Target The target to range.
         * @return The Range in degrees of rotation around the globe.
         */
        double getRange(const Spherical &target) const
        {
            return radiansToDegrees(getRange_rad(target));
        }

        /**
         * Get the range to a target in radians of rotation.
         * @param Target The target to range.
         * @return The Range in radians of rotation around the globe.
         */
        double getRange_rad(const Spherical &target) const
        {
            double wx, wy, xx, yy, yz, xz, x2, y2, z2;

            x2 = x + x;
            y2 = y + y;
            z2 = z + z;
            xx = x * x2;
            xz = x * z2;
            yy = y * y2;
            yz = y * z2;
            wx = w * x2;
            wy = w * y2;

            double X, Y, Z;
            X = xz + wy;
            Y = yz - wx;
            Z = 1.0f - (xx + yy);

            x2 = target.x + target.x;
            y2 = target.y + target.y;
            z2 = target.z + target.z;
            xx = target.x * x2;
            xz = target.x * z2;
            yy = target.y * y2;
            yz = target.y * z2;
            wx = target.w * x2;
            wy = target.w * y2;

            X *= xz + wy;
            Y *= yz - wx;
            Z *= 1.0f - (xx + yy);

            return std::acos(X + Y + Z);
        }

        /**
         * Get the bearing to the target.
         * @param Target The target to get the bearing to.
         * @return The bearing in degrees. (positive is right/clockwise, negative is left/counterclockwise)
         */
        double getBearing(const Spherical &target) const
        {
            return radiansToDegrees(getBearing_rad(target));
        }

        /**
         * Get the bearing to the target.
         * @param Target The target to get the bearing to.
         * @return The bearing in radians. (positive is right/clockwise, negative is left/counterclockwise)
         */
        double getBearing_rad(const Spherical &target) const
        {
            double dlon, dlat, a, d;
            double bear;

            LatLonHead llh = get_rad();
            LatLon tl = target.getLocation_rad();

            dlon = tl.Longatude - llh.Longatude;
            dlat = tl.Latitude - llh.Latitude;
            a = sqr(std::sin(dlat / 2)) + std::cos(llh.Latitude) * std::cos(tl.Latitude) * sqr(std::sin(dlon / 2));
            d = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));

            bear = std::acos((std::sin(tl.Latitude) - std::sin(llh.Latitude) * std::cos(d)) / (std::sin(d) * std::cos(llh.Latitude)));
            // If target is to the west.
            if(std::sin(tl.Longatude - llh.Longatude) < 0)
            {
                bear = (2 * pi) - bear;
            }

            bear -= llh.Heading;
            // if bearing is less than -180 degrees reverse the direction.
            if(bear < -pi)
            {
                // note: this condition may not be possible?  Should be tested for possible optimization.
                bear = (2 * pi) - bear;
            }
            // If bearing is greater than 180 degrees reverse the direction.
            if(bear > pi)
            {
                bear = -((2 * pi) - bear);
            }
            // If bearing is 360 degrees return it as zero.
            if(bear == 2 * pi)
            {
                bear = 0;
            }

            return bear;
        }

        /**
         * Move a specified distance around the globe in degrees.
         * @param arc The distance in degrees.
         */
        void move(double arc)
        {
            move_rad(degreesToRadians(arc));
        }

        /**
         * Move a specified distance around the globe in radians.
         * @param arc The distance in radians.
         */
        void move_rad(double arc)
        {
            double w3, x3, w1, x1, y1, z1;

            w3 = std::cos(arc / 2);
            x3 = std::sin(arc / 2);

            w1 = w;
            x1 = x;
            y1 = y;
            z1 = z;

            w = w1 * w3 - x1 * x3;
            x = w1 * x3 + x1 * w3;
            y = y1 * w3 + z1 * x3;
            z = z1 * w3 - y1 * x3;
        }

        /**
         * Turn a number of degrees to the right (relative to the direction of travel)
         * @param turn Degrees to turn.
         */
        void turn(double turn)
        {
            return turn_rad(degreesToRadians(turn));
        }

        /**
         * Turn a number of degrees to the right (relative to the direction of travel)
         * @param turn Degrees to turn.
         */
        void turn_rad(double turn)
        {
            double w1, z1;
            double w2, x2, y2, z2;

            w1 = std::cos(turn / 2);
            z1 = std::sin(turn / 2);

            w2 = w;
            x2 = x;
            y2 = y;
            z2 = z;

            w = w2 * w1 - z2 * z1;
            x = x2 * w1 + y2 * z1;
            y = y2 * w1 - x2 * z1;
            z = w2 * z1 + z2 * w1;
        }

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

