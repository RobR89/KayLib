/*
 * Copyright 2017 Robert Reinhart.
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
#define KIMAGE_H

#include "KGraphics.h"
#include "KColor.h"

#include <string>
// include SDL headers
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

namespace KayLib
{

    class KImage
    {
    public:

        /**
         * Create an image with no surface.
         */
        KImage()
        {
            surface = nullptr;
        }

        /**
         * Create an image with a new surface.
         * @param w The with of the surface to create.
         * @param h The height of the surface to create.
         */
        KImage(int w, int h)
        {
            surface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
        }

        /**
         * Create an image with a new surface with an optional alpha channel.
         * @param w The with of the surface to create.
         * @param h The height of the surface to create.
         * @param alpah True if the alpha channel should be created.
         */
        KImage(int w, int h, bool alpha)
        {
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
            if(!alpha)
            {
                amask = 0;
            }

            surface = SDL_CreateRGBSurface(0, w, h, 32, rmask, gmask, bmask, amask);
        }

        /**
         * Create an image with the existing surface.
         * @param surf The surface to use.
         */
        KImage(SDL_Surface *surf)
        {
            surface = surf;
        }

        KImage(const KImage& orig) = delete;

        virtual ~KImage()
        {
            // if a surface exists free it.
            if(surface != nullptr)
            {
                SDL_FreeSurface(surface);
            }
        }

        /**
         * Load an image into a surface.
         * @param file The file to load.
         * @returns True if successful.
         */
        bool loadImage(const std::string file)
        {
            // if a surface exists free it.
            if(surface != nullptr)
            {
                SDL_FreeSurface(surface);
            }

            // attempt to load the image.
            surface = IMG_Load(file.c_str());

            // if it failed return an error state and push message to output.
            if(surface == nullptr)
            {
                //    KError::Error("Failed to load texture \"" + file + "\": " + IMG_GetError());
                return false;
            }

            return true;
        }

        /**
         * Load an image into a surface.
         * @param file The file to load.
         * @param format The format to load to.
         * @returns True if successful.
         */
        bool loadImage(const std::string file, const SDL_PixelFormat* format)
        {
            if(!loadImage(file))
            {
                return false;
            }

            // if it failed return an error state and push message to output.
            if(surface == nullptr)
            {
                //    KError::Error("Failed to load texture \"" + file + "\": " + IMG_GetError());
                return false;
            }

            SDL_Surface *loadedImage = surface;

            surface = SDL_ConvertSurface(loadedImage, format, 0);
            if(surface == nullptr)
            {
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

        /**
         * Render this image to a surface.
         * @param dest The surface to render to.
         * @param x The x position to render to.
         * @param y The y position to render to.
         * @note The blit size will be the size of this image.
         */
        void blitSurface(SDL_Surface *dest, int x, int y)
        {
            if(surface == nullptr || dest == nullptr)
            {
                return;
            }

            SDL_Rect pos;
            pos.x = x;
            pos.y = y;

            if(SDL_BlitSurface(surface, nullptr, dest, &pos) == -2)
            {
                // to-do: image memory was lost reload it.
            }
        }

        /**
         * Render this image to a surface.
         * @param dest The image to render to.
         * @param x The x position to render to.
         * @param y The y position to render to.
         * @note The blit size will be the size of this image.
         */
        void blitSurface(KImage *dest, int x, int y)
        {
            if(dest != nullptr)
            {
                blitSurface(dest->surface, x, y);
            }
        }

        /**
         * Render this image to a surface.
         * @param dest The surface to render to.
         * @param dRect The rectangle area of the destination to render to.
         * @note The blit size will be the size of dRect.
         */
        void blitSurface(SDL_Surface *dest, const KRect &dRect)
        {
            if(surface == nullptr || dest == nullptr)
            {
                return;
            }

            SDL_Rect dr = KRectTOSDLRect(dRect);
            SDL_Rect sr = KRectTOSDLRect(dRect);
            sr.x = sr.y = 0;
            if(SDL_BlitSurface(surface, &sr, dest, &dr) == -2)
            {
                // to-do: image memory was lost reload it.
            }
        }

        /**
         * Render this image to a surface.
         * @param dest The image to render to.
         * @param dRect The rectangle area of the destination to render to.
         * @note The blit size will be the size of dRect.
         */
        void blitSurface(KImage *dest, const KRect &dRect)
        {
            if(dest != nullptr)
            {
                blitSurface(dest->surface, dRect);
            }
        }

        /**
         * Render this image to a surface.
         * @param dest The surface to render to.
         * @param x The x position to render to.
         * @param y The y position to render to.
         * @param sRect The rectangle area of the source to render from.
         * @note The blit size will be the size of sRect.
         */
        void blitSurface(SDL_Surface *dest, int x, int y, const KRect &sRect)
        {
            if(surface == nullptr || dest == nullptr)
            {
                return;
            }

            SDL_Rect sr = KRectTOSDLRect(sRect);
            SDL_Rect dr;
            dr.x = x;
            dr.y = y;

            if(SDL_BlitSurface(surface, &sr, dest, &dr) == -2)
            {
                // to-do: image memory was lost reload it.
            }
        }

        /**
         * Render this image to a surface.
         * @param dest The image to render to.
         * @param x The x position to render to.
         * @param y The y position to render to.
         * @param sRect The rectangle area of the source to render from.
         * @note The blit size will be the size of sRect.
         */
        void blitSurface(KImage *dest, int x, int y, const KRect &sRect)
        {
            if(dest != nullptr)
            {
                blitSurface(dest->surface, x, y, sRect);
            }
        }

        /**
         * Render this image to a surface.
         * @param dest The surface to render to.
         * @param dRect The rectangle area of the destination to render to.
         * @param sRect The rectangle area of the source to render from.
         * @note The blit size will scale to the size of sRect.
         */
        void blitSurface(SDL_Surface *dest, const KRect &dRect, const KRect &sRect)
        {
            if(surface == nullptr || dest == nullptr)
            {
                return;
            }

            SDL_Rect sr = KRectTOSDLRect(sRect);
            SDL_Rect dr = KRectTOSDLRect(dRect);

            if(SDL_BlitScaled(surface, &sr, dest, &dr) == -2)
            {
                // to-do: image memory was lost reload it.
            }
        }

        /**
         * Render this image to a surface.
         * @param dest The image to render to.
         * @param dRect The rectangle area of the destination to render to.
         * @param sRect The rectangle area of the source to render from.
         * @note The blit size will scale to the size of sRect.
         */
        void blitSurface(KImage *dest, const KRect &dRect, const KRect &sRect)
        {
            if(dest != nullptr)
            {
                blitSurface(dest->surface, dRect, sRect);
            }
        }

        /**
         * Get the width of the surface.
         * @return The width of the surface.
         */
        int width()
        {
            return (surface == nullptr) ? 0 : surface->w;
        }

        /**
         * Get the height of the surface.
         * @return The height of the surface.
         */
        int height()
        {
            return (surface == nullptr) ? 0 : surface->h;
        }

        /**
         * Get the size of the image.
         * @return The size of the image.
         */
        KSize getSize()
        {
            return (surface == nullptr) ? KSize() : KSize(surface->w, surface->h);
        }

        /**
         * Get the pixel format of the surface.
         * @return The pixel format of the surface.
         */
        SDL_PixelFormat *format()
        {
            return (surface == nullptr) ? 0 : surface->format;
        }

        SDL_Surface *getSurface()
        {
            return surface;
        };

        KRect getRect()
        {
            return surface == nullptr ? KRect() : KRect(0, 0, surface->w, surface->h);
        };

    private:
        SDL_Surface *surface;

        inline SDL_Rect KRectTOSDLRect(const KRect &k)
        {
            SDL_Rect rect;
            rect.x = k.x;
            rect.y = k.y;
            rect.w = k.w;
            rect.h = k.h;
            return rect;
        }

    public:
        // Drawing support fucntions.

        /**
         * Resize the surface.  All image data is lost.
         * @param w The new width for the surface.
         * @param h The new height for the surface.
         */
        void resize(int w, int h)
        {
            if(surface != nullptr)
            {
                if(surface->w == w && surface->h == h)
                    return; // surface size remains constant. do nothing.
                // different size free old surface
                SDL_FreeSurface(surface);
            }
            // create new surface.
            surface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
        }

        /**
         * Set the color of the specified pixel.
         * @param p The location of the pixel.
         * @param color The color to set.
         */
        void setPixel(const KPoint &p, const KColor &color)
        {
            if(surface == nullptr)
            {
                return;
            }
            if(p.x >= surface->w || p.y >= surface->h || p.x < 0 || p.y < 0)
            {
                return;
            }
            int bpp = surface->format->BytesPerPixel;
            // Get pixel pointer.
            uint8_t *pixel = (uint8_t *) surface->pixels;
            pixel += p.y * surface->pitch;
            pixel += p.x * bpp;
            // Get pixel mask.
            uint32_t mask = 0xffffffff;
            uint32_t invMask = 0xffffffff << bpp;
            mask = invMask ^ 0xffffffff;
            // Set new value.
            uint32_t *pixel32 = (uint32_t *) pixel;
            *pixel32 =
                    (SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a) & mask) |
                    (*pixel32 & invMask);
            //*pixel32 = colorToUInt(color);
        }

        /**
         * Get the color of the specified pixel.
         * @param p The location of the pixel.
         * @return The color.
         */
        KColor getPixel(const KPoint &p)
        {
            KColor color;
            if(surface == nullptr)
            {
                return color;
            }
            if(p.x >= surface->w || p.y >= surface->h || p.x < 0 || p.y < 0)
            {
                return color;
            }
            uint8_t *pixel = (uint8_t *) surface->pixels;
            pixel += p.y * surface->pitch;
            pixel += p.x * surface->format->BytesPerPixel;
            SDL_GetRGBA(*((uint32_t *) pixel), surface->format, &color.r, &color.g, &color.b, &color.a);
            return color;
        }

        /**
         * Draw a line from p1 to p2
         * @param p1 First point.
         * @param p2 Second point.
         * @param color The color to draw.
         */
        void drawLine(const KPoint &p1, const KPoint &p2, const KColor &color)
        {
            if(surface == nullptr)
            {
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

            // TO-DO: improve the efficiency by using a formula not a loop.
            if(p.x >= surface->w || p.y >= surface->h || p.x < 0 || p.y < 0)
            {
                // Line starts out of bounds of image. iterate till it's in bounds.
                while(c >= 0)
                {
                    c--;
                    e2 = e;
                    if(e2 > -dx)
                    {
                        // Move horizontally.
                        e -= dy;
                        p.x += sx;
                    }
                    if(e2 < dy)
                    {
                        // Move vertically.
                        e += dx;
                        p.y += sy;
                    }
                    if(p.x < surface->w && p.y < surface->h && p.x >= 0 && p.y >= 0)
                    {
                        // We are now in bounds.
                        break;
                    }
                }
            }
            int bpp = surface->format->BytesPerPixel;
            int ptch = surface->pitch;
            // Get pixel mask.
            uint32_t mask = 0xffffffff;
            uint32_t invMask = 0xffffffff << bpp;
            mask = invMask ^ 0xffffffff;
            // Get pixel pointer.
            uint8_t *pixel = (uint8_t *) surface->pixels;
            pixel += p.y * ptch;
            pixel += p.x * bpp;
            // pre multiply so this is only once.
            bpp *= sx;
            ptch *= sy;
            // Get color.
            uint32_t col = mask & SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a);
            while(c >= 0)
            {
                c--;
                // Set new value.
                uint32_t *pixel32 = (uint32_t *) pixel;
                *pixel32 = col | (*pixel32 & invMask);
                //*pixel32 = colorToUInt(color);
                e2 = e;
                if(e2 > -dx)
                {
                    // Move horizontally.
                    e -= dy;
                    p.x += sx;
                    // Are we still in bounds?
                    if(p.x >= surface->w || p.x < 0)
                    {
                        // We have gone out of bounds, no need to continue.
                        break;
                    }
                    pixel += bpp;
                }
                if(e2 < dy)
                {
                    // Move vertically.
                    e += dx;
                    p.y += sy;
                    // Are we still in bounds?
                    if(p.y >= surface->h || p.y < 0)
                    {
                        // We have gone out of bounds, no need to continue.
                        break;
                    }
                    pixel += ptch;
                }
            }
        }

        /**
         * Draw a group of individual lines.
         * <br>Each 2 points represents a discrete unconnected line (p1->p2, p3->p4, etc...)
         * @param nPoints The number of point pointed to by p.
         * @param p The points.
         * @param color The color to draw.
         * @note nPoints / 2 lines are drawn.
         */
        void drawLines(int nPoints, const KPoint p[], const KColor &color)
        {
            int i = 0;
            while(i < nPoints - 1)
            {
                drawLine(p[i], p[i + 1], color);
                i += 2;
            }
        }

        /**
         * Draw a continuous strip of lines.
         * <br>Each point after the first represents a line from the previous point. (p1->p2, p2->p3, etc...)
         * @param nPoints The number of point pointed to by p.
         * @param p The points.
         * @param color The color to draw.
         * @note nPoints-1 lines are drawn.
         */
        void drawLineStrip(int nPoints, const KPoint p[], const KColor &color)
        {
            int i = 0;
            while(i < nPoints - 1)
            {
                drawLine(p[i], p[i + 1], color);
                i++;
            }
        }

        /**
         * Draw a continuous strip of lines.
         * <br>Each point after the first represents a line from the previous point,
         * with a final line from the last point to the first. (p1->p2, p2->p3, ..., pn->p1)
         * @param nPoints The number of point pointed to by p.
         * @param p The points.
         * @param color The color to draw.
         * @note nPoints lines are drawn.
         */
        void drawLineLoop(int nPoints, const KPoint p[], const KColor &color)
        {
            int i = 0;
            while(i < nPoints - 1)
            {
                drawLine(p[i], p[i + 1], color);
                i++;
            }
            drawLine(p[i], p[0], color);
        }


        // rectangles

        void drawRect(const KRect &rect, const KColor &color)
        {
            KPoint points[4];
            points[0].x = points[3].x = rect.x;
            points[1].x = points[2].x = rect.x + rect.w;
            points[0].y = points[1].y = rect.y;
            points[2].y = points[3].y = rect.y + rect.h;
            drawLineLoop(4, &points[0], color);
        }

        void fillRect(const KRect &rect, const KColor &color)
        {
            SDL_Rect r = KRectTOSDLRect(rect);
            if(surface != nullptr)
            {
                SDL_FillRect(surface, &r, SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a));
            }
        }

        void drawFilledRect(const KRect &rect, const KColor &color, const KColor &borderColor)
        {
            fillRect(rect, color);
            drawRect(rect, borderColor);
        }


    };

}
#endif /* KIMAGE_H */

