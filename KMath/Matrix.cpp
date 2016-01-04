#include "KMath.h"

namespace KayLib {

//  FloatBuffer getFloatBuffer() {
//    float[] vals = float[16];
//    for (int i = 0; i < 16; i++) {
//      vals[i] = (float) e[i];
//    }
//    FloatBuffer fb = Buffers.newDirectFloatBuffer(vals);
//    return fb;
//  }

Matrix::Matrix() {
  for(int i = 0; i < 16; i++) {
    e[i] = 0;
  }
}

Matrix::Matrix(const Matrix &m) {
  for(int i = 0; i < 16; i++) {
    e[i] = m.e[i];
  }
}

void Matrix::zero() {
  for(int i = 0; i < 16; i++) {
    e[i] = 0;
  }
}

void Matrix::identity() {
  for(int i = 0; i < 16; i++) {
    e[i] = 0;
  }
  e[0] = e[5] = e[10] = e[15] = 1.0;
}

Matrix Matrix::createIdentity() {
  Matrix i = Matrix();
  i.e[0] = i.e[5] = i.e[10] = i.e[15] = 1.0;
  return i;
}

Matrix Matrix::transpose() {
  Matrix tm = Matrix();
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      tm.e[(i * 4) + j] = e[(j * 4) + i];
    }
  }
  return tm;
}

Matrix Matrix::inverse() {
  Matrix tm = Matrix();

  double x = determinant();
  if(x == 0) {
    return tm;
  }

  tm.e[0] = (-e[13] * e[10] * e[7] + e[9] * e[14] * e[7] + e[13] * e[6] * e[11]
          - e[5] * e[14] * e[11] - e[9] * e[6] * e[15] + e[5] * e[10] * e[15]) / x;
  tm.e[4] = (e[12] * e[10] * e[7] - e[8] * e[14] * e[7] - e[12] * e[6] * e[11]
          + e[4] * e[14] * e[11] + e[8] * e[6] * e[15] - e[4] * e[10] * e[15]) / x;
  tm.e[8] = (-e[12] * e[9] * e[7] + e[8] * e[13] * e[7] + e[12] * e[5] * e[11]
          - e[4] * e[13] * e[11] - e[8] * e[5] * e[15] + e[4] * e[9] * e[15]) / x;
  tm.e[12] = (e[12] * e[9] * e[6] - e[8] * e[13] * e[6] - e[12] * e[5] * e[10]
          + e[4] * e[13] * e[10] + e[8] * e[5] * e[14] - e[4] * e[9] * e[14]) / x;
  tm.e[1] = (e[13] * e[10] * e[3] - e[9] * e[14] * e[3] - e[13] * e[2] * e[11]
          + e[1] * e[14] * e[11] + e[9] * e[2] * e[15] - e[1] * e[10] * e[15]) / x;
  tm.e[5] = (-e[12] * e[10] * e[3] + e[8] * e[14] * e[3] + e[12] * e[2] * e[11]
          - e[0] * e[14] * e[11] - e[8] * e[2] * e[15] + e[0] * e[10] * e[15]) / x;
  tm.e[9] = (e[12] * e[9] * e[3] - e[8] * e[13] * e[3] - e[12] * e[1] * e[11]
          + e[0] * e[13] * e[11] + e[8] * e[1] * e[15] - e[0] * e[9] * e[15]) / x;
  tm.e[13] = (-e[12] * e[9] * e[2] + e[8] * e[13] * e[2] + e[12] * e[1] * e[10]
          - e[0] * e[13] * e[10] - e[8] * e[1] * e[14] + e[0] * e[9] * e[14]) / x;
  tm.e[2] = (-e[13] * e[6] * e[3] + e[5] * e[14] * e[3] + e[13] * e[2] * e[7]
          - e[1] * e[14] * e[7] - e[5] * e[2] * e[15] + e[1] * e[6] * e[15]) / x;
  tm.e[6] = (e[12] * e[6] * e[3] - e[4] * e[14] * e[3] - e[12] * e[2] * e[7]
          + e[0] * e[14] * e[7] + e[4] * e[2] * e[15] - e[0] * e[6] * e[15]) / x;
  tm.e[10] = (-e[12] * e[5] * e[3] + e[4] * e[13] * e[3] + e[12] * e[1] * e[7]
          - e[0] * e[13] * e[7] - e[4] * e[1] * e[15] + e[0] * e[5] * e[15]) / x;
  tm.e[14] = (e[12] * e[5] * e[2] - e[4] * e[13] * e[2] - e[12] * e[1] * e[6]
          + e[0] * e[13] * e[6] + e[4] * e[1] * e[14] - e[0] * e[5] * e[14]) / x;
  tm.e[3] = (e[9] * e[6] * e[3] - e[5] * e[10] * e[3] - e[9] * e[2] * e[7]
          + e[1] * e[10] * e[7] + e[5] * e[2] * e[11] - e[1] * e[6] * e[11]) / x;
  tm.e[7] = (-e[8] * e[6] * e[3] + e[4] * e[10] * e[3] + e[8] * e[2] * e[7]
          - e[0] * e[10] * e[7] - e[4] * e[2] * e[11] + e[0] * e[6] * e[11]) / x;
  tm.e[11] = (e[8] * e[5] * e[3] - e[4] * e[9] * e[3] - e[8] * e[1] * e[7]
          + e[0] * e[9] * e[7] + e[4] * e[1] * e[11] - e[0] * e[5] * e[11]) / x;
  tm.e[15] = (-e[8] * e[5] * e[2] + e[4] * e[9] * e[2] + e[8] * e[1] * e[6]
          - e[0] * e[9] * e[6] - e[4] * e[1] * e[10] + e[0] * e[5] * e[10]) / x;

  return tm;
}

