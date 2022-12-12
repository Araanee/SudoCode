#include <stdlib.h>
#include <stddef.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <string.h>

/*
** DEFINE directives.
*/

// Get Byte directive.
#define get_byte(value, n) (value >> (n*8) & 0xFF)

/*
** Array related functions.
*/

// Print an array.
void array_print(unsigned int arr[][90], size_t width)
{
    for (size_t x = 0; x < width; ++x)
    {
        for (size_t y = 0; y < 90; ++y)
        {
            printf("| %u", arr[x][y]);
        }
        printf("\n");
    }

    printf("\n");
}

void swap(int *a, int *b)
{
  int t = *a;
  *a = *b;
  *b = t;
}

// Function to find the partition position.
int partition(int array[], int low, int high)
{
  // Select the rightmost element as pivot
  int pivot = array[high];

  // Pointer for greater element
  int i = (low - 1);

  // Traverse each element of the array
  // Compare them with the pivot
  for (int j = low; j < high; j++)
  {
    if (array[j] <= pivot)
    {    
      // If element smaller than pivot is found
      // swap it with the greater element pointed by i
      i++;

      // Swap element at i with element at j
      swap(&array[i], &array[j]);
    }
  }

  // Swap the pivot element with the greater element at i
  swap(&array[i + 1], &array[high]);

  // Return the partition point
  return (i + 1);
}

// Quick sort an array.
void quick_sort(int array[], int low, int high)
{
  if (low < high)
  {
    // Find the pivot element such that
    // elements smaller than pivot are on left of pivot
    // elements greater than pivot are on right of pivot
    int pi = partition(array, low, high);
    
    // Recursive call on the left of pivot
    quick_sort(array, low, pi - 1);
    
    // Recursive call on the right of pivot
    quick_sort(array, pi + 1, high);
  }
}

// Function to print array elements.
void print_array(int array[], int size)
{
  for (int i = 0; i < size; ++i)
  {
    printf("%d  ", array[i]);
  }

  printf("\n");
}

// Converts int to string.
char* itoa(int val, int base)
{
    static char buf[32] = {0};
    int i = 30;

    for(; val && i ; --i, val /= base)
    {
        buf[i] = "0123456789abcdef"[val % base];
    }

    return &buf[i+1];
}

/*
** Math related functions.
*/

// Convert angle in degrees to its radian value.
double degrees_to_rad(double degrees)
{
    return degrees * (M_PI / 180);
}

// Find max of two ints.
int max(int a, int b)
{
    if (a > b)
    {
        return a;
    }

    return b;
}

// Lerp through s, e and t value.
float lerp(float s, float e, float t)
{
    return s + (e - s) * t;
}

// Blerp set of numbers.
float blerp(float c00, float c10, float c01, float c11, float tx, float ty)
{
    return lerp(lerp(c00, c10, tx), lerp(c01, c11, tx), ty);
}

// Squeeze number between extremums.
double squeeze(double d, double min, double max)
{
    const double t = d < min ? min : d;
    return t > max ? max : t;
}

/*
** SDL related auxiliary functions.
*/

// Load image to surface.
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

// Draw and render surface.
void draw(SDL_Renderer* renderer, SDL_Texture* texture)
{
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

// Save image to path.
void save_image(SDL_Surface *image_surface, char *path)
{
    // printf("Image Save: saving image to %s\n", path);
    int success = IMG_SavePNG(image_surface, path);

    if (success != 0)
    {
        errx(1, "Image Save: could not save the image to '%s': %s.\n", path,
            SDL_GetError());
    }
}

// Read surface's pixel from x and y coordinates.
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

// Write pixel in surface to x and y coordinates.
void SDL_WritePixel(SDL_Surface* surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8*) surface->pixels + y * surface->pitch + x * bpp;
 
    switch(bpp)
    { 
        case 1:
            *p = pixel;
            break;

        case 2:
            *(Uint16*) p = pixel;
            break;

        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
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
            *(Uint32*) p = pixel;
            break;
    }
}

