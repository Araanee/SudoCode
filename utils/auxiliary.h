#ifndef AUXILARY_H
#define AUXILARY_H

/*
** Array related functions.
*/

// Print an array.
void array_print(unsigned int arr[][90], size_t width);

// Swap two elements.
void swap(int *a, int *b);

// Function to find the partition position.
int partition(int array[], int low, int high);

// Quick sort.
void quick_sort(int array[], int low, int high);

// Function to print array elements.
void print_array(int array[], int size);

// Converts int to string.
char* itoa(int val, int base);

/*
** Math related functions.
*/

// Convert angle in degrees to its radian value.
double degrees_to_rad(double degrees);

// Find max of two ints.
int max(int a, int b);

// Lerp through s, e and t value.
float lerp(float s, float e, float t);

// Blerp set of numbers.
float blerp(float c00, float c10, float c01, float c11, float tx, float ty);

// Squeeze double between extremums.
double squeeze(double d, double min, double max);

/*
** SDL related auxiliary functions.
*/

// Load image to surface.
SDL_Surface* load_image(const char* path);

// Draw and render surface.
void draw(SDL_Renderer* renderer, SDL_Texture* texture);

// Save image to path.
void save_image(SDL_Surface *image_surface, char *path);

// Read surface's pixel from x and y coordinates.
Uint32 SDL_ReadPixel(SDL_Surface* surface, int x, int y);

// Write pixel in surface to x and y coordinates.
void SDL_WritePixel(SDL_Surface* surface, int x, int y, Uint32 pixel);

// Rescale using SDL gfx.
SDL_Surface* rescale(SDL_Surface* src_surface);

// Scale image to 28x28.
SDL_Surface *resize_surface(SDL_Surface* src_surface,
    int x, int y);

// Scale image to 28x28.
void scale(SDL_Surface* src_surface);

// Crops image and returns a surface.
SDL_Surface* crop_surface(SDL_Surface* src_surface,
    int x, int y, int width, int height);

// Crops image and returns 0, saves the cropped image as "cropped_image.jpeg".
int crop_image(SDL_Surface* sprite_sheet, int x, int y, int width, int height);

// Crops image and returns 0, saves the cropped image as "_id_.jpeg".
int crop_image_name(SDL_Surface* src_surface, int x, int y,
    int width, int height, int id, int is_number);

// Generate gray histogram.
int *image_grayscale_histogram(SDL_Surface* src_surface,
    int startx, int endx, int starty, int endy);

// Implement magic wand algorithm recursive manner.
void aux_magic_wand(SDL_Surface* src_surface, int x, int y);

// Main function for the magic wand algorithm.
int magic_wand(char* path);

// Reduce noise as per id.jpeg.
void magic_wand_id(int id);

// Reduce noise of all images with 1 <= id < 89.
int magic_wand_all();

// Invert black and white of an image using path.
int invert_black_white(char* path);

// Invert black and white color using id of an image.
void invert_black_white_id(int id);

// Invert black and white colors of image with 1 <= id < 89.
int invert_black_white_all();

#endif