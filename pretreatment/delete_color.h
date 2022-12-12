#ifndef DELETE_COLOR_H
#define DELETE_COLOR_H

// Pixel to binarized pixel.
Uint32 pixel_to_black(Uint32 pixel_color, SDL_PixelFormat* format);

// Surface to binarized image.
int surface_to_black(const char* path, Uint8 treshold);

#endif