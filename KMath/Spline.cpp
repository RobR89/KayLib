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
    return points[con.ret];
  }
  Vector3D V;
  Vector3D v3, v2, v1;
  double t = p - ((int)p);
  double t2, t3;
  t2 = t * t;
  t3 = t2 * t;
  v3 = points[con.cp1] * -0.5;
  v3 += points[con.cp2] * 1.5;
  v3 += points[con.cp3] * -1.5;
  v3 += points[con.cp4] * 0.5;
  v3 *= t3;
  v2 = points[con.cp1];
  v2 += points[con.cp2] * -2.5;
  v2 += points[con.cp3] * 2;
  v2 += points[con.cp4] * -0.5;
  v2 *= t2;
  v1 = points[con.cp1] * -0.5;
  v1 += points[con.cp3] * 0.5;
  v1 *= t;
  V = points[con.cp2];
  V += v3;
  V += v2;
  V += v1;
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
    return points[con.ret];
  }
  Vector3D V;
  Vector3D v3, v2, v1;
  double t = p - ((int)p);
  double t2, t3;
  t2 = t * t;
  t3 = t2 * t;
  v3 = points[con.cp1] * -1;
  v3 += points[con.cp2] * 3;
  v3 += points[con.cp3] * -3;
  v3 += points[con.cp4];
  v3 *= t3;
  v2 = points[con.cp1] * 3;
  v2 += points[con.cp2] * -6;
  v2 += points[con.cp3] * 3;
  v2 *= t2;
  v1 = points[con.cp1] * -3;
  v1 += points[con.cp3] * 3;
  v1 *= t;
  V = points[con.cp2] * 4;
  V += points[con.cp1];
  V += points[con.cp3];
  V += v3;
  V += v2;
  V += v1;
  V *= 1.0 / 6.0;
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
    return points[con.ret];
  }
  Vector3D V;
  double t = p - ((int)p);
  V = points[con.cp3] - points[con.cp2];
  V *= (t);
  V += (points[con.cp2]);
  return V;
}


}
