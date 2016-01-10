/*
 * Copyright 2016 Robert Reinhart.
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

#ifndef SPLINE_H
#define	SPLINE_H

namespace KayLib {

class Spline {
public:
  /**
   * Interpolate the point (p) in the Catmull-Rom spline defined by (points)
   * A Catmull-Rom spline always passes through the control points.
   * @param p The point to interpolate.
   * @param points The control points of the Catmull-Rom spline.
   * @param loop Whether or not to loop the spline.
   * @return The interpolated point.
   */
  static Vector3D catmull_rom(double p, const Vector3D points[], bool loop);

  /**
   * Interpolate the point (p) in the B-spline defined by (points)
   * The line of a B-spline is pulled toward the control points as if by an elastic band.
   * @param p The point to interpolate.
   * @param points The control points of the B-spline.
   * @param loop Whether or not to loop the spline.
   * @return The interpolated point.
   */
  static Vector3D b_spline(double p, const Vector3D points[], bool loop);

  /**
   * Interpolate the point (p) in the line spline defined by (points)
   * A straight liner interpolation of the points.
   * @param p The point to interpolate.
   * @param points The control points of the line spline.
   * @param loop Whether or not to loop the spline.
   * @return The interpolated point.
   */
  static Vector3D linear(double p, const Vector3D points[], bool loop);
};

}


#endif	/* SPLINE_H */

