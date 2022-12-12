#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include "../utils/auxiliary.h"

// Round to max values.
Uint8 f(Uint8 pixel_color, int n)
{
	if (pixel_color + n > 255)
	{
		return 255;
	}
	else if (pixel_color + n < 0)
	{
		return 0;
	}

	return pixel_color + n;
}

// Compute contrast formula.
Uint32 contrast_formula(Uint32 pixel_color, double n)
{
	if (pixel_color <= 255 / 2)
	{
		return (Uint8) ((255/2) * SDL_pow((double) 2 * pixel_color / 255, n));
	}
	else
	{
		return 255 - f(255 - pixel_color, n);
	}
}

// Pixel to its contrasted version.
Uint32 pixel_to_contrast(Uint32 pixel_color, SDL_PixelFormat* format)
{
	Uint8 r, g, b;

	SDL_GetRGB(pixel_color, format, &r, &g, &b);

	Uint8 average = contrast_formula(r,80)
	+ contrast_formula(g,80)
	+ contrast_formula(b,80);

	r = g = b = average;

	Uint32 color = SDL_MapRGB(format, r, g, b);

	return color;
}

// Surface to its contrasted version.
int surface_to_contrast(const char* path)
{
	SDL_Surface* surface = load_image(path);
	Uint32* pixels = surface-> pixels;
	int len = surface->w * surface->h;
	SDL_PixelFormat* format = surface->format;

	SDL_LockSurface(surface);

	int p = 0;

	while(p < len)
	{
		pixels[p] = pixel_to_contrast(pixels[p], format);
		p++;
	}

	SDL_UnlockSurface(surface);
	save_image(surface, "co_image.jpeg");
	printf("Contrasting: done!\n");

	return 1;
}