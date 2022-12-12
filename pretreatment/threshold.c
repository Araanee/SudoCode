#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <limits.h>
#include "../utils/auxiliary.h"
#include "delete_color.h"

Uint8 get_histogram_threshold_otsu(int *histogram, int start, int end)
{
    int nbpixel = 0;

    for (int i = start; i < end; i++)
    {
        nbpixel += histogram[i];
    }

    int sum1 = 0, sum2 = 0, w1 = 0, w2 = 0, m1 = 0, m2 = 0;
    double mid = 0, max = 0;
    Uint8 threshold = 0;

    int i;

    for (i = start; i < end; i++)
    {
        sum1 += i * histogram[i];
    }

    for (i = start; i < end; i++)
    {
        w1 += histogram[i];

        if (w1 == 0)
            continue;

        w2 = nbpixel - w1;

        if (w2 == 0)
            break;

        sum2 += i * histogram[i];
        m1 = sum2 / w1;
        m2 = (sum1 - sum2) / w2;
        mid = w1 * w2 * (m1 - m2) * (m1 - m2);

        if (mid >= max)
        {
            threshold = i;
            max = mid;
        }
    }

    return squeeze(threshold, 70, 240);
}

Uint8 get_histogram_threshold(int *histogram, int start, int end)
{
    return get_histogram_threshold_otsu(histogram, start, end);
}

int check_pixel_majority(const char* path)
{
	SDL_Surface* src_surface = load_image(path);
	int src_height = src_surface->h;
    int src_width = src_surface->w;
    int color_black = 0;
    int color_white = 0;
    Uint32 color;

    for (int x = 0; x < src_width; ++x)
    {
        for (int y = 0; y < src_height; ++y)
        {
            Uint8 r, g, b;
            color = SDL_ReadPixel(src_surface, x, y);
            SDL_GetRGB(color, src_surface->format, &r, &g, &b);
            if (r < 5 && g < 5 && b < 5) // True if black / probable edge.
            {
                ++color_black;
            }
            else
            {
            	++color_white;
            }

        }
    }

    if (max(color_black, color_white) == color_black)
    {
    	return 0;
    }

    return 1;
}

int filter_threshold(const char* path)
{
	SDL_Surface* src_surface = load_image(path);
    int src_height = src_surface->h;
    int src_width = src_surface->w;
    int *hist = image_grayscale_histogram(src_surface, 0, src_width, 0,
        src_height);
    Uint8 threshold = get_histogram_threshold(hist, 0, 256);
    int otsu = surface_to_black(path, threshold);

    free(hist);

    int majority = check_pixel_majority("bin_image.jpeg");

    if (majority)
    {
    	return otsu;
    }

    int fixed_threshold = surface_to_black(path, 157);

    return fixed_threshold;
}

Uint8 get_less(SDL_Surface *surface)
{
    Uint32 pix;
    Uint8 less = 255;
    SDL_Color col;
    
    for (int i = 0 ; i < surface->h ; i++)
    {
        for (int j = 0 ; j < surface->w ; j++)
        {
            pix = SDL_ReadPixel(surface, j, i);
            SDL_GetRGB(pix, surface->format, &col.r, &col.g, &col.b);
            if (col.r < less)
                less = col.r;
        }
    }
    return less;
}

Uint8 get_max(SDL_Surface *surface)
{
    Uint32 pix;
    Uint8 max = 0;
    SDL_Color col;
    
    for (int i = 0 ; i < surface->h ; i++)
    {
        for (int j = 0 ; j < surface->w ; j++)
        {
            pix = SDL_ReadPixel(surface, j, i);
            SDL_GetRGB(pix, surface->format, &col.r, &col.g, &col.b);
            if (col.r > max)
                max = col.r;
        }
    }
    return max;
}

Uint8 get_var(SDL_Surface *surface)
{
    Uint32 pix;
    unsigned long average = 0, var = 0;
    SDL_Color col;
    
    for (int i = 0 ; i < surface->h ; i++)
    {
        for (int j = 0 ; j < surface->w ; j++)
        {
            pix = SDL_ReadPixel(surface, j, i);
            SDL_GetRGB(pix, surface->format, &col.r, &col.g, &col.b);
            average += col.r;
        }
    }
    average = average / (surface->w * surface->h);

    for (int i = 0 ; i < surface->h ; i++)
    {
        for (int j = 0 ; j < surface->w ; j++)
        {
            pix = SDL_ReadPixel(surface, j, i);
            SDL_GetRGB(pix, surface->format, &col.r, &col.g, &col.b);
            var += (col.r - average) * (col.r - average);
        }
    }
    var = var / (surface->w * surface->h);

    return (Uint8)(average - (long)(sqrt((double)var)));

}

int blackwhite(const char* path)
{
    SDL_Surface* surface = load_image(path);
    SDL_LockSurface(surface);
    SDL_Color col;
    Uint8 less = get_less(surface), var = get_var(surface);
    Uint32 pix;
    
    for (int b = 0 ; b <= 1 ; b++)
    {
        for (int i = 0 ; i < surface->h ; i++)
        {
            for (int j = 0 ; j < surface->w ; j++)
            {
                pix = SDL_ReadPixel(surface, j, i);
                SDL_GetRGB(pix, surface->format, &col.r, &col.g, &col.b);
                if (!b)
                {
                    pix = SDL_MapRGB(surface->format, 255, 255, 255);
                    if (col.r > less + var)
                        SDL_WritePixel(surface, j, i, pix);
                }
                else if (col.r != 255)
                {
                    pix = SDL_MapRGB(surface->format, 0, 0, 0);
                    SDL_WritePixel(surface, j, i, pix);
                }
            }
        }
    }

    SDL_UnlockSurface(surface);

    save_image(surface, "bin_image.jpeg");

    return 1;
}