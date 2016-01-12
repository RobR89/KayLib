/*
 * Copyright 2016 Robert Reinhart.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef KIMAGE_H
#define	KIMAGE_H

#include "KGraphics.h"
#include "KColor.h"

#include <string>
// include SDL headers
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

namespace KayLib {

class KImage {
public:
  /**
   * Create an image with no surface.
   */
  KImage();
  /**
   * Create an image with a new surface.
   * @param w The with of the surface to create.
   * @param h The height of the surface to create.
   */
  KImage(int w, int h);
  /**
   * Create an image with a new surface with an optional alpha channel.
   * @param w The with of the surface to create.
   * @param h The height of the surface to create.
   * @param alpah True if the alpha channel should be created.
   */
  KImage(int w, int h, bool alpha);
  /**
   * Create an image with the existing surface.
   * @param surf The surface to use.
   */
  KImage(SDL_Surface *surf);
  KImage(const KImage& orig) = delete;
  virtual ~KImage();

  /**
   * Load an image into a surface.
   * @param file The file to load.
   * @returns True if successful.
   */
  bool loadImage(const std::string file);
  /**
   * Load an image into a surface.
   * @param file The file to load.
   * @param format The format to load to.
   * @returns True if successful.
   */
  bool loadImage(const std::string file, const SDL_PixelFormat* format);

  /**
   * Render the image to a surface.
   * @param dest The surface to render to.
   * @param x The x position to render to.
   * @param y The y position to render to.
   * @note The blit size will be the size of this image.
   */
  void blitSurface(SDL_Surface *dest, int x, int y);
  /**
   * Render the image to a surface.
   * @param dest The image to render to.
   * @param x The x position to render to.
   * @param y The y position to render to.
   * @note The blit size will be the size of this image.
   */
  void blitSurface(KImage *dest, int x, int y);
  /**
   * Render the image to a surface.
   * @param dest The surface to render to.
   * @param dRect The rectangle area of the destination to render to.
   * @note The blit size will be the size of dRect.
   */
  void blitSurface(SDL_Surface *dest, const KRect &dRect);
  /**
   * Render the image to a surface.
   * @param dest The image to render to.
   * @param dRect The rectangle area of the destination to render to.
   * @note The blit size will be the size of dRect.
   */
  void blitSurface(KImage *dest, const KRect &dRect);
  /**
   * Render the image to a surface.
   * @param dest The surface to render to.
   * @param x The x position to render to.
   * @param y The y position to render to.
   * @param sRect The rectangle area of the source to render from.
   * @note The blit size will be the size of sRect.
   */
  void blitSurface(SDL_Surface *dest, int x, int y, const KRect &sRect);
  /**
   * Render the image to a surface.
   * @param dest The image to render to.
   * @param x The x position to render to.
   * @param y The y position to render to.
   * @param sRect The rectangle area of the source to render from.
   * @note The blit size will be the size of sRect.
   */
  void blitSurface(KImage *dest, int x, int y, const KRect &sRect);
  /**
   * Render the image to a surface.
   * @param dest The surface to render to.
   * @param dRect The rectangle area of the destination to render to.
   * @param sRect The rectangle area of the source to render from.
   * @note The blit size will scale to the size of sRect.
   */
  void blitSurface(SDL_Surface *dest, const KRect &dRect, const KRect &sRect);
  /**
   * Render the image to a surface.
   * @param dest The image to render to.
   * @param dRect The rectangle area of the destination to render to.
   * @param sRect The rectangle area of the source to render from.
   * @note The blit size will scale to the size of sRect.
   */
  void blitSurface(KImage *dest, const KRect &dRect, const KRect &sRect);

  /**
   * Get the width of the surface.
   * @return The width of the surface.
   */
  int width() {
    return (surface == nullptr) ? 0 : surface->w;
  }

  /**
   * Get the height of the surface.
   * @return The height of the surface.
   */
  int height() {
    return (surface == nullptr) ? 0 : surface->h;
  }

  /**
   * Get the size of the image.
   * @return The size of the image.
   */
  KSize getSize() {
    return (surface == nullptr) ? KSize() : KSize(surface->w, surface->h);
  }
  /**
   * Get the pixel format of the surface.
   * @return The pixel format of the surface.
   */
  SDL_PixelFormat *format() {
    return (surface == nullptr) ? 0 : surface->format;
  }

  SDL_Surface *surface() {
    return surface;
  };

  KRect getRect() {
    return surface == nullptr ? KRect() : KRect(0, 0, surface->w, surface->h);
  };

private:
  SDL_Surface *surface;

public:
  // Drawing support fucntions.
  /**
   * Resize the surface.  All image data is lost.
   * @param w The new width for the surface.
   * @param h The new height for the surface.
   */
  void resize(int w, int h);

  /**
   * Set the color of the specified pixel.
   * @param p The location of the pixel.
   * @param color The color to set.
   */
  void setPixel(KPoint p, KColor color);
  /**
   * Get the color of the specified pixel.
   * @param p The location of the pixel.
   * @return The color.
   */
  KColor getPixel(KPoint p);

  /**
   * Draw a line from p1 to p2
   * @param p1 First point.
   * @param p2 Second point.
   * @param color The color to draw.
   */
  void drawLine(KPoint p1, KPoint p2, KColor color);
  /**
   * Draw a group of individual lines.
   * <br>Each 2 points represents a discrete unconnected line (p1->p2, p3->p4, etc...)
   * @param nPoints The number of point pointed to by p.
   * @param p The points.
   * @param color The color to draw.
   * @note nPoints / 2 lines are drawn.
   */
  void drawLines(int nPoints, KPoint *p, KColor color);
  /**
   * Draw a continuous strip of lines.
   * <br>Each point after the first represents a line from the previous point. (p1->p2, p2->p3, etc...)
   * @param nPoints The number of point pointed to by p.
   * @param p The points.
   * @param color The color to draw.
   * @note nPoints-1 lines are drawn.
   */
  void drawLineStrip(int nPoints, KPoint *p, KColor color);
  /**
   * Draw a continuous strip of lines.
   * <br>Each point after the first represents a line from the previous point,
   * with a final line from the last point to the first. (p1->p2, p2->p3, ..., pn->p1)
   * @param nPoints The number of point pointed to by p.
   * @param p The points.
   * @param color The color to draw.
   * @note nPoints lines are drawn.
   */
  void drawLineLoop(int nPoints, KPoint *p, KColor color);

  // rectangles
  void drawRect(KRect &rect, KColor color);
  void fillRect(KRect &rect, KColor color);
  void drawFilledRect(KRect &rect, KColor color, KColor borderColor);

};

}
#endif	/* KIMAGE_H */

