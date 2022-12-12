#ifndef TRESHOLD_H
#define TRESHOLD_H

Uint8 get_histogram_threshold_otsu(int *histogram, int start, int end);

Uint8 get_histogram_threshold(int *histogram, int start, int end);

int check_pixel_majority(SDL_Surface* src_surface);

int filter_threshold(const char* path);

Uint8 get_less(SDL_Surface *surface);

Uint8 get_max(SDL_Surface *surface);

Uint8 get_var(SDL_Surface *surface);

int blackwhite(const char* path);

int get_pixel_mean(SDL_Surface* src_surface, int x, int y, int n1, int n2);

int averaging_bin(const char* path);

#endif