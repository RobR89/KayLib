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
  uint32_t rmask, gmask, bmask, amask;

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

void KImage::setPixel(const KPoint &p, const KColor &color) {
  if(surface == nullptr) {
    return;
  }
  if(p.x >= surface->w || p.y >= surface->h || p.x < 0 || p.y < 0) {
    return;
  }
  uint8_t *pixel = (uint8_t *)surface->pixels;
  pixel += p.y * surface->pitch;
  pixel += p.x * surface->format->BytesPerPixel;
  uint32_t *pixel32 = (uint32_t *)pixel;
  *pixel32 = colorToUInt(color);
}

KColor KImage::getPixel(const KPoint &p) {
  KColor color;
  if(surface == nullptr) {
    return color;
  }
  if(p.x >= surface->w || p.y >= surface->h || p.x < 0 || p.y < 0) {
    return color;
  }
  uint8_t *pixel = (uint8_t *)surface->pixels;
  pixel += p.y * surface->pitch;
  pixel += p.x * surface->format->BytesPerPixel;
  SDL_GetRGBA(*((uint32_t *)pixel), surface->format, &color.r, &color.g, &color.b, &color.a);
  return color;
}

void KImage::drawLine(const KPoint &p1, const KPoint &p2, const KColor &color) {
  if(surface == nullptr) {
    return;
  }
  KPoint p = p1;
  int dx = abs(p2.x - p1.x);
  int dy = abs(p2.y - p1.y);
  int sx = p1.x < p2.x ? 1 : -1;
  int sy = p1.y < p2.y ? 1 : -1;
  int e = (dx > dy ? dx : -dy) / 2;
  int e2;
  int c = dx > dy ? dx : dy;

  if(p.x >= surface->w || p.y >= surface->h || p.x < 0 || p.y < 0) {
    // Line starts out of bounds of image. iterate till it's in bounds.
    while(c >= 0) {
      c--;
      e2 = e;
      if(e2 > -dx) {
        // Move horizontally.
        e -= dy;
        p.x += sx;
      }
      if(e2 < dy) {
        // Move vertically.
        e += dx;
        p.y += sy;
      }
      if(p.x < surface->w && p.y < surface->h && p.x >= 0 && p.y >= 0) {
        // We are now in bounds.
        break;
      }
    }
  }
  int bpp = surface->format->BytesPerPixel;
  int ptch = surface->pitch;
  uint8_t *pixel = (uint8_t *)surface->pixels;
  pixel += p.y * ptch;
  pixel += p.x * bpp;
  // pre multiply so this is only once.
  bpp *= sx;
  ptch *= sy;
  while(c >= 0) {
    c--;
    uint32_t *pixel32 = (uint32_t *)pixel;
    *pixel32 = colorToUInt(color);
    e2 = e;
    if(e2 > -dx) {
      // Move horizontally.
      e -= dy;
      p.x += sx;
      // Are we still in bounds?
      if(p.x >= surface->w || p.x < 0) {
        // We have gone out of bounds, no need to continue.
        break;
      }
      pixel += bpp;
    }
    if(e2 < dy) {
      // Move vertically.
      e += dx;
      p.y += sy;
      // Are we still in bounds?
      if(p.y >= surface->h || p.y < 0) {
        // We have gone out of bounds, no need to continue.
        break;
      }
      pixel += ptch;
    }
  }
}

void KImage::drawLines(int nPoints, const KPoint p[], const KColor &color) {
  int i = 0;
  while(i < nPoints - 1) {
    drawLine(p[i], p[i + 1], color);
    i += 2;
  }
}

void KImage::drawLineStrip(int nPoints, const KPoint p[], const KColor &color) {
  int i = 0;
  while(i < nPoints - 1) {
    drawLine(p[i], p[i + 1], color);
    i++;
  }
}

void KImage::drawLineLoop(int nPoints, const KPoint p[], const KColor &color) {
  int i = 0;
  while(i < nPoints - 1) {
    drawLine(p[i], p[i + 1], color);
    i++;
  }
  drawLine(p[i], p[0], color);
}

void KImage::drawRect(const KRect &rect, const KColor &color) {
  KPoint points[4];
  points[0].x = points[3].x = rect.x;
  points[1].x = points[2].x = rect.x + rect.w;
  points[0].y = points[1].y = rect.y;
  points[2].y = points[3].y = rect.y + rect.h;
  drawLineLoop(4, &points[0], color);
}

void KImage::fillRect(const KRect &rect, const KColor &color) {
  SDL_Rect r = KRectTOSDLRect(rect);
  if(surface != nullptr) {
    SDL_FillRect(surface, &r, colorToUInt(color));
  }
}

void KImage::drawFilledRect(const KRect &rect, const KColor &color, const KColor &borderColor) {
  fillRect(rect, color);
  drawRect(rect, borderColor);
}

}
