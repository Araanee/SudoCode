#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../utils/auxiliary.h"


Uint32 mean(SDL_Surface* surface, int i, int j, int radius)
{
    const int initial_h = SDL_max(i - radius, 0);
    const int initial_w = SDL_max(j - radius, 0);
    const int final_h = SDL_min(i + radius, surface->h - 1);
    const int final_w = SDL_min(j + radius, surface->w - 1);
    const int nb_pixel = ((final_h - initial_h) * (final_w - initial_w));
    const Uint32 *p = surface->pixels;

    Uint32 sum_r = 0, sum_g = 0, sum_b = 0;
    SDL_Color color;

    for (i = initial_h; i < final_h; i++)
    {
        for(j = initial_w; j < final_w; j++)
        {
            SDL_GetRGB(p[i * surface->w + j], surface->format,
                &color.r, &color.g, &color.b);

            sum_r += color.r;
            sum_g += color.g;
            sum_b += color.b;
        }
    }

    return SDL_MapRGB(surface->format,
        sum_r / nb_pixel, sum_g / nb_pixel, sum_b / nb_pixel);
}


int gaussian_blur(const char* path)
{
    SDL_Surface* src_surface = load_image(path);
    int src_height = src_surface->h;
    int src_width = src_surface->w;
    Uint32* pixels = src_surface-> pixels;
    int radius = 1;

    SDL_LockSurface(src_surface);

    for (int y = 0; y < src_height; ++y)
    {
        for(int x = 0; x < src_width; ++x)
        {
            pixels[y * src_width + x] = mean(src_surface, y, x, radius);
        }
    }

    SDL_UnlockSurface(src_surface);

    save_image(src_surface, "gau_image.jpeg");
    printf("Gaussian: done!\n");

    return 1;
}