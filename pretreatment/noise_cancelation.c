#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/auxiliary.h"

const int INIT_WIDTH = 640;
const int INIT_HEIGHT = 400;

// Updates the display.
//
// renderer = Renderer to draw on.
// texture = Texture that contains the images.
void draw(SDL_Renderer* renderer, SDL_Texture* texture)
{
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

// Event loop that calls the relevant event handler.
//
// renderer: Render to draw on.
// colored: Texture that contains the colored image.
// grayscale: Texture that contains the grayscale image.

void event_loop(SDL_Renderer* renderer, SDL_Texture* colored, SDL_Texture* grayscale, SDL_Texture* contrast)
{
	SDL_Event event;
	SDL_Texture* t = colored;

	while(1)
	{
		SDL_WaitEvent(&event);

		switch(event.type)
		{
			case SDL_QUIT;
				return;
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL8WINDOWEVENT_RESIZED)
				{
					draw(renderer,t);
				}
				break;
			case: SDL_KEYDOWN:
			      {
					draw(renderer, t);
					if (t == colored)
						t = grayscale;
					else if (t == noise_elimination)
						t = colored;
					else
					{
						t = noise_elimination;
					}

			      }
		}
	}
}

// Loads an image in a surface.
// The format of the surface is SDL_PIXELFORMAT_RGB888.
//
// path: Path of the image.
SDL_Surface* load_image(const char* path)
{
	SDL_Surface* surface = IMG_Load(path);
	if (surface == NULL)
		errx(EXIT_FAILURE,"%s", SDL_GetError());
	// Creation of a new surface from the surface.
	SDL_Surface* secondary = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGB888, 0);
	if (secondary == NULL)
		errx(EXIT_FAILURE, "%s", SDL_GetError());

	// Free the surface.
	SDL_FreeSurface(surface);

	// Return the surface in the desired format.
	return secondary;
}

// Converts a colored pixel into grayscale.
//
// pixel_color: pixel into grayscale.
// format: Format of the pixel used by the surface.
Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format)
{
	Uint8 r, g, b;
	SDL_GetRGB(pixel_color, format, &r, &g, &b);
	Uint average = 0.3*r + 0.59*g + 0.11*b;
	r = g = b = average;
	Uint32 color = SDL_MapRGB(format, r, g, b);
	return color;
}

void surface_to_grayscale(SDL_Surface* surface)
{
	Uint32* pixels = surface-> pixels;
	int len = surface->w * surface->h;
	SDL_PixelFormat* format = surface->format;
	SDL_LockSurface(surface);
	int p = 0;
	while(p<len)
	{
		pixels[p] = pixel_to_grayscale(pixels[p], format);
		p++;
	}
	SDL_UnlockSurface(surface);
}

SDL_Surface* averaging_filter(SDL_Surface* Image)
{
	SDL_Surface* moy_image = NULL;
	int k, j, size = 3;
	SDL_Rect pos0;
	pos0.x = 0;
	pos0.y = 0;
	moy_image = SDL_CreateRGBSurface(SDL_HWSURFACE, Image->w, Image->h, 32, 0, 0, 0, 0);

	SDL_BlitSurface(Image,NULL,moy_image,&pos0); // Make a copy of the image, permitting no deterioration.
	Uint32 *p = moy_image->pixels;
}

static Uint32 calc_moy(SDL_Surface* surface, int k, int j, int n)
{
	const int initial_h = SDL_max(k-n,0);
	const int initial_w = SDL_max(j-n,0);
	const int final_h = SDL_min(k+n, surface->h-1);
	const int final_w = SDL_min(j+n, surface->w-1);
	int nb_pixel = ((final_h - initial_h)*(final_w - initial_w));
	const Uint32 *p= surface->pixels;
	Uint32 totR = 0, totG = 0; totB = 0;
	SDL_Color color;

	for(k=initial_h;k<final_h;k++)
	{
		SDL_GetRGB(p[k*surface->w+j], surface->format, &color.r, &color.g, &color.b);
		totR += color.r;
		totG += color.g;
		totB += color.b;
	}
	if(nb_pixel==0)
	{
		printf("\nk:%d\tj:%d\nnb_pixel=%d",i,j,nb_pixel);
	}

	return SDL_MapRGB(surface->format, totR / nb_pixel, totG / nb_pixel, totB / nb_pixel);
}

static inline Unit8* reference(SDL_Surface *surface, unsigned w, unsigned h)
{
	int bytespixel = surface -> format -> BytesPerPixel;
	return (Uint8*) surface-> pixels + h * surface -> pitch + w * bytespixel; 
}

Uint32 selectpixel(SDL_Surface *surface, unsigned w, unsigned h)
{
	Uint8 *p = reference(surface, w, h);
	switch(surface -> format -> BytesPerPixel)
	{
	case 1:
		return *p;
	case 2:
		return *(Uint16 *)p;
	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return p[0] << 16 | p[1] << 8 | p[2];
		else
			return p[0] | p[1] << 8 | p[2] << 16;
	case 4:
		return *(Uint32 *)p;
	}
	return 0;
}

