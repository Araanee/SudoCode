#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include "../utils/auxiliary.h"

int rotate_by_angle(const char* path, double angle)
{
	SDL_Surface* src_surface = load_image(path);
	SDL_Surface* dest_surface;
	int src_height = src_surface->h;
	int src_width = src_surface->w;
	int m_x = (src_width / 2);
	int m_y = (src_height / 2);
	double angle_radian = degrees_to_rad(angle);
	double cos_s = cos(angle_radian);
	double sin_s = sin(angle_radian);
	double dest_width  = ceil(src_width * fabs(cos_s) 
		+ src_height * fabs(sin_s));
	double dest_height = ceil(src_width * fabs(sin_s) 
		+ src_height * fabs(cos_s));
	double rotate_x, rotate_y;
	Uint32 color;

	dest_surface = SDL_CreateRGBSurface(SDL_SWSURFACE, dest_width, dest_height, 
		src_surface->format->BitsPerPixel, src_surface->format->Rmask,
		src_surface->format->Gmask, src_surface->format->Bmask,
		src_surface->format->Amask);

	SDL_FillRect(dest_surface, 
		NULL, SDL_MapRGB(dest_surface->format, 255, 255, 255));

	if (dest_surface == NULL)
	{
		errx(EXIT_FAILURE,"%s", SDL_GetError());
	}

	int m_x_dest = dest_surface->w/2.;
	int m_y_dest = dest_surface->h/2.;

	SDL_LockSurface(src_surface);
	SDL_LockSurface(dest_surface);

	for (int y = 0; y < dest_height; ++y)
	{
		for (int x = 0; x < dest_width; ++x)
		{
			rotate_x = (ceil (cos_s * (x-m_x_dest) 
				+ sin_s * (y-m_y_dest) + m_x));
			rotate_y = (ceil (-sin_s * (x-m_x_dest) 
				+ cos_s * (y-m_y_dest) + m_y));

			if (0 <= rotate_x 
				&& rotate_x < src_width
				&& 0 <= rotate_y 
				&& rotate_y < src_height)
			{
				color = SDL_ReadPixel(src_surface, rotate_x, rotate_y);
				SDL_WritePixel(dest_surface, x, y, color);
			}
		}
	}

	SDL_UnlockSurface(src_surface);
	SDL_UnlockSurface(dest_surface);

	save_image(dest_surface, "rot_image.jpeg");

	return 1;
}