double Matrix::determinant() const {
  return e[12] * e[9] * e[6] * e[3]
          - e[8] * e[13] * e[6] * e[3]
          - e[12] * e[5] * e[10] * e[3]
          + e[4] * e[13] * e[10] * e[3]
          + e[8] * e[5] * e[14] * e[3]
          - e[4] * e[9] * e[14] * e[3]
          - e[12] * e[9] * e[2] * e[7]
          + e[8] * e[13] * e[2] * e[7]
          + e[12] * e[1] * e[10] * e[7]
          - e[0] * e[13] * e[10] * e[7]
          - e[8] * e[1] * e[14] * e[7]
          + e[0] * e[9] * e[14] * e[7]
          + e[12] * e[5] * e[2] * e[11]
          - e[4] * e[13] * e[2] * e[11]
          - e[12] * e[1] * e[6] * e[11]
          + e[0] * e[13] * e[6] * e[11]
          + e[4] * e[1] * e[14] * e[11]
          - e[0] * e[5] * e[14] * e[11]
          - e[8] * e[5] * e[2] * e[15]
          + e[4] * e[9] * e[2] * e[15]
          + e[8] * e[1] * e[6] * e[15]
          - e[0] * e[9] * e[6] * e[15]
          - e[4] * e[1] * e[10] * e[15]
          + e[0] * e[5] * e[10] * e[15];
}

void Matrix::set(const Matrix &m) {
  for(int i = 0; i < 16; i++) {
    e[i] = m.e[i];
  }
}

Matrix Matrix::addTo(const Matrix &m) const {
  Matrix nm = Matrix();
  for(int i = 0; i < 16; i++) {
    nm.e[i] = e[i] + m.e[i];
  }
  return nm;
}

void Matrix::add(const Matrix &m) {
  for(int i = 0; i < 16; i++) {
    e[i] += m.e[i];
  }
}

Matrix Matrix::subtractFrom(const Matrix &m) const {
  Matrix nm = Matrix();
  for(int i = 0; i < 16; i++) {
    nm.e[i] = e[i] - m.e[i];
  }
  return nm;
}

void Matrix::subtract(const Matrix &m) {
  for(int i = 0; i < 16; i++) {
    e[i] -= m.e[i];
  }
}