// Crops image and returns a surface.
SDL_Surface* crop_surface(SDL_Surface* src_surface,
    int x, int y, int width, int height)
{
    SDL_Surface* surface = SDL_CreateRGBSurface(src_surface->flags, width,
        height, src_surface->format->BitsPerPixel, src_surface->format->Rmask,
        src_surface->format->Gmask, src_surface->format->Bmask,
        src_surface->format->Amask);
    SDL_Rect rect = {x, y, width, height};

    printf("Surface Crop: cropping surface...\n");
    SDL_BlitSurface(src_surface, &rect, surface, 0);
    printf("Surface Crop: done!\n");

    return surface;
}

SDL_Surface *resize_surface(SDL_Surface* src_surface,
    int x, int y)
{
    SDL_Surface *dest_surface = SDL_CreateRGBSurface(0, x, y, 32, 0, 0, 0, 0);
    SDL_BlitScaled(src_surface, NULL, dest_surface, NULL);

    return dest_surface;
}

// Scale image to 28x28.
void scale(SDL_Surface* src_surface)
{
    SDL_Surface* dest_surface;
    int new_width = 28;
    int new_height = 28;

    dest_surface = SDL_CreateRGBSurface(SDL_SWSURFACE, new_width, new_height,
        src_surface->format->BitsPerPixel, src_surface->format->Rmask,
        src_surface->format->Gmask, src_surface->format->Bmask,
        src_surface->format->Amask);

    SDL_FillRect(dest_surface,
        NULL, SDL_MapRGB(dest_surface->format, 255, 255, 255));

    if (dest_surface == NULL)
    {
        errx(EXIT_FAILURE,"%s", SDL_GetError());
    }

    SDL_LockSurface(src_surface);
    SDL_LockSurface(dest_surface);

    int x, y;

    for (x = 0, y = 0; y < new_height; x++)
    {
        if (x > new_width)
        {
            x = 0; 
            y++;
        }

        double gx = max(x / (double) (new_width) * (src_surface->w)
            - 0.5f, src_surface->w - 1);
        double gy = max(y / (double) (new_height) * (src_surface->h)
            - 0.5, src_surface->h - 1);
        int gxi = (int) gx;
        int gyi = (int) gy;

        Uint32 result = 0;
        Uint32 c00 = SDL_ReadPixel(src_surface, gxi, gyi);
        Uint32 c10 = SDL_ReadPixel(src_surface, gxi + 1, gyi);
        Uint32 c01 = SDL_ReadPixel(src_surface, gxi, gyi + 1);
        Uint32 c11 = SDL_ReadPixel(src_surface, gxi + 1, gyi+  1);
        Uint8 i;

        for (i = 0; i < 3; i++)
        {
            
            result |= (Uint8) blerp(get_byte(c00, i), 
                get_byte(c10, i), get_byte(c01, i),
                get_byte(c11, i), gx - gxi, gy -gyi) << (8*i);
        }

        SDL_WritePixel(dest_surface, x, y, result);
    }
    SDL_UnlockSurface(src_surface);
    SDL_UnlockSurface(dest_surface);

    *src_surface = *dest_surface;
    free(dest_surface);
}

// Crops image and returns 0, saves the cropped image as "cropped_image.jpeg".
int crop_image(SDL_Surface* src_surface, int x, int y, int width, int height)
{
    SDL_Surface* surface = SDL_CreateRGBSurface(src_surface->flags, width,
        height, src_surface->format->BitsPerPixel, src_surface->format->Rmask,
        src_surface->format->Gmask, src_surface->format->Bmask,
        src_surface->format->Amask);
    SDL_Rect rect = {x, y, width, height};
    
    printf("Image Crop: cropping image...\n");
    SDL_BlitSurface(src_surface, &rect, surface, 0);
    printf("Image Crop: done!\n");
    
    save_image(surface, "c_image.jpeg");

    return 1;
}

