#include "KMath.h"

namespace KayLib {

LatLonHead Spherical::get() const {
  LatLonHead llh = get_rad();
  llh.Latitude = radiansToDegrees(llh.Latitude);
  llh.Longatude = radiansToDegrees(llh.Longatude);
  llh.Heading = radiansToDegrees(llh.Heading);
  return llh;
}

LatLon Spherical::getLocation() const {
  LatLon ll = getLocation_rad();
  ll.Latitude = radiansToDegrees(ll.Latitude);
  ll.Longatude = radiansToDegrees(ll.Longatude);
  return ll;
}

double Spherical::getHeading() const {
  return radiansToDegrees(getHeading_rad());
}

LatLonHead Spherical::get_rad() const {
  double la, lo, hd;
  double y1, y3, x1, z1;

  double wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;
  x2 = x + x;
  y2 = y + y;
  z2 = z + z;
  xx = x * x2;
  xy = x * y2;
  xz = x * z2;
  yy = y * y2;
  yz = y * z2;
  zz = z * z2;
  wx = w * x2;
  wy = w * y2;
  wz = w * z2;

  x1 = xz + wy;

  y3 = xy + wz;
  y2 = 1.0f - (xx + zz);
  y1 = yz - wx;

  z1 = 1.0f - (xx + yy);

  la = asin(y1);
  double clat = cos(la);

  // scale latatudnal ring to unit size
  lo = sqrt(1.0 - (y1 * y1));
  lo = x1 / lo;
  // calculate longitude
  lo = asin(lo);
  if(z1 < 0) {
    lo = pi - lo;
  } else if(x1 < 0) {
    lo = (2 * pi) + lo;
  }

  if(clat == 0) {
    y2 = 0;
  } else {
    y2 /= clat;
  }
  hd = y2;
  if(hd > 1) {
    hd = 1;
  }
  if(hd < -1) {
    hd = -1;
  }
  hd = acos(hd);
  if(y3 > 0) {
    hd = (2 * pi) - hd;
  }

  LatLonHead llh = LatLonHead();
  llh.Latitude = la;
  llh.Longatude = lo;
  llh.Heading = hd;
  return llh;
}

LatLon Spherical::getLocation_rad() const {
  double la, lo;
  double y1, x1, z1;

  double wx, wy, xx, yy, yz, xz, x2, y2, z2;
  x2 = x + x;
  y2 = y + y;
  z2 = z + z;
  xx = x * x2;
  xz = x * z2;
  yy = y * y2;
  yz = y * z2;
  wx = w * x2;
  wy = w * y2;

  x1 = xz + wy;

  y1 = yz - wx;

  z1 = 1.0f - (xx + yy);

  la = asin(y1);

  // scale latitudinal ring to unit size
  lo = sqrt(1.0 - (y1 * y1));
  lo = x1 / lo;
  // calculate longitude
  lo = asin(lo);
  if(z1 < 0) {
    lo = pi - lo;
  } else if(x1 < 0) {
    lo = (2 * pi) + lo;
  }

  LatLon ll = LatLon();
  ll.Latitude = la;
  ll.Longatude = lo;
  return ll;
}

double Spherical::getHeading_rad() const {
  double y1, y3;
  double wz, xx, xy, zz, x2, y2, z2, yz, wx;
  x2 = x + x;
  y2 = y + y;
  z2 = z + z;
  xx = x * x2;
  xy = x * y2;
  zz = z * z2;
  yz = y * z2;
  wx = w * x2;
  wz = w * z2;

  y1 = yz - wx;
  y3 = xy + wz;
  y2 = 1.0f - (xx + zz);

  double clat = sqrt(1 - (y1 * y1));

  if(clat == 0) {
    y2 = 0;
  } else {
    y2 /= clat;
  }
  double hd = y2;
  if(hd > 1) {
    hd = 1;
  }
  if(hd < -1) {
    hd = -1;
  }
  hd = acos(hd);
  if(y3 > 0) {
    hd = (2 * pi) - hd;
  }

  return hd;
}

void Spherical::set(double lat, double lon, double head) {
  double hd = degreesToRadians(head);
  double la = degreesToRadians(lat);
  double lo = degreesToRadians(lon);
  set_rad(la, lo, hd);
}

void Spherical::set_rad(double lat, double lon, double head) {
  double w1, z1, w2, y2, w3, x3;
  double w23, x23, y23, z23;

  w1 = cos(-head / 2);
  z1 = sin(-head / 2);
  w2 = cos(lon / 2);
  y2 = sin(lon / 2);
  w3 = cos(-lat / 2);
  x3 = sin(-lat / 2);

  w23 = w2 * w3;
  x23 = w2 * x3;
  y23 = y2 * w3;
  z23 = -y2 * x3;

  w = w23 * w1 - z23 * z1;
  x = x23 * w1 + y23 * z1;
  y = y23 * w1 - x23 * z1;
  z = w23 * z1 + z23 * w1;
}

void Spherical::setLocation(double lat, double lon) {
  set(lat, lon, getHeading());
}

void Spherical::setLocation_rad(double lat, double lon) {
  set_rad(lat, lon, getHeading_rad());
}

void Spherical::setHeading(double head) {
  LatLon ll = getLocation();
  set(ll.Latitude, ll.Longatude, head);
}

void Spherical::setHeading_rad(double head) {
  LatLon ll = getLocation_rad();
  set_rad(ll.Latitude, ll.Longatude, head);
}

double Spherical::getRange(const Spherical &target) const {
  return radiansToDegrees(getRange_rad(target));
}

double Spherical::getRange_rad(const Spherical &target) const {
  double wx, wy, xx, yy, yz, xz, x2, y2, z2;

  x2 = x + x;
  y2 = y + y;
  z2 = z + z;
  xx = x * x2;
  xz = x * z2;
  yy = y * y2;
  yz = y * z2;
  wx = w * x2;
  wy = w * y2;

  double X, Y, Z;
  X = xz + wy;
  Y = yz - wx;
  Z = 1.0f - (xx + yy);

  x2 = target.x + target.x;
  y2 = target.y + target.y;
  z2 = target.z + target.z;
  xx = target.x * x2;
  xz = target.x * z2;
  yy = target.y * y2;
  yz = target.y * z2;
  wx = target.w * x2;
  wy = target.w * y2;

  X *= xz + wy;
  Y *= yz - wx;
  Z *= 1.0f - (xx + yy);

  return acos(X + Y + Z);
}

double Spherical::getBearing(const Spherical &target) const {
  return radiansToDegrees(getBearing_rad(target));
}

double Spherical::getBearing_rad(const Spherical &target) const {
  double dlon, dlat, a, d;
  double bear;

  LatLonHead llh = get_rad();
  LatLon tl = target.getLocation_rad();

  dlon = tl.Longatude - llh.Longatude;
  dlat = tl.Latitude - llh.Latitude;
  a = sqr(sin(dlat / 2)) + cos(llh.Latitude) * cos(tl.Latitude) * sqr(sin(dlon / 2));
  d = 2 * atan2(sqrt(a), sqrt(1 - a));

  bear = acos((sin(tl.Latitude) - sin(llh.Latitude) * cos(d)) / (sin(d) * cos(llh.Latitude)));
  // If target is to the west.
  if(sin(tl.Longatude - llh.Longatude) < 0) {
    bear = (2 * pi) - bear;
  }

  bear -= llh.Heading;
  // if bearing is less than -180 degrees reverse the direction.
  if(bear < -pi) {
    // note: this condition may not be possible?  Should be tested for possible optimization.
    bear = (2 * pi) - bear;
  }
  // If bearing is greater than 180 degrees reverse the direction.
  if(bear > pi) {
    bear = -((2 * pi) - bear);
  }
  // If bearing is 360 degrees return it as zero.
  if(bear == 2 * pi) {
    bear = 0;
  }

  return bear;
}

void Spherical::move(double arc) {
  move_rad(degreesToRadians(arc));
}

void Spherical::move_rad(double arc) {
  double w3, x3, w1, x1, y1, z1;

  w3 = cos(arc / 2);
  x3 = sin(arc / 2);

  w1 = w;
  x1 = x;
  y1 = y;
  z1 = z;

  w = w1 * w3 - x1 * x3;
  x = w1 * x3 + x1 * w3;
  y = y1 * w3 + z1 * x3;
  z = z1 * w3 - y1 * x3;
}

void Spherical::turn(double turn) {
  return turn_rad(degreesToRadians(turn));
}

void Spherical::turn_rad(double turn) {
  double w1, z1;
  double w2, x2, y2, z2;

  w1 = cos(turn / 2);
  z1 = sin(turn / 2);

  w2 = w;
  x2 = x;
  y2 = y;
  z2 = z;

  w = w2 * w1 - z2 * z1;
  x = x2 * w1 + y2 * z1;
  y = y2 * w1 - x2 * z1;
  z = w2 * z1 + z2 * w1;
}

}
