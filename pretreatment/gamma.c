#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include "../utils/auxiliary.h"

// Compute gamma function.
Uint8 pixel_to_gamma(Uint8 pixel_color, double gamma)
{
    return (Uint8) (255 * SDL_pow((double) pixel_color / 255, gamma));
}

// Surface to its gamma version.
int surface_to_gamma(const char* path, double gamma)
{
	SDL_Surface* src_surface = load_image(path);
	int src_height = src_surface->h;
	int src_width = src_surface->w;
	Uint32* pixels = src_surface-> pixels;
	SDL_PixelFormat* format = src_surface->format;

	SDL_LockSurface(src_surface);

	for (int y = 0; y < src_height; y++)
	{
	    for (int x = 0; x < src_width; x++)
	    {
	    	Uint8 r, g, b;
	        SDL_GetRGB(pixels[y * src_width + x], format, &r, &g, &b);

	        r = pixel_to_gamma(r, gamma);
	        g = pixel_to_gamma(g, gamma);
	        b = pixel_to_gamma(b, gamma);

	        pixels[y * src_width + x] = SDL_MapRGB(format, r, g, b);
	    }
	}

	SDL_UnlockSurface(src_surface);

	save_image(src_surface, "gam_image.jpeg");
	printf("Gamma: done!\n");

	return 1;
}