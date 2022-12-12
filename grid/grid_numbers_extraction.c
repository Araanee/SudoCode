#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <limits.h>
#include "../utils/auxiliary.h"

int extract_numbers(SDL_Surface* src_surface, int x, int src_height,
    int interval, int id)
{
    crop_image_name(src_surface, x, 0, interval, src_height, id, 1);

    return 1;
}

int grid_numbers_extract(const char* path, int start, int end)
{
	SDL_Surface* src_surface = load_image(path);
    int src_width = src_surface->w;
	int src_height = src_surface->h;
    int interval = src_width / 9;
    // printf("Grid Numbers Extraction: interval set to: %i\n", interval);
    int x = 0;

    for (int i = start; i < end; ++i)
    {
    	extract_numbers(src_surface, x, src_height, interval, i);
        x += interval;
    }

    return 1;
}

int grid_numbers_full_extract()
{
    grid_numbers_extract("1.jpeg", 1, 10);
    grid_numbers_extract("2.jpeg", 10, 19);
    grid_numbers_extract("3.jpeg", 20, 29);
    grid_numbers_extract("4.jpeg", 30, 39);
    grid_numbers_extract("5.jpeg", 40, 49);
    grid_numbers_extract("6.jpeg", 50, 59);
    grid_numbers_extract("7.jpeg", 60, 69);
    grid_numbers_extract("8.jpeg", 70, 79);
    grid_numbers_extract("9.jpeg", 80, 89);

    return 1;
}