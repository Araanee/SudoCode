#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include "../utils/auxiliary.h"
#include "../grid/grid_detection.h"

// Resize image as per id.jpeg.
void resize_jpeg(int id)
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

    save_image(resize_surface(load_image(file_name), 28, 28), file_name);
}

// Resize all image as per id.jpeg with 1 <= id < 89.
void resize_all()
{
    printf("Resize: starting resizing process of all images...\n");

	for (int id = 1; id < 89; ++id)
	{
        if (id % 10 == 9 && id != 9)
        {
            continue;
        }
		resize_jpeg(id);
	}

    printf("Resize: done!\n");
}

void post_rot_resize(char* path)
{
    SDL_Surface* src_surface = load_image(path);
    int src_width = src_surface->w;
    int src_height = src_surface->h;

    // int left_x = x_left(src_surface);
    // int right_x = x_right(src_surface);
    // int top_y = y_topp(src_surface);
    // int bottom_y = y_bottomm(src_surface);

    // int width = right_x - left_x;
    // int height = bottom_y - top_y;

    // SDL_Rect src_rect;
    // src_rect.x = left_x;
    // src_rect.y = top_y;
    // src_rect.w = src_width;
    // src_rect.h = src_height;

    save_image(resize_surface(load_image(path),
        src_width*2, src_height*2), path);

    printf("Resize: done!\n");
}