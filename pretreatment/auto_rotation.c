#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include "../utils/auxiliary.h"
#include "rotation.h"

int hough_transform(const char* path)
{
	SDL_Surface* src_surface = load_image(path);
	int src_height = src_surface->h;
	int src_width = src_surface->w;
    // Biggest possible accumulator.
    int (*accumulator)[src_height] = calloc(src_width, sizeof *accumulator);
    printf("Auto-Rotation: accumulator created.\n");
    Uint32 color; // Needed to check if point can be considered as edge.
    int rho;

    printf("Auto-Rotation: accumulator initialization to 0...\n");

    for (int i = 0; i < src_width; ++i)
    {
        for (int j = 0; j < src_height; ++j)
        {
            // Initialize accumulator to zeros.
            accumulator[i][j] = 0;
        }
    }

    printf("Auto-Rotation: accumulator hough transform loop...\n");

    for (int x = 0; x < src_width; ++x)
    {
        for (int y = 0; y < src_height; ++y)
        {
            Uint8 r, g, b;
            color = SDL_ReadPixel(src_surface, x, y);
            SDL_GetRGB(color, src_surface->format, &r, &g, &b);
            if (r < 5 && g < 5 && b < 5) // True if black / probable edge.
            {
                for (int theta = 0; theta < 180; ++theta)
                {
                    rho = ceil(x * cos(degrees_to_rad(theta))
                        + y * sin(degrees_to_rad(theta)));
                    accumulator[theta][rho]++;
                }
            }

        }
    }

    printf("Auto-Rotation: hough transform done!\n");
    printf("Auto-Rotation: theta votes in process...\n");

    int maxi = 0;
    int theta = 0;

    for (int i = 0; i < src_width; ++i)
    {
        for (int j = 0; j < src_height; ++j)
        {
            if (accumulator[i][j] >= maxi)
            {
                maxi = accumulator[i][j];
                theta = i;
            }
        }
    }

    printf("Auto-Rotation: theta votes done...\n");

    free(accumulator); // Free memory usage.

    printf("Auto-Rotation: found angle is: %i\n", 450 - theta);

    return 450-theta;
}

int auto_rotate(const char* path)
{
    int angle = hough_transform(path);
    printf("Auto-Rotation: angle used for rotation: %i\n", angle);
    int rotate = rotate_by_angle(path, (double) angle);
    printf("Auto-Rotation: starting rotation with angle.\n");

    return rotate;
}
