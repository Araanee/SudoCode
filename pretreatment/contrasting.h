#ifndef CONTRASTING_H
#define CONTRASTING_H

// Round to max values.
Uint8 f(Uint8 pixel_color, int n);

// Compute contrast formula.
Uint32 contrast_formula(Uint32 pixel_color, double n);

// Pixel to its contrasted version.
Uint32 pixel_to_contrast(Uint32 pixel_color, SDL_PixelFormat* format);

// Surface to its contrasted version.
int surface_to_contrast(const char* path);

#endif