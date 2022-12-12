#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <limits.h>
#include "../utils/auxiliary.h"

// Sobelize single pixel.
Uint32 sobelize(Uint32 pixel, SDL_PixelFormat* format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel, format, &r, &g, &b);
    Uint8 avg = 0.3 * r + 0.59 * g + 0.11 * b;

    r = avg;
    r = 255 - r;
    g = avg;
    g = 255 - g;
    b = avg;
    b = 255 - b;
    
    Uint32 color = SDL_MapRGB(format, r, g, b);

    return color;
}

// Apply sobel filter to all surface.
int apply_sobel(const char* path)
{
    SDL_Surface* src_surface = load_image(path);
    int src_height = src_surface->h;
    int src_width = src_surface->w;
    Uint32* pixels = src_surface->pixels;
    SDL_PixelFormat* format = src_surface->format;

    SDL_LockSurface(src_surface);

    for (int j = 0 ; j < src_width; j++)
    {
        for (int i = 0 ; i < src_height;i++)
        {
            Uint32 color = pixels[i * (src_width) + j];
            pixels[i * (src_surface->w) + j] = sobelize(color, format);
        }
    }

    for(int j = 0; j < src_width; j++)
    {
        for (int i = 0; i < src_height ; i++)
        {
            if(pixels[i * (src_width) + j] < INT_MAX)
            {
                pixels[i * (src_width) + j] = 0;
            }
            else
            {
                pixels[i*(src_width)+j]=16777215;
            }
        }

    }

    SDL_UnlockSurface(src_surface);

    save_image(src_surface, "bin_image.jpeg");

    return 1;
}