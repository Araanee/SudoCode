#include "img.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

#define MAXCHAR 10000

SDL_Surface* load_image(const char* path)
{
    SDL_Surface* surface = IMG_Load(path);
    if (surface == NULL)
        errx(EXIT_FAILURE,"%s", SDL_GetError());
    // Creation of a new surface from the surface.
    SDL_Surface* secondary = SDL_ConvertSurfaceFormat(surface,
        SDL_PIXELFORMAT_RGB888, 0);
     if (secondary == NULL)
        errx(EXIT_FAILURE,"%s", SDL_GetError());

     // Free the surface.
     SDL_FreeSurface(surface);

     // Return the surface in the desired format.
     return secondary;
}


Uint32 SDL_ReadPixel(SDL_Surface* surface, int x, int y)
{
  int bpp = surface->format->BytesPerPixel;

  Uint8 *p = (Uint8*) surface->pixels + y * surface->pitch + x * bpp;

  switch(bpp)
  {
             case 1:
                  return *p;
             case 2:
                  return *(Uint16*) p;
             case 3:
                 if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                    {
                        return p[0] << 16 | p[1] << 8 | p[2];
                    }
                 else
                 {
                    return p[0] | p[1] << 8 | p[2] << 16;
                 }
             case 4:
                  return *(Uint32*) p;
             default:
                  return 0;
  }
}

void sdl_to_csv(const char* path, char* file_string)
{
	SDL_Surface* src_surface = load_image(path);
	int src_width = src_surface->w;
	int src_height = src_surface->h;
	Uint32 color;
	FILE* fp = fopen(file_string, "w");

	// Insert dummy label.
	fputc('x', fp);


	for (int x = 0; x < src_width; ++x)
	{
		for (int y = 0; y < src_height; ++y)
		{
			fputc(',', fp);
			Uint8 r, g, b;
			color = SDL_ReadPixel(src_surface, x, y);
			SDL_GetRGB(color, src_surface->format, &r, &g, &b);
			if (r < 127 && g < 127 && b < 127)
        	{
            	fputc('2', fp);
            	fputc('5', fp);
            	fputc('4', fp);
        	}
        	else
        	{
        		fputc('0', fp);
        	}
		}
	}

	fclose(fp);
}

Img** csv_to_imgs(char* file_string, int number_of_imgs) {
	FILE *fp;
	Img** imgs = malloc(number_of_imgs * sizeof(Img*));
	char row[MAXCHAR];
	fp = fopen(file_string, "r");

	// Read the first line 
	fgets(row, MAXCHAR, fp);
	int i = 0;
	while (feof(fp) != 1 && i < number_of_imgs) {
		imgs[i] = malloc(sizeof(Img));

		int j = 0;
		fgets(row, MAXCHAR, fp);
		char* token = strtok(row, ",");
		imgs[i]->img_data = matrix_create(28, 28);
		while (token != NULL) {
			if (j == 0) {
				imgs[i]->label = atoi(token);
			} else {
				imgs[i]->img_data->entries[(j-1) / 28][(j-1) % 28] = atoi(token) / 256.0;
			}
			token = strtok(NULL, ",");
			j++;
		}
		i++;
	}
	fclose(fp);
	return imgs;
}

void img_print(Img* img) {
	matrix_print(img->img_data);
	printf("Img Label: %d\n", img->label);
}

void img_free(Img* img) {
	matrix_free(img->img_data);
	free(img);
	img = NULL;
}

void imgs_free(Img** imgs, int n) {
	for (int i = 0; i < n; i++) {
		img_free(imgs[i]);
	}
}