#ifndef SOBEL_H
#define SOBEL_H

// Sobelize single pixel.
Uint32 sobelize(Uint32 pixel, SDL_PixelFormat* format);

// Apply sobel filter to all surface.
int apply_sobel(const char* path);

#endif