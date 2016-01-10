#include "KMath.h"

namespace KayLib {

Vector4D::Vector4D() {
  x = y = z = 0;
  w = 1;
}

Vector4D::Vector4D(double nx, double ny, double nz) {
  x = nx;
  y = ny;
  z = nz;
  w = 1;
}

Vector4D::Vector4D(double nx, double ny, double nz, double nw) {
  x = nx;
  y = ny;
  z = nz;
  w = nw;
}

Vector4D::Vector4D(const Vector3D &v) {
  x = v.x;
  y = v.y;
  z = v.z;
  w = 1;
}

Vector4D::Vector4D(const Vector4D &v) {
  x = v.x;
  y = v.y;
  z = v.z;
  w = v.w;
}

// Retrieves the length of the Vector4D

double Vector4D::magnitude() const {
  double m;

  //calculate the length of the Vector3D
  m = (x * x) + (y * y) + (z * z);
  m = sqrt(m);

  return m;
}

// calculate the magnitude in homogeneous space.

double Vector4D::magnitude_H() const {
  double m;

  //calculate the length of the Vector3D
  m = (x * x) + (y * y) + (z * z);
  if(w == 0) {
    m = sqrt(m);
  } else {
    m = sqrt(m / w);
  }

  return m;
}

// Scales the Vector4D so it has a length of 1

void Vector4D::normalize() {
  double m;

  w = 1;
  //calculate the length of the Vector3D
  m = (x * x) + (y * y) + (z * z);
  m = sqrt(m);

  // if the length is zero then the Vector3D is zero so return
  if(m == 0) {
    return;
  }

  //Scale the Vector4D to a unit length
  x /= m;
  y /= m;
  z /= m;
}

// calculate the normal in homogeneous space.

void Vector4D::normalize_H() {
  double m = magnitude_H();

  w = 1;
  // if the length is zero then the Vector3D is zero so return
  if(m == 0) {
    return;
  }

  //Scale the Vector4D to a unit length
  x /= m;
  y /= m;
  z /= m;
}

// calculate the dot product of this Vector4D and the Vector4D v
// if both Vector4Ds are unit Vector4Ds then the result is the cosine
// of the angle between them.  otherwise it's scaled by the product of there lengths.
// calculate dot product as 3d vector

double Vector4D::dotProduct(const Vector4D &v) const {
  return ((x * v.x) + (y * v.y) + (z * v.z));
}

// calculate dot product in homogeneous space.

double Vector4D::dot_H(const Vector4D &v) const {
  Vector4D a;
  Vector4D b;
  a = (*this) / w;
  b = v / v.w;
  return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z));
}

// retrieve vector perpendicular to the plane formed by *this and v
// magnatude of c = area of parallellogram formed by *this and v
// calculate cross product as 3d vector

Vector4D Vector4D::crossProduct(const Vector3D &v) const {
  double nx = y * v.z - z * v.y;
  double ny = z * v.x - x * v.z;
  double nz = x * v.y - y * v.x;
  return Vector4D(nx, ny, nz);
}

// calculate cross product in homogeneous space.

Vector4D Vector4D::cross_H(const Vector4D &v) const {
  Vector4D a;
  Vector4D b;
  a = (*this) / w;
  b = v / v.w;
  Vector4D c = Vector4D();
  c.x = a.y * b.z - a.z * b.y;
  c.y = a.z * b.x - a.x * b.z;
  c.z = a.x * b.y - a.y * b.x;
  c.w = 1.0;
  return c;
}

// calculate the reflection vector.

Vector4D Vector4D::reflection(Vector4D norm) const {
  Vector4D refl;
  refl = norm * (-2 * dotProduct(norm));
  refl = (*this) - refl;
  return refl;
}

// calculate the refraction vector.

Vector4D Vector4D::refraction(Vector4D norm, double fact) const {
  Vector4D refr = Vector4D();
  double n_r = this->dotProduct(norm);
  double k = 1 - (n_r * n_r);
  k = 1 - (fact * fact) * k;
  if(k < 0) {
    return refr;
  }
  refr = (*this) * fact;
  refr -= norm * (fact * n_r + sqrt(k));
  return refr;
}

