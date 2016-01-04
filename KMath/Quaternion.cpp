#include "KMath.h"
#include <cstdlib>

namespace KayLib {

Quaternion::Quaternion() {
  w = x = y = z = 0;
}

Quaternion::Quaternion(const Quaternion &q) {
  w = q.w;
  x = q.x;
  y = q.y;
  z = q.z;
}

void Quaternion::eulerToQuat(double ax, double ay, double az) {
  Quaternion Q1, Q2, Q3;
  Vector3D v = Vector3D(1, 0, 0);
  Q1 = quaternionFromAxisAngle(ax, v);
  v.set(0, 1, 0);
  Q2 = quaternionFromAxisAngle(ay, v);
  v.set(0, 0, 1);
  Q3 = quaternionFromAxisAngle(az, v);
  set(Q3.multiplyBy(Q2).multiplyBy(Q1));
}

void Quaternion::eulerToQuat_deg(double ax, double ay, double az) {
  double rx = degreesToRadians(ax);
  double ry = degreesToRadians(ay);
  double rz = degreesToRadians(az);
  eulerToQuat(rx, ry, rz);
}

void Quaternion::eulerToQuat(Vector3D a) {
  eulerToQuat(a.x, a.y, a.z);
}

void Quaternion::eulerToQuat_deg(Vector3D a) {
  eulerToQuat_deg(a.x, a.y, a.z);
}

void Quaternion::axisAngleToQuat(AxisAngle aa) {
  axisAngleToQuat(aa.angle, aa.axis);
}

void Quaternion::axisAngleToQuat(double angle, Vector3D axis) {
  w = cos(angle / 2);
  double sa = sin(angle / 2);
  // adjust to normalize axis
  sa /= axis.magnitude();
  x = axis.x * sa;
  y = axis.y * sa;
  z = axis.z * sa;
  normalize();
}

Quaternion Quaternion::quaternionFromAxisAngle(AxisAngle aa) {
  Quaternion q;
  q.axisAngleToQuat(aa);
  return q;
}

Quaternion Quaternion::quaternionFromAxisAngle(double angle, Vector3D axis) {
  Quaternion q;
  q.axisAngleToQuat(angle, axis);
  return q;
}

AxisAngle Quaternion::quatToAxisAngle() {
  AxisAngle aa;
  normalize();
  double ca = w;
  aa.angle = acos(ca) * 2;
  double sa = sqrt(1.0f - ca * ca);
  if(abs(sa) < 0.00000001) {
    sa = 1;
  }
  aa.axis = Vector3D(x / sa, y / sa, z / sa);
  aa.axis.normalize();
  return aa;
}

Vector3D Quaternion::getEulerAngles() {
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
  if(tmp > 0.999999) {
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

void Quaternion::latLonHeadToQuat(double Latitude, double Longitude, double Heading) {
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

LatLonHead Quaternion::quatToLatLon() {
  LatLonHead llh;
  normalize();
  double lat, lon;

  double ca = w;
  double sa = sqrt(1.0 - ca * ca);
  llh.Heading = acos(ca) * 2;

  if(abs(sa) < 0.0000001) {
    sa = 1;
  }

  double tx = x / sa;
  double ty = y / sa;
  double tz = z / sa;

  lat = -asin(ty);
  if(tx * tx + tz * tz < 0.0000001) {
    lon = 0;
  } else {
    lon = atan2(tx, tz);
  }
  if(lon < 0) {
    lon += 360.0;
  }

  llh.Latitude = lat;
  llh.Longatude = lon;
  return llh;
}

void Quaternion::set(const Quaternion &q) {
  w = q.w;
  x = q.x;
  y = q.y;
  z = q.z;
}

Quaternion Quaternion::addTo(const Quaternion &q) {
  Quaternion nq = Quaternion();
  nq.w = w + q.w;
  nq.x = x + q.x;
  nq.y = y + q.y;
  nq.z = z + q.z;
  return nq;
}

void Quaternion::add(const Quaternion &q) {
  w += q.w;
  x += q.x;
  y += q.y;
  z += q.z;
}

Quaternion Quaternion::subtractFrom(const Quaternion &q) {
  Quaternion nq = Quaternion();
  nq.w = w - q.w;
  nq.x = x - q.x;
  nq.y = y - q.y;
  nq.z = z - q.z;
  return nq;
}

void Quaternion::subtract(const Quaternion &q) {
  w -= q.w;
  x -= q.x;
  y -= q.y;
  z -= q.z;
}

Quaternion Quaternion::multiplyBy(const Quaternion &q) {
  Quaternion nq = Quaternion();

  nq.w = w * q.w - x * q.x - y * q.y - z * q.z;
  nq.x = w * q.x + x * q.w + y * q.z - z * q.y;
  nq.y = w * q.y + y * q.w + z * q.x - x * q.z;
  nq.z = w * q.z + z * q.w + x * q.y - y * q.x;

  return nq;
}

void Quaternion::multiply(const Quaternion &q) {
  Quaternion nq = Quaternion();

  nq.w = w * q.w - x * q.x - y * q.y - z * q.z;
  nq.x = w * q.x + x * q.w + y * q.z - z * q.y;
  nq.y = w * q.y + y * q.w + z * q.x - x * q.z;
  nq.z = w * q.z + z * q.w + x * q.y - y * q.x;

  w = nq.w;
  x = nq.x;
  y = nq.y;
  z = nq.z;
}

Quaternion Quaternion::divideBy(const Quaternion &q) {
  Quaternion a = Quaternion(*this);
  a.inverse();
  return a.multiplyBy(q);
}

void Quaternion::divide(const Quaternion &q) {
  inverse();
  multiply(q);
}

Quaternion Quaternion::conjugate() {
  Quaternion nq = Quaternion();
  nq.w = w;
  nq.x = -x;
  nq.y = -y;
  nq.z = -z;
  return nq;
}

Vector3D Quaternion::orient(const Vector3D &v) {
  Vector3D nv = Vector3D(v);
  Matrix m = Matrix::matrixFromQuaternion(*this);
  nv.multiply(m);
  return nv;
}

Vector4D Quaternion::orient(const Vector4D &v) {
  Vector4D nv = Vector4D(v);
  Matrix m = Matrix::matrixFromQuaternion(*this);
  nv.multiply(m);
  return nv;
}

Quaternion Quaternion::quaternionFromMatrix(const Matrix &m) {
  double tr, s;
  Quaternion q = Quaternion();

  tr = 1 + m.e[0] + m.e[5] + m.e[10];

  // check the diagonal
  if(tr > 0.00000001) {
    s = 0.5 / sqrt(tr);
    q.x = (m.e[9] - m.e[6]) * s;
    q.y = (m.e[2] - m.e[8]) * s;
    q.z = (m.e[4] - m.e[1]) * s;
    q.w = 0.25 / s;
  } else {
    // diagonal is negative
    if(m.e[0] > m.e[5] && m.e[0] > m.e[10]) {
      s = 2 * sqrt(1.0f + m.e[0] - m.e[5] - m.e[10]);
      q.x = 0.5 / s;
      q.y = (m.e[1] + m.e[4]) / s;
      q.z = (m.e[2] + m.e[8]) / s;
      q.w = (m.e[6] - m.e[9]) / s;
    } else if(m.e[5] > m.e[10]) {
      s = 2 * sqrt(1.0f + m.e[5] - m.e[0] - m.e[10]);
      q.x = (m.e[1] + m.e[4]) / s;
      q.y = 0.5 / s;
      q.z = (m.e[6] + m.e[9]) / s;
      q.w = (m.e[2] + m.e[8]) / s;
    } else {
      s = 2 * sqrt(1.0f + m.e[10] - m.e[0] - m.e[5]);
      q.x = (m.e[2] + m.e[8]) / s;
      q.y = (m.e[6] + m.e[9]) / s;
      q.z = 0.5 / s;
      q.w = (m.e[1] + m.e[4]) / s;
    }
  }
  // normalize needed to prevent errors in some cases
  q.normalize();
  return q;
}

void Quaternion::inverse() {
  double d = w * w + x * x + y * y + z * z;
  w /= d;
  x = (-x) / d;
  y = (-y) / d;
  z = (-z) / d;
}

double Quaternion::magnitude() {
  return sqrt(w * w + x * x + y * y + z * z);
}

void Quaternion::normalize() {
  double m = magnitude();
  x /= m;
  y /= m;
  z /= m;
  w /= m;
}

}
