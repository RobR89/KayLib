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
  refl = norm.multiplyBy(-2 * dotProduct(norm));
  refl = this->subtractFrom(refl);
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
  refr = multiplyBy(fact);
  refr.subtract(norm.multiplyBy(fact * n_r + sqrt(k)));
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

Vector3D Vector3D::addTo(const Vector3D &v) const {
  double nx = x + v.x;
  double ny = y + v.y;
  double nz = z + v.z;
  return Vector3D(nx, ny, nz);
}

void Vector3D::add(const Vector3D &v) {
  x += v.x;
  y += v.y;
  z += v.z;
}

Vector3D Vector3D::subtractFrom(const Vector3D &v) const {
  double nx = x - v.x;
  double ny = y - v.y;
  double nz = z - v.z;
  return Vector3D(nx, ny, nz);
}

void Vector3D::subtract(const Vector3D &v) {
  x -= v.x;
  y -= v.y;
  z -= v.z;
}

Vector3D Vector3D::multiplyBy(const Vector3D &v) const {
  double nx = x * v.x;
  double ny = y * v.y;
  double nz = z * v.z;
  return Vector3D(nx, ny, nz);
}

void Vector3D::multiply(const Vector3D &v) {
  x *= v.x;
  y *= v.y;
  z *= v.z;
}

Vector3D Vector3D::divideBy(const Vector3D &v) const {
  double nx = 0;
  double ny = 0;
  double nz = 0;
  if(v.x != 0) {
    nx = x / v.x;
  }
  if(v.y != 0) {
    ny = y / v.y;
  }
  if(v.z != 0) {
    nz = z / v.z;
  }
  return Vector3D(nx, ny, nz);
}

void Vector3D::divide(const Vector3D &v) {
  if(v.x != 0) {
    x /= v.x;
  }
  if(v.y != 0) {
    y /= v.y;
  }
  if(v.z != 0) {
    z /= v.z;
  }
}

Vector3D Vector3D::multiplyBy(double k) const {
  double nx = x * k;
  double ny = y * k;
  double nz = z * k;
  return Vector3D(nx, ny, nz);
}

void Vector3D::multiply(double k) {
  x *= k;
  y *= k;
  z *= k;
}

Vector3D Vector3D::divideBy(double k) const {
  if(k == 0) {
    return Vector3D();
  }
  double nx = x / k;
  double ny = y / k;
  double nz = z / k;
  return Vector3D(nx, ny, nz);
}

void Vector3D::divide(double k) {
  if(k == 0) {
    return;
  }
  x /= k;
  y /= k;
  z /= k;
}

Vector3D Vector3D::multiplyBy(const Matrix &m) const {
  double nx = ((x * m.e[0]) + (y * m.e[4]) + (z * m.e[8]) + (m.e[12]));
  double ny = ((x * m.e[1]) + (y * m.e[5]) + (z * m.e[9]) + (m.e[13]));
  double nz = ((x * m.e[2]) + (y * m.e[6]) + (z * m.e[10]) + (m.e[14]));
  return Vector3D(nx, ny, nz);
}

void Vector3D::multiply(const Matrix &m) {
  double nx, ny, nz;
  nx = ((x * m.e[0]) + (y * m.e[4]) + (z * m.e[8]) + (m.e[12]));
  ny = ((x * m.e[1]) + (y * m.e[5]) + (z * m.e[9]) + (m.e[13]));
  nz = ((x * m.e[2]) + (y * m.e[6]) + (z * m.e[10]) + (m.e[14]));
  x = nx;
  y = ny;
  z = nz;
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
}
