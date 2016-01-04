#include "KMath.h"

namespace KayLib {

struct ControlPoints {
  int cp1;
  int cp2;
  int cp3;
  int cp4;
  int ret = -1;
};

void getSplinePoints(ControlPoints &con, double p, int pLen, bool loop) {
  if(loop) {
    // looping the spline so normalize the interpolation point to the spline range.
    while(p < 0) {
      p += pLen;
    }
    while(p >= pLen) {
      p -= pLen;
    }
  } else {
    // not looping, return end point if out of bounds.
    if(p < 0) {
      con.ret = 0;
      return;
    }
    if(p > pLen - 1) {
      con.ret = pLen - 1;
      return;
    }
  }
  con.cp1 = ((int)p) - 1;
  con.cp2 = ((int)p);
  con.cp3 = ((int)p) + 1;
  con.cp4 = ((int)p) + 2;
  if(loop) {
    // Looping, adjust interpolation points if they loop.
    while(con.cp1 < 0) {
      con.cp1 += pLen;
    }
    while(con.cp3 > pLen - 1) {
      con.cp3 -= pLen;
    }
    while(con.cp4 > pLen - 1) {
      con.cp4 -= pLen;
    }
  } else {
    // Not looping, crop final points to end of spline.
    if(con.cp1 < 0) {
      con.cp1 = 0;
    }
    if(con.cp3 > pLen - 1) {
      con.cp3 = pLen - 1;
    }
    if(con.cp4 > pLen - 1) {
      con.cp4 = pLen - 1;
    }
  }
}

Vector3D Spline::catmull_rom(double p, const Vector3D points[], bool loop) {
  if(points == nullptr) {
    return Vector3D();
  }
  int pLen = sizeof (points) / sizeof (Vector3D);
  ControlPoints con;
  getSplinePoints(con, p, pLen, loop);
  if(con.ret != -1) {
    return points[con.ret].copy();
  }
  Vector3D V;
  Vector3D v3, v2, v1;
  double t = p - ((int)p);
  double t2, t3;
  t2 = t * t;
  t3 = t2 * t;
  v3 = points[con.cp1].multiplyBy(-0.5);
  v3.add(points[con.cp2].multiplyBy(1.5));
  v3.add(points[con.cp3].multiplyBy(-1.5));
  v3.add(points[con.cp4].multiplyBy(0.5));
  v3.multiply(t3);
  v2 = points[con.cp1].copy();
  v2.add(points[con.cp2].multiplyBy(-2.5));
  v2.add(points[con.cp3].multiplyBy(2));
  v2.add(points[con.cp4].multiplyBy(-0.5));
  v2.multiply(t2);
  v1 = points[con.cp1].multiplyBy(-0.5);
  v1.add(points[con.cp3].multiplyBy(0.5));
  v1.multiply(t);
  V = points[con.cp2].copy();
  V.add(v3);
  V.add(v2);
  V.add(v1);
  return V;
}

Vector3D Spline::b_spline(double p, const Vector3D points[], bool loop) {
  if(points == nullptr) {
    return Vector3D();
  }
  int pLen = sizeof (points) / sizeof (Vector3D);
  ControlPoints con;
  getSplinePoints(con, p, pLen, loop);
  if(con.ret != -1) {
    return points[con.ret].copy();
  }
  Vector3D V;
  Vector3D v3, v2, v1;
  double t = p - ((int)p);
  double t2, t3;
  t2 = t * t;
  t3 = t2 * t;
  v3 = points[con.cp1].multiplyBy(-1);
  v3.add(points[con.cp2].multiplyBy(3));
  v3.add(points[con.cp3].multiplyBy(-3));
  v3.add(points[con.cp4]);
  v3.multiply(t3);
  v2 = points[con.cp1].multiplyBy(3);
  v2.add(points[con.cp2].multiplyBy(-6));
  v2.add(points[con.cp3].multiplyBy(3));
  v2.multiply(t2);
  v1 = points[con.cp1].multiplyBy(-3);
  v1.add(points[con.cp3].multiplyBy(3));
  v1.multiply(t);
  V = points[con.cp2].multiplyBy(4);
  V.add(points[con.cp1]);
  V.add(points[con.cp3]);
  V.add(v3);
  V.add(v2);
  V.add(v1);
  V.multiply(1.0 / 6.0);
  return V;
}

Vector3D Spline::linear(double p, const Vector3D points[], bool loop) {
  if(points == nullptr) {
    return Vector3D();
  }
  int pLen = sizeof (points) / sizeof (Vector3D);
  ControlPoints con;
  getSplinePoints(con, p, pLen, loop);
  if(con.ret != -1) {
    return points[con.ret].copy();
  }
  Vector3D V;
  double t = p - ((int)p);
  V = points[con.cp3].subtractFrom(points[con.cp2]);
  V.multiply(t);
  V.add(points[con.cp2]);
  return V;
}


}
