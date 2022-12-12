#ifndef GAMMA_H
#define GAMMA_H

// Compute gamma function.
Uint8 pixel_to_gamma(Uint8 pixel_color, double gamma);

// Generates gamme filtered version of image.
int surface_to_gamma(const char* path, double gamma);

#endif