Matrix Matrix::multiplyBy(const Matrix &m) const {
  Matrix nm = Matrix();
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      for(int a = 0; a < 4; a++) {
        nm.e[i + (j * 4)] += e[a + (j * 4)] * m.e[i + (a * 4)];
      }
    }
  }
  return nm;
}

void Matrix::multiply(const Matrix &m) {
  Matrix nm = Matrix();
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      for(int a = 0; a < 4; a++) {
        nm.e[i + (j * 4)] += e[a + (j * 4)] * m.e[i + (a * 4)];
      }
    }
  }
  this->set(nm);
}

Matrix Matrix::divideBy(const Matrix &m) const {
  Matrix nm = Matrix();
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      for(int a = 0; a < 4; a++) {
        nm.e[i + (j * 4)] -= e[a + (j * 4)] * m.e[i + (a * 4)];
      }
    }
  }
  return nm;
}

void Matrix::divide(const Matrix &m) {
  Matrix nm = Matrix();
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      for(int a = 0; a < 4; a++) {
        nm.e[i + (j * 4)] -= e[a + (j * 4)] * m.e[i + (a * 4)];
      }
    }
  }
  this->set(nm);
}

Matrix Matrix::matrixFromQuaternion(const Quaternion &q) {
  // set to identity matrix
  Matrix m = createIdentity();

  // coefficients
  double wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;
  // calculate coefficients
  x2 = q.x + q.x; // x * 2
  y2 = q.y + q.y; // y * 2
  z2 = q.z + q.z; // z * 2
  xx = q.x * x2; // 2x^2
  xy = q.x * y2; // 2xy
  xz = q.x * z2; // 2xz
  yy = q.y * y2; // 2y^2
  yz = q.y * z2; // 2yz
  zz = q.z * z2; // 2z^2
  wx = q.w * x2; // 2w^2
  wy = q.w * y2; // 2wy
  wz = q.w * z2; // 2wz

  // 1- 2y^2 - 2z^2
  m.e[0] = 1.0f - (yy + zz);
  // 2xy - 2wz
  m.e[4] = xy - wz;
  // 2xz + 2wy
  m.e[8] = xz + wy;

  // 2xy + 2wz
  m.e[1] = xy + wz;
  // 1- 2x^2 + 2z^2
  m.e[5] = 1.0f - (xx + zz);
  // 2yz - 2wx
  m.e[9] = yz - wx;

  // 2xz - 2wy
  m.e[2] = xz - wy;
  // 2yz + 2wx
  m.e[6] = yz + wx;
  // 1 - 2x^2 + 2y^2
  m.e[10] = 1.0f - (xx + yy);

  return m;
}

void Matrix::translate(const Vector3D &T) {
  Vector3D t;
  t = T.multiplyBy(*this);
  e[12] += t.x;
  e[13] += t.y;
  e[14] += t.z;
}

void Matrix::scale(const Vector3D &S) {
  Matrix Sc = Matrix();
  Sc.identity();
  Sc.e[0] *= S.x;
  Sc.e[5] *= S.y;
  Sc.e[10] *= S.z;
  set(Sc.multiplyBy(*this));
}

void Matrix::rotate(const Vector3D &R) {
  this->rotateZ(R.z);
  this->rotateX(R.x);
  this->rotateY(R.y);
}

void Matrix::rotateX(double X) {
  double cs = cos(X);
  double si = sin(X);
  Matrix R = createIdentity();
  R.e[5] = cs;
  R.e[6] = si;
  R.e[9] = -si;
  R.e[10] = cs;
  set(R.multiplyBy(*this));
}

void Matrix::rotateY(double Y) {
  double cs = cos(Y);
  double si = sin(Y);
  Matrix R = createIdentity();
  R.e[0] = cs;
  R.e[2] = -si;
  R.e[8] = si;
  R.e[10] = cs;
  set(R.multiplyBy(*this));
}

