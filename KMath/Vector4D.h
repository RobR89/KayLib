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

/**
 * A 4 Dimensional double vector.
 */

#ifndef VECTOR4D_H
#define	VECTOR4D_H

namespace KayLib {

class Vector4D {
public:
  double x, y, z, w;

  Vector4D();
  Vector4D(double nx, double ny, double nz);
  Vector4D(double nx, double ny, double nz, double nw);
  Vector4D(const Vector3D &v);
  Vector4D(const Vector4D &v);

  /**
   * Return a copy of the vector.
   * @return The copy of the vector.
   */
  Vector4D copy() const {
    return Vector4D(*this);
  }

  /**
   * Retrieves the length of the Vector.
   * @return The length of the vector.
   */
  double magnitude() const;

  /**
   * Retrieves the length of the Vector in homogeneous space.
   * @return The length of the vector.
   */
  double magnitude_H() const;

  /**
   * Scale the Vector so it has a length of 1.
   */
  void normalize();

  /**
   * Scale the Vector so it has a length of 1 in homogeneous space.
   */
  void normalize_H();

  /**
   * Calculate the dot product of this vector and the provided vector.
   * @param v The provided vector.
   * @return The dot product.
   * @note If both Vectors are unit Vectors then the result is the cosine of the angle between them.
   * otherwise it's scaled by the product of there lengths.
   */
  double dotProduct(const Vector4D &v) const;

  /**
   * Calculate the dot product of this vector and the provided vector in homogeneous space.
   * @param v The provided vector.
   * @return The dot product.
   * @note If both Vectors are unit Vectors then the result is the cosine of the angle between them.
   * otherwise it's scaled by the product of there lengths.
   */
  double dot_H(const Vector4D &v) const;

  /**
   * Calculate the cross product of this vector and the provided vector.
   * @param v The provided vector.
   * @return The cross product of the 2 vectors.
   * @note The resultant vector is perpendicular to a plane formed by the two vectors.
   * @note The magnitude of the result is the area formed by the parallelogram of the two vectors.
   */
  Vector4D crossProduct(const Vector3D &v) const;

  /**
   * Calculate the cross product of this vector and the provided vector in homogeneous space.
   * @param v The provided vector.
   * @return The cross product of the 2 vectors.
   * @note The resultant vector is perpendicular to a plane formed by the two vectors.
   * @note The magnitude of the result is the area formed by the parallelogram of the two vectors.
   */
  Vector4D cross_H(const Vector4D &v) const;

  /**
   * Calculate the reflection vector for a surface with the specified normal.
   * @param norm The surface normal of the surface to reflect from.
   * @return The reflection vector.
   */
  Vector4D reflection(Vector4D norm) const;

  /**
   * Calculate the refraction vector for a surface with the specified normal.
   * @param norm The surface normal of the surface to refract through.
   * @param fact The refraction factor.
   * @return The refraction vector.
   */
  Vector4D refraction(Vector4D norm, double fact) const;

  /**
   * Set the values of the vector.
   * @param nx The new value for X.
   * @param ny The new value for Y.
   * @param nz The new value for Z.
   */
  void set(double nx, double ny, double nz);
  /**
   * Set the values of the vector.
   * @param nx The new value for X.
   * @param ny The new value for Y.
   * @param nz The new value for Z.
   * @param nw The new value for W.
   */
  void set(double nx, double ny, double nz, double nw);
  /**
   * Set the values of the vector.
   * @param v The new value for the vector.
   */
  void set(const Vector3D &v);
  /**
   * Set the values of the vector.
   * @param v The new value for the vector.
   */
  void set(const Vector4D &v);
  /**
   * Set this vector to the axis of the quaternion.
   * @param q The quaternion.
   */
  void set(Quaternion q);

  /**
   * Compare this vector to the specified vector to determine if they are equal.
   * @param v The vector to compare to.
   * @return True if equal false if not.
   */
  bool equals(const Vector4D &v) const {
    return v.x == x && v.y == y && v.z == z && v.w == w;
  }

  Vector4D& operator=(const Vector4D &v);

  Vector4D operator+(const Vector4D &v) const;
  Vector4D& operator+=(const Vector4D &v);
  Vector4D operator-(const Vector4D &v) const;
  Vector4D& operator-=(const Vector4D &v);
  Vector4D operator*(const Vector4D &v) const;
  Vector4D& operator*=(const Vector4D &v);
  Vector4D operator/(const Vector4D &v) const;
  Vector4D& operator/=(const Vector4D &v);

  //scale the Vector3D
  Vector4D operator*(const double &k) const;
  Vector4D& operator*=(const double &k);
  Vector4D operator/(const double &k) const;
  Vector4D& operator/=(const double &k);

  Vector4D operator*(const Matrix &m) const;
  Vector4D& operator*=(const Matrix &m);
};

}

#endif	/* VECTOR4D_H */