// Crops image and returns 0, saves the cropped image as "_id_.jpeg".
int crop_image_name(SDL_Surface* src_surface, int x, int y,
    int width, int height, int id, int is_number)
{
    SDL_Surface* surface = SDL_CreateRGBSurface(src_surface->flags, width,
        height, src_surface->format->BitsPerPixel, src_surface->format->Rmask,
        src_surface->format->Gmask, src_surface->format->Bmask,
        src_surface->format->Amask);
    SDL_Rect rect = {x, y, width, height};
    
    printf("Image Crop: cropping image...\n");
    SDL_BlitSurface(src_surface, &rect, surface, 0);
    printf("Image Crop: done!\n");

    char file_name[64] = {0};

    if (is_number)
    {
        file_name[0] = 'n';
        file_name[1] = 'u';
        file_name[2] = 'm';
        file_name[3] = 'b';
        file_name[4] = 'e';
        file_name[5] = 'r';
        file_name[6] = 's';
        file_name[7] = '/';
    }

    char* id_str = itoa(id, 10);
    strcat(file_name, id_str);
    if (is_number)
    {
        strcat(file_name, ".jpeg");
    }
    else
    {
        strcat(file_name, ".jpeg");
    }

    save_image(surface, file_name);

    return 1;
}

// Generate gray histogram.
int *image_grayscale_histogram(SDL_Surface* src_surface,
    int startx, int endx, int starty, int endy)
{
    Uint32* pixels = src_surface-> pixels;
    SDL_PixelFormat* format = src_surface->format;
    int *hist = calloc(sizeof(int), 256);
    Uint8 r, g, b;

    for (int y = starty; y < endy && y < src_surface->h; y++)
    {
        for (int x = startx; x < endx && x < src_surface->w; x++)
        {
            SDL_GetRGB(pixels[y * (src_surface->w) + x], format, &r, &g, &b);
            hist[r]++;
        }
    }

    return hist;
}

// Implement magic wand algorithm recursive manner.
void aux_magic_wand(SDL_Surface* src_surface, int x, int y)
{
    Uint32 white = SDL_MapRGB(src_surface->format, 255, 255, 255);
    Uint32 color = SDL_ReadPixel(src_surface, x, y);
    Uint8 r, g,b;
    SDL_GetRGB(color, src_surface->format, &r, &g, &b);

    if (r < 5 && g < 5 && b < 5) // True if black / probable noise.
    {
        SDL_LockSurface(src_surface);
        SDL_WritePixel(src_surface, x, y, white);
        SDL_UnlockSurface(src_surface);
        if (x - 1 >= 0)
        {
            aux_magic_wand(src_surface, x - 1, y);
        }

        if (x + 1 < src_surface->w)
        {
            aux_magic_wand(src_surface, x + 1, y);
        }

        if (y - 1 >= 0)
        {
            aux_magic_wand(src_surface, x, y - 1);
        }

        if (y + 1 < src_surface->h)
        {
            aux_magic_wand(src_surface, x, y + 1);
        }
    }
}

// Main function for the magic wand algorithm.
int magic_wand(char* path)
{
    SDL_Surface* src_surface = load_image(path);
    int src_width = src_surface->w;
    int src_height = src_surface->h;
    Uint32 color;

    for (int x = 0; x < src_width; ++x)
    {
        for (int y = 0; y < 5; ++y)
        {
            Uint8 r, g, b;
            color = SDL_ReadPixel(src_surface, x, y);
            SDL_GetRGB(color, src_surface->format, &r, &g, &b);
            if (r < 5 && g < 5 && b < 5) // True if black / probable edge.
            {
                aux_magic_wand(src_surface, x, 0);
            }
        }
    }

    for (int x = 0; x < src_width; ++x)
    {
        for (int y = 1; y < 6; ++y)
        {
            Uint8 r, g, b;
            color = SDL_ReadPixel(src_surface, x, src_height-y);
            SDL_GetRGB(color, src_surface->format, &r, &g, &b);
            if (r < 5 && g < 5 && b < 5) // True if black / probable edge.
            {
                aux_magic_wand(src_surface, x, src_height-y);
            }
        }
    }

    for (int y = 0; y < src_height; ++y)
    {
        for (int x = 0; x < 5; ++x)
        {
            Uint8 r, g, b;
            color = SDL_ReadPixel(src_surface, x, y);
            SDL_GetRGB(color, src_surface->format, &r, &g, &b);
            if (r < 5 && g < 5 && b < 5) // True if black / probable edge.
            {
                aux_magic_wand(src_surface, 0, y);
            }
        }
    }

    for (int y = 0; y < src_height; ++y)
    {
        for (int x = 1; x < 6; ++x)
        {
            Uint8 r, g, b;
            color = SDL_ReadPixel(src_surface, src_width-x, y);
            SDL_GetRGB(color, src_surface->format, &r, &g, &b);
            if (r < 5 && g < 5 && b < 5) // True if black / probable edge.
            {
                aux_magic_wand(src_surface, src_width-x, y);
            }
        }
    }

    save_image(src_surface, path);

    return 1;
}

