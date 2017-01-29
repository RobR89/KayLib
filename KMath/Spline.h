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

#include <glm/vec3.hpp>

#ifndef SPLINE_H
#define SPLINE_H

namespace KayLib
{

    class Spline
    {
    public:

        /**
         * Interpolate the point (p) in the Catmull-Rom spline defined by (points)
         * A Catmull-Rom spline always passes through the control points.
         * @param p The point to interpolate.
         * @param points The control points of the Catmull-Rom spline.
         * @param loop Whether or not to loop the spline.
         * @return The interpolated point.
         */
        template<int N>
        static glm::vec3 catmull_rom(double p, const glm::vec3(&points)[N], bool loop = false)
        {
            if(points == nullptr)
            {
                return glm::vec3();
            }
            int pLen = N;
            ControlPoints con;
            getSplinePoints(con, p, pLen, loop);
            if(con.ret != -1)
            {
                return points[con.ret];
            }
            glm::vec3 V;
            glm::vec3 v3, v2, v1;
            float t = p - ((int) p);
            float t2, t3;
            t2 = t * t;
            t3 = t2 * t;
            v3 = points[con.cp1] * -0.5f;
            v3 += points[con.cp2] * 1.5f;
            v3 += points[con.cp3] * -1.5f;
            v3 += points[con.cp4] * 0.5f;
            v3 *= t3;
            v2 = points[con.cp1];
            v2 += points[con.cp2] * -2.5f;
            v2 += points[con.cp3] * 2.f;
            v2 += points[con.cp4] * -0.5f;
            v2 *= t2;
            v1 = points[con.cp1] * -0.5f;
            v1 += points[con.cp3] * 0.5f;
            v1 *= t;
            V = points[con.cp2];
            V += v3;
            V += v2;
            V += v1;
            return V;
        }

        /**
         * Interpolate the point (p) in the Catmull-Rom spline defined by (points)
         * A Catmull-Rom spline always passes through the control points.
         * @param p The point to interpolate.
         * @param points The control points of the Catmull-Rom spline.
         * @param loop Whether or not to loop the spline.
         * @return The interpolated point.
         */
        template<int N>
        static glm::dvec3 catmull_rom(double p, const glm::dvec3(&points)[N], bool loop = false)
        {
            if(points == nullptr)
            {
                return glm::dvec3();
            }
            int pLen = N;
            ControlPoints con;
            getSplinePoints(con, p, pLen, loop);
            if(con.ret != -1)
            {
                return points[con.ret];
            }
            glm::dvec3 V;
            glm::dvec3 v3, v2, v1;
            double t = p - ((int) p);
            double t2, t3;
            t2 = t * t;
            t3 = t2 * t;
            v3 = points[con.cp1] * -0.5;
            v3 += points[con.cp2] * 1.5;
            v3 += points[con.cp3] * -1.5;
            v3 += points[con.cp4] * 0.5;
            v3 *= t3;
            v2 = points[con.cp1];
            v2 += points[con.cp2] * -2.5;
            v2 += points[con.cp3] * 2.0;
            v2 += points[con.cp4] * -0.5;
            v2 *= t2;
            v1 = points[con.cp1] * -0.5;
            v1 += points[con.cp3] * 0.5;
            v1 *= t;
            V = points[con.cp2];
            V += v3;
            V += v2;
            V += v1;
            return V;
        }

        /**
         * Interpolate the point (p) in the B-spline defined by (points)
         * The line of a B-spline is pulled toward the control points as if by an elastic band.
         * @param p The point to interpolate.
         * @param points The control points of the B-spline.
         * @param loop Whether or not to loop the spline.
         * @return The interpolated point.
         */
        template<int N>
        static glm::vec3 b_spline(double p, const glm::vec3(&points)[N], bool loop = false)
        {
            if(points == nullptr)
            {
                return glm::vec3();
            }
            int pLen = N;
            ControlPoints con;
            getSplinePoints(con, p, pLen, loop);
            if(con.ret != -1)
            {
                return points[con.ret];
            }
            glm::vec3 V;
            glm::vec3 v3, v2, v1;
            float t = p - ((int) p);
            float t2, t3;
            t2 = t * t;
            t3 = t2 * t;
            v3 = points[con.cp1] * -1.f;
            v3 += points[con.cp2] * 3.f;
            v3 += points[con.cp3] * -3.f;
            v3 += points[con.cp4];
            v3 *= t3;
            v2 = points[con.cp1] * 3.f;
            v2 += points[con.cp2] * -6.f;
            v2 += points[con.cp3] * 3.f;
            v2 *= t2;
            v1 = points[con.cp1] * -3.f;
            v1 += points[con.cp3] * 3.f;
            v1 *= t;
            V = points[con.cp2] * 4.f;
            V += points[con.cp1];
            V += points[con.cp3];
            V += v3;
            V += v2;
            V += v1;
            V *= 1.0f / 6.0f;
            return V;
        }

