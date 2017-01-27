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
#include <cstdlib>

namespace KayLib
{

    Quaternion::Quaternion()
    {
        w = x = y = z = 0;
    }

    Quaternion::Quaternion(const Quaternion &q)
    {
        w = q.w;
        x = q.x;
        y = q.y;
        z = q.z;
    }

    Quaternion Quaternion::quaternionFromAxisAngle(const AxisAngle &aa)
    {
        Quaternion q;
        q.set(aa);
        return q;
    }

    Quaternion Quaternion::quaternionFromAxisAngle(double angle, const Vector3D &axis)
    {
        Quaternion q;
        q.set(angle, axis);
        return q;
    }

    Quaternion Quaternion::quaternionFromMatrix(const Matrix &m)
    {
        Quaternion q;
        q = m;
        return q;
    }

    void Quaternion::set(double ax, double ay, double az)
    {
        Quaternion Q1, Q2, Q3;
        Vector3D v = Vector3D(1, 0, 0);
        Q1 = quaternionFromAxisAngle(ax, v);
        v.set(0, 1, 0);
        Q2 = quaternionFromAxisAngle(ay, v);
        v.set(0, 0, 1);
        Q3 = quaternionFromAxisAngle(az, v);
        *this = (Q3 * Q2) * Q1;
    }

    void Quaternion::set_deg(double ax, double ay, double az)
    {
        double rx = degreesToRadians(ax);
        double ry = degreesToRadians(ay);
        double rz = degreesToRadians(az);
        set(rx, ry, rz);
    }

    void Quaternion::set(const Vector3D &a)
    {
        set(a.x, a.y, a.z);
    }

    void Quaternion::set_deg(const Vector3D &a)
    {
        set_deg(a.x, a.y, a.z);
    }

    void Quaternion::set(const AxisAngle &aa)
    {
        set(aa.angle, aa.axis);
    }

    void Quaternion::set(double angle, const Vector3D &axis)
    {
        w = cos(angle / 2);
        double sa = sin(angle / 2);
        // adjust to normalize axis
        sa /= axis.magnitude();
        x = axis.x * sa;
        y = axis.y * sa;
        z = axis.z * sa;
        normalize();
    }

    void Quaternion::set(const LatLonHead &loc)
    {
        set(loc.Latitude, loc.Longatude, loc.Heading);
    }

    void Quaternion::setLatLon(double Latitude, double Longitude, double Heading)
    {
        double sa = sin(Heading / 2);
        double ca = cos(Heading / 2);
        double slat = sin(Latitude);
        double clat = cos(Latitude);
        double slon = sin(Longitude);
        double clon = cos(Longitude);

        x = sa * clat * slon;
        y = sa * slat;
        z = sa * slat * clon;
        w = ca;

        normalize();
    }

    AxisAngle Quaternion::toAxisAngle() const
    {
        AxisAngle aa;
        Quaternion q(*this);
        q.normalize();
        double ca = q.w;
        aa.angle = acos(ca) * 2;
        double sa = sqrt(1.0f - ca * ca);
        if(abs(sa) < 0.00000001)
        {
            sa = 1;
        }
        aa.axis = Vector3D(q.x / sa, q.y / sa, q.z / sa);
        aa.axis.normalize();
        return aa;
    }

    Vector3D Quaternion::toEulerAngles() const
    {
        double r11, r21, r31, r32, r33, r12, r13;
        double q00, q11, q22, q33;
        double tmp;
        Vector3D u = Vector3D();

        q00 = w * w;
        q11 = x * x;
        q22 = y * y;
        q33 = z * z;

        r11 = q00 + q11 - q22 - q33;
        r21 = 2 * (x * y + w * z);
        r31 = 2 * (x * z - w * y);
        r32 = 2 * (y * z + w * x);
        r33 = q00 - q11 - q22 + q33;

        tmp = abs(r31);
        if(tmp > 0.999999)
        {
            r12 = 2 * (x * y - w * z);
            r13 = 2 * (x * z + w * y);
            u.x = 0.0f; //roll
            u.y = (-(pi / 2) * r31 / tmp); // pitch
            u.z = atan2(-r12, -r31 * r13); // yaw
            return u;
        }

        u.x = atan2(r32, r33); // roll
        u.y = asin(-r31); // pitch
        u.z = atan2(r21, r11); // yaw

        return u;
    }