// Reduce noise as per id.jpeg.
void magic_wand_id(int id)
{
    char file_name[64] = {0};

    int is_number = 1;

    if (is_number)
    {
        file_name[0] = 'n';
        file_name[1] = 'u';
        file_name[2] = 'm';
        file_name[3] = 'b';
        file_name[4] = 'e';
        file_name[5] = 'r';
        file_name[6] = 's';
        file_name[7] = '/';
    }

    char* id_str = itoa(id, 10);
    strcat(file_name, id_str);
    if (is_number)
    {
        strcat(file_name, ".jpeg");
    }
    else
    {
        strcat(file_name, ".jpeg");
    }

    magic_wand(file_name);
}

// Reduce noise of all images with 1 <= id < 89.
int magic_wand_all()
{
    printf("Magic Wand: starting magic wand process of all images...\n");

    for (int id = 1; id < 89; ++id)
    {
        if (id % 10 == 9 && id != 9)
        {
            continue;
        }
        magic_wand_id(id);
    }

    printf("Magic Wand: done!\n");

    return 1;
}

// Invert black and white of an image using path.
int invert_black_white(char* path)
{

    SDL_Surface* src_surface = load_image(path);
    int src_width = src_surface->w;
    int src_height = src_surface->h;
    Uint32 color;
    Uint32 white = SDL_MapRGB(src_surface->format, 255, 255, 255);
    Uint32 black = SDL_MapRGB(src_surface->format, 0, 0, 0);

    for (int y = 0; y < src_height; ++y)
    {
        for (int x = 0; x < src_width; ++x)
        {
            Uint8 r, g, b;
            color = SDL_ReadPixel(src_surface, x, y);
            SDL_GetRGB(color, src_surface->format, &r, &g, &b);
            if (r < 5 && g < 5 && b < 5)
            {
                SDL_WritePixel(src_surface, x, y, white);
            }
            else
            {
                SDL_WritePixel(src_surface, x, y, black);
            }
        }
    }

    save_image(src_surface, path);

    return 1;
}

// Invert black and white color using id of an image.
void invert_black_white_id(int id)
{
    char file_name[64] = {0};

    int is_number = 1;

    if (is_number)
    {
        file_name[0] = 'n';
        file_name[1] = 'u';
        file_name[2] = 'm';
        file_name[3] = 'b';
        file_name[4] = 'e';
        file_name[5] = 'r';
        file_name[6] = 's';
        file_name[7] = '/';
    }

    char* id_str = itoa(id, 10);
    strcat(file_name, id_str);
    if (is_number)
    {
        strcat(file_name, ".jpeg");
    }
    else
    {
        strcat(file_name, ".jpeg");
    }

    invert_black_white(file_name);
}

// Invert black and white colors of image with 1 <= id < 89.
int invert_black_white_all()
{
    printf("Color Invertion: starting color invertion of all images...\n");

    for (int id = 1; id < 89; ++id)
    {
        if (id % 10 == 9 && id != 9)
        {
            continue;
        }
        invert_black_white_id(id);
    }

    printf("Color Invertion: done!\n");

    return 1;
}