        /**
         * Interpolate the point (p) in the B-spline defined by (points)
         * The line of a B-spline is pulled toward the control points as if by an elastic band.
         * @param p The point to interpolate.
         * @param points The control points of the B-spline.
         * @param loop Whether or not to loop the spline.
         * @return The interpolated point.
         */
        template<int N>
        static glm::dvec3 b_spline(double p, const glm::dvec3(&points)[N], bool loop = false)
        {
            if(points == nullptr)
            {
                return glm::dvec3();
            }
            int pLen = N;
            ControlPoints con;
            getSplinePoints(con, p, pLen, loop);
            if(con.ret != -1)
            {
                return points[con.ret];
            }
            glm::dvec3 V;
            glm::dvec3 v3, v2, v1;
            double t = p - ((int) p);
            double t2, t3;
            t2 = t * t;
            t3 = t2 * t;
            v3 = points[con.cp1] * -1.0;
            v3 += points[con.cp2] * 3.0;
            v3 += points[con.cp3] * -3.0;
            v3 += points[con.cp4];
            v3 *= t3;
            v2 = points[con.cp1] * 3.0;
            v2 += points[con.cp2] * -6.0;
            v2 += points[con.cp3] * 3.0;
            v2 *= t2;
            v1 = points[con.cp1] * -3.0;
            v1 += points[con.cp3] * 3.0;
            v1 *= t;
            V = points[con.cp2] * 4.0;
            V += points[con.cp1];
            V += points[con.cp3];
            V += v3;
            V += v2;
            V += v1;
            V *= 1.0 / 6.0;
            return V;
        }

        /**
         * Interpolate the point (p) in the line spline defined by (points)
         * A straight liner interpolation of the points.
         * @param p The point to interpolate.
         * @param points The control points of the line spline.
         * @param loop Whether or not to loop the spline.
         * @return The interpolated point.
         */
        template<int N>
        static glm::vec3 linear(double p, const glm::vec3(&points)[N], bool loop = false)
        {
            if(points == nullptr)
            {
                return glm::vec3();
            }
            int pLen = N;
            ControlPoints con;
            getSplinePoints(con, p, pLen, loop);
            if(con.ret != -1)
            {
                return points[con.ret];
            }
            glm::vec3 V;
            float t = p - ((int) p);
            V = points[con.cp3] - points[con.cp2];
            V *= (t);
            V += (points[con.cp2]);
            return V;
        }

        /**
         * Interpolate the point (p) in the line spline defined by (points)
         * A straight liner interpolation of the points.
         * @param p The point to interpolate.
         * @param points The control points of the line spline.
         * @param loop Whether or not to loop the spline.
         * @return The interpolated point.
         */
        template<int N>
        static glm::dvec3 linear(double p, const glm::dvec3(&points)[N], bool loop = false)
        {
            if(points == nullptr)
            {
                return glm::dvec3();
            }
            int pLen = N;
            ControlPoints con;
            getSplinePoints(con, p, pLen, loop);
            if(con.ret != -1)
            {
                return points[con.ret];
            }
            glm::dvec3 V;
            double t = p - ((int) p);
            V = points[con.cp3] - points[con.cp2];
            V *= (t);
            V += (points[con.cp2]);
            return V;
        }

    protected:

        struct ControlPoints
        {
            int cp1;
            int cp2;
            int cp3;
            int cp4;
            int ret = -1;
        };

        static void getSplinePoints(ControlPoints &con, double p, int pLen, bool loop)
        {
            if(loop)
            {
                // looping the spline so normalize the interpolation point to the spline range.
                while(p < 0)
                {
                    p += pLen;
                }
                while(p >= pLen)
                {
                    p -= pLen;
                }
            }
            else
            {
                // not looping, return end point if out of bounds.
                if(p < 0)
                {
                    con.ret = 0;
                    return;
                }
                if(p > pLen - 1)
                {
                    con.ret = pLen - 1;
                    return;
                }
            }
            con.cp1 = ((int) p) - 1;
            con.cp2 = ((int) p);
            con.cp3 = ((int) p) + 1;
            con.cp4 = ((int) p) + 2;
            if(loop)
            {
                // Looping, adjust interpolation points if they loop.
                while(con.cp1 < 0)
                {
                    con.cp1 += pLen;
                }
                while(con.cp3 > pLen - 1)
                {
                    con.cp3 -= pLen;
                }
                while(con.cp4 > pLen - 1)
                {
                    con.cp4 -= pLen;
                }
            }
            else
            {
                // Not looping, crop final points to end of spline.
                if(con.cp1 < 0)
                {
                    con.cp1 = 0;
                }
                if(con.cp3 > pLen - 1)
                {
                    con.cp3 = pLen - 1;
                }
                if(con.cp4 > pLen - 1)
                {
                    con.cp4 = pLen - 1;
                }
            }
        }

    };

}


#endif /* SPLINE_H */

