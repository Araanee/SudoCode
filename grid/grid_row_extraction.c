#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../utils/auxiliary.h"

int extract_row(SDL_Surface* src_surface, int y, int src_width,
    int interval, int id)
{
    crop_image_name(src_surface, 0, y, src_width, interval + 10, id, 0);

    return 1;
}

int grid_row_extract(const char* path)
{
	SDL_Surface* src_surface = load_image(path);
    int src_width = src_surface->w;
	int src_height = src_surface->h;
    // printf("Grid Row Extraction: height found.\n");
    int interval = src_height / 9;
    // printf("Grid Row Extraction: interval set to: %i\n", interval);
    int y = 0;

    for (int i = 1; i < 10; ++i)
    {
    	extract_row(src_surface, y, src_width, interval, i);
        y += interval;
    }

    return 1;
}