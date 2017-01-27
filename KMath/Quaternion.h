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

/*
 * This code was based on David M. Bourg's code in "Physics for Game Developers" O'Riley publishing c2002 ISBN: 0-596-00006-5
 */

#include "KMath.h"

#ifndef QUATERNION_H
#define QUATERNION_H

namespace KayLib
{

    class AxisAngle
    {
    public:
        double angle;
        Vector3D axis;

    };

    class Quaternion
    {
    public:
        double w, x, y, z;

        Quaternion();
        Quaternion(const Quaternion &q);

        /**
         * Create a quaternion from an axis and angle.
         * @param aa The axis and angle value.
         * @return The new quaternion.
         */
        static Quaternion quaternionFromAxisAngle(const AxisAngle &aa);
        /**
         * Create a quaternion from an axis and angle.
         * @param angle The angle.
         * @param axis The axis.
         * @return The new quaternion.
         */
        static Quaternion quaternionFromAxisAngle(double angle, const Vector3D &axis);
        /**
         * Create a quaternion from a matrix.
         * @param m The matrix to create the quaternion from.
         * @return The new quaternion.
         */
        static Quaternion quaternionFromMatrix(const Matrix &m);

        /**
         * Set this quaternion to the euler angles.
         * @param ax The rotation around the x axis in radians.
         * @param ay The rotation around the y axis in radians.
         * @param az The rotation around the z axis in radians.
         */
        void set(double ax, double ay, double az);
        /**
         * Set this quaternion to the euler angles.
         * @param ax The rotation around the x axis in degrees.
         * @param ay The rotation around the y axis in degrees.
         * @param az The rotation around the z axis in degrees.
         */
        void set_deg(double ax, double ay, double az);
        /**
         * Set this quaternion to the euler angles.
         * @param a The rotation angles in radians.
         */
        void set(const Vector3D &a);
        /**
         * Set this quaternion to the euler angles.
         * @param a The rotation angles in degrees.
         */
        void set_deg(const Vector3D &a);
        /**
         * Set this quaternion to the axis and angle values.
         * @param aa The axis and angle values in radians.
         */
        void set(const AxisAngle &aa);
        /**
         * Set this quaternion to the axis and angle values.
         * @param angle The angle in radians.
         * @param axis The axis in radians.
         */
        void set(double angle, const Vector3D &axis);
        /**
         * Set the quaternion to the latitude, longitude, and heading values.
         * @param Latitude The latitude in radians.
         * @param Longitude The longitude in radians.
         * @param Heading The heading in radians.
         */
        void setLatLon(double Latitude, double Longitude, double Heading);
        /**
         * Set the quaternion to the latitude, longitude, and heading values.
         * @param loc The location to set the quaternion to.
         */
        void set(const LatLonHead &loc);

        /**
         * Get the axis angle and rotation of this quaternion.
         * @return The axis angle and rotation of this quaternion.
         */
        AxisAngle toAxisAngle() const;
        /**
         * Get the euler angle representation of this quaternion.
         * @return The euler angles in radians.
         */
        Vector3D toEulerAngles() const;
        /**
         * Get the latitude, longitude, and heading values represented by this quaternion on a sphere of radius 1.
         * @return The latitude, longitude, and heading values in radians.
         */
        LatLonHead toLatLon() const;

        /**
         * Get the conjugate of the quaternion.
         * @return The conjugate.
         */
        Quaternion conjugate() const;

        /**
         * Invert the quaternion.
         */
        void inverse();
        /**
         * Get the magnitude of this quaternion.
         * @return The magnitude of the quaternion.
         */
        double magnitude() const;
        /**
         * Normalize this quaternion.
         */
        void normalize();

        /**
         * Check of the quaternions are equal
         * @param q The quaternion to check against.
         * @return
         */
        bool equals(const Quaternion &q)
        {
            return w == q.w && x == q.x && y == q.y && z == q.z;
        }

        Quaternion& operator=(const Quaternion &q);
        Quaternion operator+(const Quaternion &q) const;
        Quaternion& operator+=(const Quaternion &q);
        Quaternion operator-(const Quaternion &q) const;
        Quaternion& operator-=(const Quaternion &q);
        Quaternion operator*(const Quaternion &q) const;
        Quaternion& operator*=(const Quaternion &q);
        Quaternion operator/(const Quaternion &q) const;
        Quaternion& operator/=(const Quaternion &q);

        Quaternion& operator=(const Matrix &m);
        Quaternion& operator=(const AxisAngle &aa);

    };

}

#endif /* QUATERNION_H */