// Set the values of the Vector4D

void Vector4D::set(double nx, double ny, double nz) {
  x = nx;
  y = ny;
  z = nz;
  w = 1;
}

void Vector4D::set(double nx, double ny, double nz, double nw) {
  x = nx;
  y = ny;
  z = nz;
  w = nw;
}

void Vector4D::set(const Vector3D &v) {
  x = v.x;
  y = v.y;
  z = v.z;
  w = 1;
}

void Vector4D::set(const Vector4D &v) {
  x = v.x;
  y = v.y;
  z = v.z;
  w = v.w;
}

void Vector4D::set(Quaternion q) {
  double Scale = (q.x * q.x) + (q.y * q.y) + (q.z * q.z);
  w = 1;
  if(Scale == 0) {
    x = y = 0;
    z = 1;
    return;
  }

  x = q.x / Scale;
  y = q.y / Scale;
  z = q.z / Scale;
}

Vector4D& Vector4D::operator=(const Vector4D &v) {
  x = v.x;
  y = v.y;
  z = v.z;
  w = v.w;
  return *this;
}

Vector4D Vector4D::operator+(const Vector4D &v) const {
  return Vector4D(*this) += v;
}

Vector4D& Vector4D::operator+=(const Vector4D &v) {
  x += v.x;
  y += v.y;
  z += v.z;
  w += v.w;
  return *this;
}

Vector4D Vector4D::operator-(const Vector4D &v) const {
  return Vector4D(*this) -= v;
}

Vector4D& Vector4D::operator-=(const Vector4D &v) {
  x -= v.x;
  y -= v.y;
  z -= v.z;
  w -= v.w;
  return *this;
}

Vector4D Vector4D::operator*(const Vector4D &v) const {
  return Vector4D(*this) *= v;
}

Vector4D& Vector4D::operator*=(const Vector4D &v) {
  x *= v.x;
  y *= v.y;
  z *= v.z;
  w *= v.w;
  return *this;
}

Vector4D Vector4D::operator/(const Vector4D &v) const {
  return Vector4D(*this) /= v;
}

Vector4D& Vector4D::operator/=(const Vector4D &v) {
  x /= v.x;
  y /= v.y;
  z /= v.z;
  w /= v.w;
  return *this;
}

Vector4D Vector4D::operator*(const double &k) const {
  return Vector4D(*this) *= k;
}

Vector4D& Vector4D::operator*=(const double &k) {
  x *= k;
  y *= k;
  z *= k;
  w *= k;
  return *this;
}

Vector4D Vector4D::operator/(const double &k) const {
  return Vector4D(*this) /= k;
}

Vector4D& Vector4D::operator/=(const double &k) {
  x /= k;
  y /= k;
  z /= k;
  w /= k;
  return *this;
}

Vector4D Vector4D::operator*(const Matrix &m) const {
  return Vector4D(*this) *= m;
}

Vector4D& Vector4D::operator*=(const Matrix &m) {
  double nx = ((x * m.e[0]) + (y * m.e[4]) + (z * m.e[8]) + (m.e[12]));
  double ny = ((x * m.e[1]) + (y * m.e[5]) + (z * m.e[9]) + (m.e[13]));
  double nz = ((x * m.e[2]) + (y * m.e[6]) + (z * m.e[10]) + (m.e[14]));
  double nw = (x * m.e[3]) + (y * m.e[7]) + (z * m.e[11]) + (w * m.e[15]);
  x = nx;
  y = ny;
  z = nz;
  w = nw;
  return *this;
}

Vector4D Vector4D::operator*(const Quaternion &q) const {
  Matrix m = Matrix::matrixFromQuaternion(q);
  Vector4D nv = (*this) * m;
  return nv;
}

Vector4D& Vector4D::operator*=(const Quaternion &q) {
  Matrix m = Matrix::matrixFromQuaternion(q);
  (*this) *= m;
  return *this;
}

}