void Matrix::rotateZ(double Z) {
  double cs = cos(Z);
  double si = sin(Z);
  Matrix R = createIdentity();
  R.e[0] = cs;
  R.e[1] = si;
  R.e[4] = -si;
  R.e[5] = cs;
  set(R.multiplyBy(*this));
}

void Matrix::rotateX_LH(double X) {
  double cs = cos(X);
  double si = sin(X);
  Matrix R = createIdentity();
  R.e[5] = cs;
  R.e[6] = -si;
  R.e[9] = si;
  R.e[10] = cs;
  set(R.multiplyBy(*this));
}

void Matrix::rotateY_LH(double Y) {
  double cs = cos(Y);
  double si = sin(Y);
  Matrix R = createIdentity();
  R.e[0] = cs;
  R.e[2] = si;
  R.e[8] = -si;
  R.e[10] = cs;
  set(R.multiplyBy(*this));
}

void Matrix::rotateZ_LH(double Z) {
  double cs = cos(Z);
  double si = sin(Z);
  Matrix R = createIdentity();
  R.e[0] = cs;
  R.e[1] = -si;
  R.e[4] = si;
  R.e[5] = cs;
  set(R.multiplyBy(*this));
}

Matrix Matrix::lookAt(const Vector3D &eye, const Vector3D &lookAt, const Vector3D &up) {
  // based on http://www.opengl.org/sdk/docs/man2/xhtml/gluLookAt.xml
  Vector3D U = up;
  Vector3D F(lookAt);
  F.subtract(eye);
  // normalize.
  F.normalize();
  U.normalize();
  Vector3D S = F.crossProduct(U);
  S.normalize();
  U = S.crossProduct(F);
  Matrix m;
  m.identity();
  m.e[0] = S.x;
  m.e[4] = S.y;
  m.e[8] = S.z;
  m.e[1] = U.x;
  m.e[5] = U.y;
  m.e[9] = U.z;
  m.e[2] = -F.x;
  m.e[6] = -F.y;
  m.e[10] = -F.z;
  Vector3D ee(eye);
  ee.multiply(-1);
  m.translate(ee);
  return m;
}

Matrix Matrix::orthographicProjection(double left, double right, double top, double bottom, double near, double far) {
  Matrix m;
  m.zero();
  m.e[15] = 1;
  double rl = right - left;
  double tb = top - bottom;
  double fn = far - near;
  m.e[0] = 2 / rl;
  m.e[5] = 2 / tb;
  m.e[10] = -2 / fn;
  m.e[12] = -((right + left) / rl);
  m.e[13] = -((top + bottom) / tb);
  m.e[14] = -((far + near) / fn);
  return m;
}

Matrix Matrix::perspectiveProjection(double fov, double aspect, double near, double far) {
  Matrix m;
  m.zero();
  double fovR = degreesToRadians(fov);
  double f = 1;
  // protect against div errors.
  if(fov > 0 && fov < 90) {
    f = 1 / tan(fovR / 2);
  }
  m.e[11] = -1;
  m.e[0] = f / aspect;
  m.e[5] = f;
  double nf = near - far;
  m.e[10] = (far + near) / nf;
  m.e[14] = (2 * far * near) / nf;
}

Vector3D Matrix::multiplyBy(const Vector3D &v) {
  Vector3D p = Vector3D();
  p.x = e[0] * v.x + e[4] * v.y + e[8] * v.z + e[12];
  p.y = e[1] * v.x + e[5] * v.y + e[9] * v.z + e[13];
  p.z = e[2] * v.x + e[6] * v.y + e[10] * v.z + e[14];
  return p;
}

Vector4D Matrix::multiplyBy(const Vector4D &v) {
  Vector4D p = Vector4D();
  p.x = e[0] * v.x + e[4] * v.y + e[8] * v.z + e[12] * v.w;
  p.y = e[1] * v.x + e[5] * v.y + e[9] * v.z + e[13] * v.w;
  p.z = e[2] * v.x + e[6] * v.y + e[10] * v.z + e[14] * v.w;
  p.w = e[3] * v.x + e[7] * v.y + e[11] * v.z + e[15] * v.w;
  return p;
}

}

