#ifndef GAUSSIAN_H
#define GAUSSIAN_H

// Compute gaussian mean.
Uint32 mean(SDL_Surface* surface, int i, int j, int radius);

// Compute gaussian blur.
int gaussian_blur(const char* path);

#endif