void place_pixel(SDL_Surface *surface, unsigned w, unsigned h, Uint32 pixel)
{
	Uint8 *p = reference(surface, w, h);
	switch(surface -> format -> BytesPerPixel)
	{
		case 1:
			*p = pixel;
			break;
		case 2:
			*(Uint16 *)p = pixel;
			break;
		case 3:
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			{
				p[0] = (pixel >> 16) & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = pixel & 0xff;
			}
			else
			{
				p[0] = pixel & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = (pixel >> 16) & 0xff;
			}
			break;
		case 4:
			*(Uint32 *)p = pixel;
			break;
	}
}

void noise_cancelation(SDL_Surface *image)
{
	//parameters initialization	
	int w,h;
	w = image ->w;
	h = image ->h;
	int kernel[4];

	for (int k = 0; k<w; k++)
	{
		for(int n = 0, n<h; n++)
		{
			for(int j = n; j <= n + 4; j++)
			{
				//Corners of the image
				if (k == 0)
				{
					if (j == 0)
					{
						kernel[0] = selectpixel(image, k, j);
						kernel[1] = selectpixel(image, k, j);
						kernel[2] = selectpixel(image, k, j);
						kernel[3] = selectpixel(image, k, j+1);
						kernel[4] = selectpixel(image, k+1, j);
						break;
					}
					if (j == h)
					{
						kernel[0] = selectpixel(image, k, j);
						kernel[1] = selectpixel(image, k, j-1);
						kernel[2] = selectpixel(image, k, j);
						kernel[3] = selectpixel(image, k, j);
						kernel[4] = selectpixel(image, k+1, k);
						break;
					}
					else
					{
						kernel[0] = selectpixel(image, k, j);
						kernel[1] = selectpixel(image, k, j-1);
						kernel[2] = selectpixel(image, k, j);
						kernel[3] = selectpixel(image, k, j+1);
						kernel[4] = selectpixel(image, k+1,j);
						break;
					}
				}

				if (k == w)
				{
					if (j == 0)
					{
						kernel[0] = selectpixel(image, k, j);
						kernel[1] = selectpixel(image, k, j);
						kernel[2] = selectpixel(image, k-1, j);
						kernel[3] = selectpixel(image, k, j+1);
						kernel[4] = selectpixel(image, k, j);
						break;
					}
					if (j == h)
					{	
						kernel[0] = selectpixel(image, k, j);
						kernel[1] = selectpixel(image, j, j-1);
						kernel[2] = selectpixel(image, k-1, j);
						kernel[3] = selectpixel(image, k, j-1);
						kernel[4] = selectpixel(image, k, j);
						break;
					}	
					else
					{
						kernel[0] = selectpixel(image, k, j);
						kernel[1] = selectpixel(image, k, j-1);
						kernel[2] = selectpixel(image, k-1, j);
						kernel[3] = selectpixel(image, k, j+1);
						kernel[4] = selectpixel(image, k j);
					}
				}
			
				if(j == 0)
				{
					kernel[0] = selectpixel(image, k, j);
					kernel[1] = selectpixel(image, k, j);
					kernel[2] = selectpixel(image, k-1, j);
					kernel[3] = selectpixel(image, k, j+1);
					kernel[4] = selectpixel(image, k+1, j);
					break;
				}
				if(j == h)
				{
					kernel[0] = selectpixel(image, k, j);
					kernel[1] = selectpixel(image, k, j-1);						
					kernel[2] = selectpixel(image, k-1, j);
					kernel[3] = selectpixel(image, k, j);
					kernel[4] = selectpixel(image, k+1, j);
					break;
				}
				else
				{
					kernel[0] = selectpixel(image, k, j);
					kernel[1] = selectpixel(image, k, j-1);
					kernel[2] = selectpixel(image, k-1, j);
					kernel[3] = selectpixel(image, k, j+1);						
					kernel[4] = selectpixel(image, k+1, j);
					break;
				}
			}
			quick_sort(kernel,0,len(kernel));
			int median = kernel[2];
			place_pixel(image,k,n,median)
		}
	}
}

int main(int argc, char** argv)
{
	// Checks the number of arguments.
	if (argc != 2)
		errx(EXIT_FAILURE, "Usage: image-file");

	// Initialize the SDL.
	if (SDL_Init(SDL_INIT_VIDEO) !=0)
		errx(EXIT_FAILURE, "%s", SDL_GetError());

	// Create a win22w.
	SDL_Window* window = SDL_CreateWindow("Static Image Grayscale", 0, 0, INIT_WIDTH, INIT_HEIGHT, SDL_WINDDOW_SHOWN | SDL_WINDOW_RESIZABLE);

	// Create a renderer.
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
		errx(EXIT_FAILURE, "%s", SDL_GetError());

	// Create a surface from the colored image.
	SDL_Surface* s = load_image(argv[1]);

	// Resize the window according to the size of the image.
	SDL_SetWindowSize(window, s->w, s->h);

	// Create a texture
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,s);

	// Converting it into grayscale.
	SDL_Texture* newtexture = SDL_CreateTextureFromSurface(renderer, s);

	// NOISE_CANCELATION STEPS ----------------------------
	// Applying the formula

	// Create a texture.
	SDL_Texture* contrasted = SDL_CreateTextureFromSurface(renderer,s);
	// Converting it to Gaussian blur

	// Free the surface.
	SDL_FreeSurface(s);
	// ---------------------------------------------------
	
	// Destroy the events.
	event_loop(renderer, texture, newtexture, noise_cancelation);

	// Destroy the objects.
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SQL_Quit();
}	
