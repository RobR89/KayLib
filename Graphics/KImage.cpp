#include "KImage.h"

namespace KayLib {

inline SDL_Rect KRectTOSDLRect(const KRect &k) {
  SDL_Rect rect;
  rect.x = k.x;
  rect.y = k.y;
  rect.w = k.w;
  rect.h = k.h;
  return rect;
}

KImage::KImage() {
  surface = nullptr;
}

KImage::KImage(int w, int h) {
  surface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
}

KImage::KImage(int w, int h, bool alpha) {
  Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  rmask = 0xff000000;
  gmask = 0x00ff0000;
  bmask = 0x0000ff00;
  amask = 0x000000ff;
#else
  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
  amask = 0xff000000;
#endif
  if(!alpha) {
    amask = 0;
  }

  surface = SDL_CreateRGBSurface(0, w, h, 32, rmask, gmask, bmask, amask);
}

KImage::KImage(SDL_Surface *surf) {
  surface = surf;
}

KImage::~KImage(void) {
  // if a surface exists free it.
  if(surface != nullptr) {
    SDL_FreeSurface(surface);
  }
}

bool KImage::loadImage(const std::string file) {
  // if a surface exists free it.
  if(surface != nullptr) {
    SDL_FreeSurface(surface);
  }

  // attempt to load the image.
  surface = IMG_Load(file.c_str());

  // if it failed return an error state and push message to output.
  if(surface == nullptr) {
    //    KError::Error("Failed to load texture \"" + file + "\": " + IMG_GetError());
    return false;
  }

  return true;
}

bool KImage::loadImage(const std::string file, const SDL_PixelFormat* format) {
  if(!loadImage(file)) {
    return false;
  }

  // if it failed return an error state and push message to output.
  if(surface == nullptr) {
    //    KError::Error("Failed to load texture \"" + file + "\": " + IMG_GetError());
    return false;
  }

  SDL_Surface *loadedImage = surface;

  surface = SDL_ConvertSurface(loadedImage, format, 0);
  if(surface == nullptr) {
    // if the surface was not optimized return an error state and push message to output.
    // push the error message.
    //    KError::Error("Failed to optimize surface \"" + file + "\": " + SDL_GetError());
    // free the image (we no longer need it).
    SDL_FreeSurface(loadedImage);
    return false;
  }
  // free the image (we no longer need it).
  SDL_FreeSurface(loadedImage);

  return true;
}

void KImage::blitSurface(SDL_Surface *dest, int x, int y) {
  if(surface == nullptr || dest == nullptr) {
    return;
  }

  SDL_Rect pos;
  pos.x = x;
  pos.y = y;

  if(SDL_BlitSurface(surface, nullptr, dest, &pos) == -2) {
    // to-do: image memory was lost reload it.
  }
}

void KImage::blitSurface(KImage *dest, int x, int y) {
  if(dest != nullptr) {
    blitSurface(dest->surface, x, y);
  }
}

void KImage::blitSurface(SDL_Surface *dest, const KRect &dRect) {
  if(surface == nullptr || dest == nullptr) {
    return;
  }

  SDL_Rect dr = KRectTOSDLRect(dRect);
  SDL_Rect sr = KRectTOSDLRect(dRect);
  sr.x = sr.y = 0;
  if(SDL_BlitSurface(surface, &sr, dest, &dr) == -2) {
    // to-do: image memory was lost reload it.
  }
}

void KImage::blitSurface(KImage *dest, const KRect &dRect) {
  if(dest != nullptr) {
    blitSurface(dest->surface, dRect);
  }
}

void KImage::blitSurface(SDL_Surface *dest, int x, int y, const KRect &sRect) {
  if(surface == nullptr || dest == nullptr) {
    return;
  }

  SDL_Rect sr = KRectTOSDLRect(sRect);
  SDL_Rect dr;
  dr.x = x;
  dr.y = y;

  if(SDL_BlitSurface(surface, &sr, dest, &dr) == -2) {
    // to-do: image memory was lost reload it.
  }
}

void KImage::blitSurface(KImage *dest, int x, int y, const KRect &sRect) {
  if(dest != nullptr) {
    blitSurface(dest->surface, x, y, sRect);
  }
}

void KImage::blitSurface(SDL_Surface *dest, const KRect &dRect, const KRect &sRect) {
  if(surface == nullptr || dest == nullptr) {
    return;
  }

  SDL_Rect sr = KRectTOSDLRect(sRect);
  SDL_Rect dr = KRectTOSDLRect(dRect);

  if(SDL_BlitScaled(surface, &sr, dest, &dr) == -2) {
    // to-do: image memory was lost reload it.
  }
}

void KImage::blitSurface(KImage *dest, const KRect &dRect, const KRect &sRect) {
  if(dest != nullptr) {
    blitSurface(dest->surface, dRect, sRect);
  }
}

//**************************************************************

#define colorToUInt(color) SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a)

void KImage::resize(int w, int h) {
  if(surface != nullptr) {
    if(surface->w == w && surface->h == h)
      return; // surface size remains constant. do nothing.
    // different size free old surface
    SDL_FreeSurface(surface);
  }
  // create new surface.
  surface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
}

void KImage::setPixel(KPoint p, KColor color) {
  if(surface == nullptr) {
    return;
  }
  if(p.x >= surface->w || p.y >= surface->h) {
    return;
  }
  if(p.x < 0 || p.y < 0) {
    return;
  }
  Uint8 *pixel = (Uint8 *)surface->pixels;
  pixel += p.y * surface->pitch;
  pixel += p.x * surface->format->BytesPerPixel;
  Uint32 *pixel32 = (Uint32 *)pixel;
  *pixel32 = colorToUInt(color);
}

KColor KImage::getPixel(KPoint p) {
  KColor color;
  if(surface == nullptr) {
    return color;
  }
  if(p.x >= surface->w || p.y >= surface->h) {
    return color;
  }
  if(p.x < 0 || p.y < 0) {
    return color;
  }
  Uint8 *pixel = (Uint8 *)surface->pixels;
  pixel += p.y * surface->pitch;
  pixel += p.x * surface->format->BytesPerPixel;
  SDL_GetRGBA(*((Uint32 *)pixel), surface->format, &color.r, &color.g, &color.b, &color.a);
  return color;
}

void KImage::drawLine(KPoint p1, KPoint p2, KColor color) {
  int dx = abs(p2.x - p1.x);
  int dy = abs(p2.y - p1.y);
  int sx = p1.x < p2.x ? 1 : -1;
  int sy = p1.y < p2.y ? 1 : -1;
  int e = (dx > dy ? dx : -dy) / 2;
  int e2;
  int c = dx > dy ? dx : dy;

  while(c >= 0) {
    c--;
    setPixel(p1, color);
    e2 = e;
    if(e2 >-dx) {
      e -= dy;
      p1.x += sx;
    }
    if(e2 < dy) {
      e += dx;
      p1.y += sy;
    }
  }
}

void KImage::drawLines(int nPoints, KPoint *p, KColor color) {
  while(nPoints >= 2) {
    drawLine(*p, *(p++), color);
    p++;
    nPoints -= 2;
  }
}

void KImage::drawLineStrip(int nPoints, KPoint *p, KColor color) {
  while(nPoints >= 2) {
    drawLine(*p, *(p++), color);
    nPoints--;
  }
}

void KImage::drawLineLoop(int nPoints, KPoint *p, KColor color) {
  KPoint *start = p;
  while(nPoints >= 2) {
    drawLine(*p, *(p++), color);
    nPoints--;
  }
  drawLine(*p, *start, color);
}

void KImage::drawRect(KRect &rect, KColor color) {
  KPoint points[4];
  points[0].x = points[3].x = rect.x;
  points[1].x = points[2].x = rect.x + rect.w;
  points[0].y = points[1].y = rect.y;
  points[2].y = points[3].y = rect.y + rect.h;
  drawLineLoop(4, &points[0], color);
}

void KImage::fillRect(KRect &rect, KColor color) {
  SDL_Rect r = KRectTOSDLRect(rect);
  if(surface != nullptr) {
    SDL_FillRect(surface, &r, colorToUInt(color));
  }
}

void KImage::drawFilledRect(KRect &rect, KColor color, KColor borderColor) {
  fillRect(rect, color);
  drawRect(rect, borderColor);
}

}