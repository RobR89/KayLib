#include "KMath.h"

namespace KayLib {

Vector3D::Vector3D(const Vector4D &v) {
  double w = 1;
  if(v.w != 0) {
    w = v.w;
  }
  x = v.x / w;
  y = v.y / w;
  z = v.z / w;
}

double Vector3D::magnitude() const {
  double m;

  //calculate the length of the Vector3D
  m = (x * x) + (y * y) + (z * z);
  m = sqrt(m);

  return m;
}

void Vector3D::normalize() {
  double m;

  //calculate the length of the Vector3D
  m = (x * x) + (y * y) + (z * z);
  m = sqrt(m);

  // if the length is zero then the Vector3D is zero so return
  if(m == 0) {
    return;
  }

  //Scale the Vector3D to a unit length
  x /= m;
  y /= m;
  z /= m;
}

double Vector3D::dotProduct(const Vector3D &v) const {
  return ((x * v.x) + (y * v.y) + (z * v.z));
}

Vector3D Vector3D::crossProduct(const Vector3D &v) const {
  double nx = y * v.z - z * v.y;
  double ny = z * v.x - x * v.z;
  double nz = x * v.y - y * v.x;
  return Vector3D(nx, ny, nz);
}

Vector3D Vector3D::reflection(Vector3D norm) const {
  Vector3D refl;
  refl = norm * (-2 * dotProduct(norm));
  refl = (*this) - refl;
  return refl;
}

Vector3D Vector3D::refraction(Vector3D norm, double fact) const {
  Vector3D refr;
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

void Vector3D::set(double nx, double ny, double nz) {
  x = nx;
  y = ny;
  z = nz;
}

void Vector3D::set(const Vector3D &v) {
  x = v.x;
  y = v.y;
  z = v.z;
}

void Vector3D::set(Quaternion q) {
  double Scale = ((q.x * q.x) + (q.y * q.y) + (q.z * q.z));
  if(Scale == 0) {
    x = y = 0;
    z = 1;
    return;
  }

  x = q.x / Scale;
  y = q.y / Scale;
  z = q.z / Scale;
}

Vector3D& Vector3D::operator=(const Vector3D &v) {
  x = v.x;
  y = v.y;
  z = v.z;
  return *this;
}

Vector3D Vector3D::operator+(const Vector3D &v) const {
  return Vector3D(*this) += v;
}

Vector3D& Vector3D::operator+=(const Vector3D &v) {
  x += v.x;
  y += v.y;
  z += v.z;
  return *this;
}

Vector3D Vector3D::operator-(const Vector3D &v) const {
  return Vector3D(*this) -= v;
}

Vector3D& Vector3D::operator-=(const Vector3D &v) {
  x -= v.x;
  y -= v.y;
  z -= v.z;
  return *this;
}

Vector3D Vector3D::operator*(const Vector3D &v) const {
  return Vector3D(*this) *= v;
}

Vector3D& Vector3D::operator*=(const Vector3D &v) {
  x *= v.x;
  y *= v.y;
  z *= v.z;
  return *this;
}

Vector3D Vector3D::operator/(const Vector3D &v) const {
  return Vector3D(*this) /= v;
}

Vector3D& Vector3D::operator/=(const Vector3D &v) {
  x /= v.x;
  y /= v.y;
  z /= v.z;
  return *this;
}

Vector3D Vector3D::operator*(const double &k) const {
  return Vector3D(*this) *= k;
}

Vector3D& Vector3D::operator*=(const double &k) {
  x *= k;
  y *= k;
  z *= k;
  return *this;
}

Vector3D Vector3D::operator/(const double &k) const {
  return Vector3D(*this) /= k;
}

Vector3D& Vector3D::operator/=(const double &k) {
  x /= k;
  y /= k;
  z /= k;
  return *this;
}

Vector3D Vector3D::operator*(const Matrix &m) const {
  return Vector3D(*this) *= m;
}

Vector3D& Vector3D::operator*=(const Matrix &m) {
  double nx, ny, nz;
  nx = ((x * m.e[0]) + (y * m.e[4]) + (z * m.e[8]) + (m.e[12]));
  ny = ((x * m.e[1]) + (y * m.e[5]) + (z * m.e[9]) + (m.e[13]));
  nz = ((x * m.e[2]) + (y * m.e[6]) + (z * m.e[10]) + (m.e[14]));
  x = nx;
  y = ny;
  z = nz;
  return *this;
}

}
