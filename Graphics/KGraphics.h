/*
 * File:   KGraphics.h
 * Author: Robert Reinhart
 *
 * Created on January 12, 2016, 7:09 AM
 */

#ifndef KGRAPHICS_H
#define	KGRAPHICS_H

namespace KayLib {

struct KPoint {

  KPoint() {
    x = y = 0;
  };

  KPoint(int nx, int ny) {
    x = nx;
    y = ny;
  };
  int x;
  int y;
};

struct KSize {

  KSize() {
    w = h = 0;
  }

  KSize(int nw, int nh) {
    w = nw;
    h = nh;
  }
  int w;
  int h;
};

struct KRect {

  KRect() {
    x = y = w = h = 0;
  }

  KRect(int nx, int ny, int nw, int nh) {
    x = nx;
    y = ny;
    w = nw;
    h = nh;
  }
  int x, y;
  int w, h;
};

}

#endif	/* KGRAPHICS_H */

