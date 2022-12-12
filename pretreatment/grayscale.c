#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include "../utils/auxiliary.h"

// Converts a colored pixel into grayscale.
Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format)
{
	Uint8 r, g, b;

	SDL_GetRGB(pixel_color, format, &r, &g, &b);

	Uint8 average = 0.4 * r + 0.35 * g + 0.25 * b;
	r = g = b = average;
	Uint32 color = SDL_MapRGB(format, r, g, b);

	return color;
}

// Surface to its grayscale version.
int surface_to_grayscale(const char* path)
{
	SDL_Surface* surface = load_image(path);
	Uint32* pixels = surface-> pixels;
	int len = surface->w * surface->h;
	SDL_PixelFormat* format = surface->format;

	SDL_LockSurface(surface);

	int p = 0;

	while (p < len)
	{
		pixels[p] = pixel_to_grayscale(pixels[p], format);
		p++;
	}

	SDL_UnlockSurface(surface);

	save_image(surface, "gray_image.jpeg");
	printf("Grayscale: done!\n");

	return 1;
}