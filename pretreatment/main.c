#include <stdlib.h>
#include <err.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../utils/auxiliary.h"
#include "rotation.h"
#include "auto_rotation.h"
#include "delete_color.h"
#include "grayscale.h"
#include "gaussian.h"
#include "gamma.h"
#include "threshold.h"

int main(int argc, char** argv)
{
    // Checks the number of arguments.
    if (argc < 3)
        errx(EXIT_FAILURE, "Usage: image_file process args\n");

    if (strcmp(argv[2], "gray") == 0)
    {
        int grayscale = surface_to_grayscale(argv[1]);
        if (grayscale)
        {
            printf("Main: grayscale done with no errors.\n");
            return EXIT_SUCCESS;
        }
        return EXIT_FAILURE;
    }

    if (strcmp(argv[2], "gauss") == 0)
    {
        int gaussian = gaussian_blur(argv[1]);
        if (gaussian)
        {
            printf("Main: gauss blur done with no errors.\n");
            return EXIT_SUCCESS;
        }
        return EXIT_FAILURE;
    }

    if (strcmp(argv[2], "gamma") == 0)
    {
        int gamma = surface_to_gamma(argv[1], 0.623);
        if (gamma)
        {
            printf("Main: gamma increase done with no errors.\n");
            return EXIT_SUCCESS;
        }
        return EXIT_FAILURE;
    }

    if (strcmp(argv[2], "bin") == 0)
    {
        int binary = filter_threshold(argv[1]);
        if (binary)
        {
            printf("Main: binarization done with no errors.\n");
            return EXIT_SUCCESS;
        }
        return EXIT_FAILURE;
    }

    if (strcmp(argv[2], "rotation") == 0)
    {
        int rot = rotate_by_angle(argv[1], strtol(argv[3], NULL, 10));
        if (rot)
        {
            printf("Main: rotation done with no errors.\n");
            return EXIT_SUCCESS;
        }
        return EXIT_FAILURE;
    }

    if (strcmp(argv[2], "auto-rotation") == 0)
    {
        int auto_rot = auto_rotate(argv[1]);
        if (auto_rot + 1)
        {
            printf("Main: auto-rotation done with no errors.\n");
            return EXIT_SUCCESS;
        }
        return EXIT_FAILURE;
    }

    if (strcmp(argv[2], "all-no-rotate") == 0)
    {
        int grayscale = surface_to_grayscale(argv[1]);
        if (grayscale)
        {
            int gaussian = gaussian_blur("gray_image.jpeg");
            if (gaussian)
            {
                int gamma = surface_to_gamma("gau_image.jpeg", 0.623);
                if (gamma)
                {
                    int binary = filter_threshold("gam_image.jpeg");
                    if (binary)
                    {
                        printf("Main: processes done with no errors.\n");
                        return EXIT_SUCCESS;
                    }
                    return EXIT_FAILURE;
                }
                return EXIT_FAILURE;
            }
            return EXIT_FAILURE;
        }
        return EXIT_FAILURE;
    }

    if (strcmp(argv[2], "all-man-rotate") == 0)
    {
        int grayscale = surface_to_grayscale(argv[1]);
        if (grayscale)
        {
            int gaussian = gaussian_blur("gray_image.jpeg");
            if (gaussian)
            {
                int gamma = surface_to_gamma("gau_image.jpeg", 0.623);
                if (gamma)
                {
                    int binary = filter_threshold("gam_image.jpeg");
                    if (binary)
                    {
                        int rot = rotate_by_angle("bin_image.jpeg",
                            strtol(argv[3], NULL, 10));
                        if (rot)
                        {
                            printf("Main: processes done with no errors.\n");
                            return EXIT_SUCCESS;
                        }
                        return EXIT_FAILURE;
                    }
                    return EXIT_FAILURE;
                }
                return EXIT_FAILURE;
            }
            return EXIT_FAILURE;
        }
        return EXIT_FAILURE;
    }

    if (strcmp(argv[2], "all-auto-rotate") == 0)
    {
        int grayscale = surface_to_grayscale(argv[1]);
        if (grayscale)
        {
            int gaussian = gaussian_blur("gray_image.jpeg");
            if (gaussian)
            {
                int gamma = surface_to_gamma("gau_image.jpeg", 0.623);
                if (gamma)
                {
                    int binary = filter_threshold("gam_image.jpeg");
                    if (binary)
                    {
                        int auto_rot = auto_rotate("bin_image.jpeg");
                        if (auto_rot + 1)
                        {
                            printf("Main: processes done with no errors\n");
                            return EXIT_SUCCESS;
                        }
                        return EXIT_FAILURE;
                    }
                    return EXIT_FAILURE;
                }
                return EXIT_FAILURE;
            }
            return EXIT_FAILURE;
        }
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}