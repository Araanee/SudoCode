#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../utils/auxiliary.h"
#include "contrasting.h"

// Pixel to binarized pixel.
Uint32 pixel_to_black(Uint32 pixel_color, SDL_PixelFormat* format,
    Uint8 treshold)
{
    Uint8 r, g, b;

	SDL_GetRGB(pixel_color, format, &r, &g, &b);

    Uint8 average = 0.3 * r + 0.3 * g + 0.3 * b;
    Uint32 color;

    if (average >= treshold)
    {
        color = SDL_MapRGB(format, 255, 255, 255);
    }
    else
    {
        color = SDL_MapRGB(format, 0, 0, 0);
    }

	return color;
}

// Surface to binarized image.
int surface_to_black(const char* path, Uint8 treshold)
{
    SDL_Surface* surface = load_image(path);
    int len = surface->w * surface->h;
    Uint32* pixels = surface->pixels;
    SDL_PixelFormat* format = surface->format;

    SDL_LockSurface(surface);

    for (int i = 0; i < len; i++)
    {
        pixels[i] = pixel_to_black(pixels[i], format, treshold);
    }

    SDL_UnlockSurface(surface);
    save_image(surface, "bin_image.jpeg");
    printf("Binarization: done!\n");

    return 1;
}