    LatLonHead Quaternion::toLatLon() const
    {
        LatLonHead llh;
        Quaternion q(*this);
        q.normalize();
        double lat, lon;

        double ca = q.w;
        double sa = sqrt(1.0 - ca * ca);
        llh.Heading = acos(ca) * 2;

        if(abs(sa) < 0.0000001)
        {
            sa = 1;
        }

        double tx = q.x / sa;
        double ty = q.y / sa;
        double tz = q.z / sa;

        lat = -asin(ty);
        if(tx * tx + tz * tz < 0.0000001)
        {
            lon = 0;
        }
        else
        {
            lon = atan2(tx, tz);
        }
        if(lon < 0)
        {
            lon += 360.0;
        }

        llh.Latitude = lat;
        llh.Longatude = lon;
        return llh;
    }

    Quaternion Quaternion::conjugate() const
    {
        Quaternion nq = Quaternion();
        nq.w = w;
        nq.x = -x;
        nq.y = -y;
        nq.z = -z;
        return nq;
    }

    void Quaternion::inverse()
    {
        double d = w * w + x * x + y * y + z * z;
        w /= d;
        x = (-x) / d;
        y = (-y) / d;
        z = (-z) / d;
    }

    double Quaternion::magnitude() const
    {
        return sqrt(w * w + x * x + y * y + z * z);
    }

    void Quaternion::normalize()
    {
        double m = magnitude();
        x /= m;
        y /= m;
        z /= m;
        w /= m;
    }

    Quaternion& Quaternion::operator=(const Quaternion &q)
    {
        x = q.x;
        y = q.y;
        z = q.z;
        w = q.w;
        return *this;
    }

    Quaternion Quaternion::operator+(const Quaternion &q) const
    {
        return Quaternion(*this) += q;
    }

    Quaternion& Quaternion::operator+=(const Quaternion &q)
    {
        w += q.w;
        x += q.x;
        y += q.y;
        z += q.z;
        return *this;
    }

    Quaternion Quaternion::operator-(const Quaternion &q) const
    {
        return Quaternion(*this) -= q;
    }

    Quaternion& Quaternion::operator-=(const Quaternion &q)
    {
        w -= q.w;
        x -= q.x;
        y -= q.y;
        z -= q.z;
        return *this;
    }

    Quaternion Quaternion::operator*(const Quaternion &q) const
    {
        return Quaternion(*this) *= q;
    }

    Quaternion& Quaternion::operator*=(const Quaternion &q)
    {
        double nw, nx, ny, nz;

        nw = w * q.w - x * q.x - y * q.y - z * q.z;
        nx = w * q.x + x * q.w + y * q.z - z * q.y;
        ny = w * q.y + y * q.w + z * q.x - x * q.z;
        nz = w * q.z + z * q.w + x * q.y - y * q.x;

        w = nw;
        x = nx;
        y = ny;
        z = nz;

        return *this;
    }

    Quaternion Quaternion::operator/(const Quaternion &q) const
    {
        return Quaternion(*this) /= q;
    }

    Quaternion& Quaternion::operator/=(const Quaternion &q)
    {
        inverse();
        (*this) * q;
        return *this;
    }

    Quaternion& Quaternion::operator=(const Matrix &m)
    {
        double tr, s;
        tr = 1 + m.e[0] + m.e[5] + m.e[10];

        // check the diagonal
        if(tr > 0.00000001)
        {
            s = 0.5 / sqrt(tr);
            x = (m.e[9] - m.e[6]) * s;
            y = (m.e[2] - m.e[8]) * s;
            z = (m.e[4] - m.e[1]) * s;
            w = 0.25 / s;
        }
        else
        {
            // diagonal is negative
            if(m.e[0] > m.e[5] && m.e[0] > m.e[10])
            {
                s = 2 * sqrt(1.0f + m.e[0] - m.e[5] - m.e[10]);
                x = 0.5 / s;
                y = (m.e[1] + m.e[4]) / s;
                z = (m.e[2] + m.e[8]) / s;
                w = (m.e[6] - m.e[9]) / s;
            }
            else if(m.e[5] > m.e[10])
            {
                s = 2 * sqrt(1.0f + m.e[5] - m.e[0] - m.e[10]);
                x = (m.e[1] + m.e[4]) / s;
                y = 0.5 / s;
                z = (m.e[6] + m.e[9]) / s;
                w = (m.e[2] + m.e[8]) / s;
            }
            else
            {
                s = 2 * sqrt(1.0f + m.e[10] - m.e[0] - m.e[5]);
                x = (m.e[2] + m.e[8]) / s;
                y = (m.e[6] + m.e[9]) / s;
                z = 0.5 / s;
                w = (m.e[1] + m.e[4]) / s;
            }
        }
        // normalize needed to prevent errors in some cases
        normalize();
        return *this;
    }

    Quaternion &Quaternion::operator=(const AxisAngle &aa)
    {
        set(aa);
        return *this;
    }

}
