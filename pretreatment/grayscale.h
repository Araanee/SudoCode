#ifndef GRAYSCALE_H
#define GRAYSCALE_H

// Converts a colored pixel into grayscale.
Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format);

// Surface to its grayscale version.
int surface_to_grayscale(const char* path);

#endif