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

#ifndef MATRIX_H
#define	MATRIX_H

namespace KayLib {

#include "KMath.h"

class Matrix {
public:
  double e[16];

  Matrix();
  Matrix(const Matrix &m);
  Matrix(const Quaternion &q);

  /**
   * Return a copy of the Matrix.
   * @return A copy of the matrix.
   */
  Matrix copy() const {
    return Matrix(*this);
  }

  /**
   * Set the matrix to all zero.
   */
  void zero();
  /**
   * Set the matrix to an identity matrix.
   */
  void identity();
  /**
   * Get an identity matrix.
   * @return A new identity matrix.
   */
  static Matrix createIdentity();

  /**
   * Get the transposed matrix.
   * @return The transposed matrix.
   */
  Matrix transpose() const;

  /**
   * Get the inverse matrix.
   * @return The inverse of the matrix.
   */
  Matrix inverse() const;

  /**
   * Get the determinant of the matrix.
   * @return The determinant.
   */
  double determinant() const;

  /**
   * Set the matrix to the same values as the specified matrix.
   * @param m The matrix to set the values to.
   */
  void set(const Matrix &m);

  /**
   * Construct a matrix representation of a Quaternion.
   * @param q The quaternion to represent.
   * @return The matrix representation.
   */
  static Matrix matrixFromQuaternion(const Quaternion &q);

  /**
   * Translate (move) the position of this matrix by the specified amount.
   * @param T The amount to move the matrix location by.
   */
  void translate(const Vector3D &T);
  /**
   * Scale the matrix by the specified amount.
   * @param S The amount to scale the matrix by.
   */
  void scale(const Vector3D &S);
  /**
   * Rotate the matrix by the specified amount.
   * @param R The amount to rotate by.
   * @note the rotation is conducted in the order Z, X, Y and is
   * the same as calling rotateZ followed by rotateX and rotateY.
   */
  void rotate(const Vector3D &R);
  /**
   * Rotate the matrix around the X axis by the specified amount.
   * @param X The amount to rotate in radians.
   * @note Uses right handed rotation.
   */
  void rotateX(double X);
  /**
   * Rotate the matrix around the Y axis by the specified amount.
   * @param Y The amount to rotate in radians.
   * @note Uses right handed rotation.
   */
  void rotateY(double Y);
  /**
   * Rotate the matrix around the Z axis by the specified amount.
   * @param Z The amount to rotate in radians.
   * @note Uses right handed rotation.
   */
  void rotateZ(double Z);
  /**
   * Rotate the matrix around the X axis by the specified amount.
   * @param X The amount to rotate in radians.
   * @note Uses left handed rotation.
   */
  void rotateX_LH(double X);
  /**
   * Rotate the matrix around the Y axis by the specified amount.
   * @param Y The amount to rotate in radians.
   * @note Uses left handed rotation.
   */
  void rotateY_LH(double Y);
  /**
   * Rotate the matrix around the Z axis by the specified amount.
   * @param Z The amount to rotate in radians.
   * @note Uses left handed rotation.
   */
  void rotateZ_LH(double Z);

  /**
   * Create a matrix based on the specified camera parameters
   * @param eye The location of the camera (eye).
   * @param lookAt The location to look at.
   * @param up The direction that is UP.
   * @return The new camera matrix.
   */
  static Matrix lookAt(const Vector3D &eye, const Vector3D &lookAt, const Vector3D &up);
  /**
   * Create a orthographic projection matrix.
   * @param left The left edge of the screen.
   * @param right The right edge of the screen.
   * @param top The top of the screen.
   * @param bottom The bottom of the screen.
   * @param near The closest edge of the screen.
   * @param far The farthest into the screen.
   * @return The orthographic projection matrix.
   */
  static Matrix orthographicProjection(double left, double right, double top, double bottom, double near, double far);
  /**
   * Create a perspective projection matrix.
   * @param fov The field of view angle in degrees.
   * @param aspect The aspect (width/height) ratio of the display.
   * @param near The nearest something can be to the screen.
   * @param far The farthest something can be from the screen.
   * @return The new perspective projection matrix.
   */
  static Matrix perspectiveProjection(double fov, double aspect, double near, double far);

  Matrix operator+(const Matrix &m) const;
  Matrix& operator+=(const Matrix &m);
  Matrix operator-(const Matrix &m) const;
  Matrix& operator-=(const Matrix &m);
  Matrix operator*(const Matrix &m) const;
  Matrix& operator*=(const Matrix &m);
  Matrix operator/(const Matrix &m) const;
  Matrix& operator/=(const Matrix &m);

  Vector3D operator*(const Vector3D &v) const;
  Vector4D operator*(const Vector4D &v) const;

  Matrix& operator=(const Matrix &m);
  Matrix& operator=(const Quaternion &q);

};
}
#endif	/* MATRIX_H */
