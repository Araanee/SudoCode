#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#pragma GCC diagnostic pop
#include<stdio.h>
#include<unistd.h> 
#include<stdlib.h>
#include "pixel_operations.h"
#include "newreader.h"
#include "../resolution/import_export.h"

// Function: Result_construct
// Description: Constructs a the png with the grid solved
// Inputs: char *grid - the solved grid
// Returns: SDL_Surface * - the solved grid

int Result_construct(char *base, char *final)
{
	int basegrid[9][9], grid[9][9];
	import_grid(base, basegrid);
	import_grid(final, grid);

	SDL_Surface *result = IMG_Load("base_grid.jpeg");
	if (result == NULL)
		errx(EXIT_FAILURE, "Could not load base grid");

	int x = 2;
	int y = 2;
	for(int i = 0; i < 9; i++)
	{
		for(int j = 0; j < 9; j++)
		{
			if(grid[i][j] == 0)
			{
				x += 72;
				if(x == 650)
				{
					x = 2;
					y += 72;
				}
			}
			else
			{
				SDL_Rect rect = {x, y, 0, 0};
				char *text = malloc(2 * sizeof(char));
				text[0] = grid[i][j]+'0';
				text[1] = '\0';
				char *path = malloc(50 * sizeof(char));
				strcpy(path, "grid_to_image/digits/");
				strcat(path, text);
				
				if (basegrid[i][j] == 0)
				{
					strcat(path, ".jpeg");
				}
				else
					strcat(path, ".png");

				SDL_Surface *number = IMG_Load(path);
				free(path);
				free(text);

				if (number == NULL)
					errx(1, "Could not load digit image");

				SDL_BlitSurface(number, NULL, result, &rect);
				SDL_FreeSurface(number);

				x += 72;
				if(x == 650)
				{
					x = 2;
					y += 72;
				}
			}
		}
	}

    	SDL_SaveBMP(result, "solved.bmp");

	return EXIT_SUCCESS;
}
