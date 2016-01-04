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
 * A 3 dimensional double vector.
 */

#ifndef VECTOR3D_H
#define	VECTOR3D_H

namespace KayLib {

class Vector3D {
public:
  double x, y, z;

  Vector3D() {
    x = y = z = 0;
  }

  Vector3D(double nx, double ny, double nz) {
    x = nx;
    y = ny;
    z = nz;
  }

  Vector3D(const Vector3D &v) {
    x = v.x;
    y = v.y;
    z = v.z;
  }

  Vector3D(const Vector4D &v);

  /**
   * Return a copy of the vector.
   * @return The copy of the vector.
   */
  Vector3D copy() const {
    return Vector3D(*this);
  }

  /**
   * Retrieves the length of the Vector.
   * @return The length of the vector.
   */
  double magnitude() const;

  /**
   * Scale the Vector so it has a length of 1.
   */
  void normalize();

  /**
   * Calculate the dot product of this vector and the provided vector.
   * @param v The provided vector.
   * @return The dot product.
   * @note If both Vectors are unit Vectors then the result is the cosine of the angle between them.
   * otherwise it's scaled by the product of there lengths.
   */
  double dotProduct(const Vector3D &v) const;

  /**
   * Calculate the cross product of this vector and the provided vector.
   * @param v The provided vector.
   * @return The cross product of the 2 vectors.
   * @note The resultant vector is perpendicular to a plane formed by the two vectors.
   * @note The magnitude of the result is the area formed by the parallelogram of the two vectors.
   */
  Vector3D crossProduct(const Vector3D &v) const;

  /**
   * Calculate the reflection vector for a surface with the specified normal.
   * @param norm The surface normal of the surface to reflect from.
   * @return The reflection vector.
   */
  Vector3D reflection(Vector3D norm) const;

  /**
   * Calculate the refraction vector for a surface with the specified normal.
   * @param norm The surface normal of the surface to refract through.
   * @param fact The refraction factor.
   * @return The refraction vector.
   */
  Vector3D refraction(Vector3D norm, double fact) const;

  /**
   * Set the values of the vector.
   * @param nx The new value for X.
   * @param ny The new value for Y.
   * @param nz The new value for Z.
   */
  void set(double nx, double ny, double nz);
  /**
   * Set the values of the vector.
   * @param v The new value for the vector.
   */
  void set(const Vector3D &v);

  /**
   * Add the specified vector to this vector and return the result.
   * @param v The vector to add.
   * @return The result of the addition.
   * @note this object is not modified.
   */
  Vector3D addTo(const Vector3D &v) const;
  /**
   * Add the specified vector to this vector.
   * @param v The vector to add to this vector.
   */
  void add(const Vector3D &v);
  /**
   * Subtract the specified vector from this vector.
   * @param v The vector to subtract.
   * @return The result of the subtraction.
   * @note this object is not modified.
   */
  Vector3D subtractFrom(const Vector3D &v) const;
  /**
   * Subtract the specified vector from this vector.
   * @param v The vector to subtract from this vector.
   */
  void subtract(const Vector3D &v);
  /**
   * Multiply this vector by the specified vector.
   * @param v The vector to multiply by.
   * @return The result of the multiplication.
   * @note this object is not modified.
   */
  Vector3D multiplyBy(const Vector3D &v) const;
  /**
   * Multiply this vector by the specified vector.
   * @param v The vector to multiply by.
   */
  void multiply(const Vector3D &v);
  /**
   * Divide this vector by the specified vector.
   * @param v The vector to divide by.
   * @return The result of the division.
   * @note this object is not modified.
   */
  Vector3D divideBy(const Vector3D &v) const;
  /**
   * Divide this vector by the specified vector.
   * @param v The vector to divide by.
   */
  void divide(const Vector3D &v);
  /**
   * Multiply this vector by the specified constant.
   * @param k The constant to multiply by.
   * @return The result of the multiplication.
   * @note this object is not modified.
   */
  Vector3D multiplyBy(double k) const;
  /**
   * Multiply this vector by the specified constant.
   * @param k The constant to multiply by.
   */
  void multiply(double k);
  /**
   * Divide this vector by the specified constant.
   * @param k The constant to divide by.
   * @return The result of the division.
   * @note this object is not modified.
   */
  Vector3D divideBy(double k) const;
  /**
   * Divide this vector by the specified constant.
   * @param k The constant to divide by.
   */
  void divide(double k);
  /**
   * Multiply this vector by the specified matrix.
   * @param m The matrix to multiply by.
   * @return The result of the multiplication.
   * @note this object is not modified.
   */
  Vector3D multiplyBy(const Matrix &m) const;
  /**
   * Multiply this vector by the specified matrix.
   * @param m The matrix to multiply by.
   */
  void multiply(const Matrix &m);
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
  bool equals(const Vector3D &v) const {
    return v.x == x && v.y == y && v.z == z;
  }

};

}

#endif	/* VECTOR3D_H */

