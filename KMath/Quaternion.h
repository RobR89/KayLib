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

#ifndef QUATERNION_H
#define	QUATERNION_H

namespace KayLib {

class AxisAngle {
public:
  double angle;
  Vector3D axis;

};

class Quaternion {
public:
  double w, x, y, z;

  Quaternion();
  Quaternion(const Quaternion &q);
  /**
   * Set this quaternion to the euler angles.
   * @param ax The rotation around the x axis in radians.
   * @param ay The rotation around the y axis in radians.
   * @param az The rotation around the z axis in radians.
   */
  void eulerToQuat(double ax, double ay, double az);
  /**
   * Set this quaternion to the euler angles.
   * @param ax The rotation around the x axis in degrees.
   * @param ay The rotation around the y axis in degrees.
   * @param az The rotation around the z axis in degrees.
   */
  void eulerToQuat_deg(double ax, double ay, double az);
  /**
   * Set this quaternion to the euler angles.
   * @param a The rotation angles in radians.
   */
  void eulerToQuat(Vector3D a);
  /**
   * Set this quaternion to the euler angles.
   * @param a The rotation angles in degrees.
   */
  void eulerToQuat_deg(Vector3D a);
  /**
   * Set this quaternion to the axis and angle values.
   * @param aa The axis and angle values in radians.
   */
  void axisAngleToQuat(AxisAngle aa);
  /**
   * Set this quaternion to the axis and angle values.
   * @param angle The angle in radians.
   * @param axis The axis in radians.
   */
  void axisAngleToQuat(double angle, Vector3D axis);

  /**
   * Create a quaternion from an axis and angle.
   * @param aa The axis and angle value.
   * @return The new quaternion.
   */
  static Quaternion quaternionFromAxisAngle(AxisAngle aa);

  /**
   * Create a quaternion from an axis and angle.
   * @param angle The angle.
   * @param axis The axis.
   * @return The new quaternion.
   */
  static Quaternion quaternionFromAxisAngle(double angle, Vector3D axis);

  /**
   * Get the axis angle and rotation of this quaternion.
   * @return The axis angle and rotation of this quaternion.
   */
  AxisAngle quatToAxisAngle();
  /**
   * Get the euler angle representation of this quaternion.
   * @return The euler angles in radians.
   */
  Vector3D getEulerAngles();
  /**
   * Set the quaternion to the latitude, longitude, and heading values.
   * @param Latitude The latitude in radians.
   * @param Longitude The longitude in radians.
   * @param Heading The heading in radians.
   */
  void latLonHeadToQuat(double Latitude, double Longitude, double Heading);
  /**
   * Set the quaternion to the latitude, longitude, and heading values.
   * @param loc The location to set the quaternion to.
   */
  void latLonHeadToQuat(const LatLonHead &loc);
  /**
   * Get the latitude, longitude, and heading values represented by this quaternion on a sphere of radius 1.
   * @return The latitude, longitude, and heading values in radians.
   */
  LatLonHead quatToLatLon();
  /**
   * Set this quaternion to the values of another quaternion.
   * @param q The quaternion to set to.
   */
  void set(const Quaternion &q);

  Quaternion addTo(const Quaternion &q);
  void add(const Quaternion &q);
  Quaternion subtractFrom(const Quaternion &q);
  void subtract(const Quaternion &q);
  Quaternion multiplyBy(const Quaternion &q);
  void multiply(const Quaternion &q);
  Quaternion divideBy(const Quaternion &q);
  void divide(const Quaternion &q);
  /**
   * Get the conjugate of the quaternion.
   * @return The conjugate.
   */
  Quaternion conjugate();


  /**
   * Orient a point in space to quaternion space.
   * @param v The point to orient.
   * @return The new point.
   */
  Vector3D orient(const Vector3D &v);
  /**
   * Orient a point in space to quaternion space.
   * @param v The point to orient.
   * @return The new point.
   */
  Vector4D orient(const Vector4D &v);
  /**
   * Create a quaternion from a matrix.
   * @param m The matrix to create the quaternion from.
   * @return The new quaternion.
   */
  static Quaternion quaternionFromMatrix(const Matrix &m);
  /**
   * Invert the quaternion.
   */
  void inverse();
  /**
   * Get the magnitude of this quaternion.
   * @return The magnitude of the quaternion.
   */
  double magnitude();
  /**
   * Normalize this quaternion.
   */
  void normalize();

  /**
   * Check of the quaternions are equal
   * @param q The quaternion to check against.
   * @return
   */
  bool equals(const Quaternion &q) {
    return w == q.w && x == q.x && y == q.y && z == q.z;
  }

};

}

#endif	/* QUATERNION_H */

