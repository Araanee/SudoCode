#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include "../utils/auxiliary.h"
#include "Neural.h"
#include "DataLoad.h"
#include "Persist.h"
#include "MathTools.h"

int is_blank(double* n)
{
    double  white = 0;

    for (int i = 0; i < NUM_INPUTS; i++)
    {
        if (n[i] == 1.0f)
        {
            white++;
        }
    }

    if (((white * 100) / NUM_INPUTS) < 1)
    {
        return 1;
    }

    return 0;
}

int predict_number(char* path)
{
    double* format = calloc(NUM_INPUTS, sizeof(double));
    LoadImage(path, format, 0);
    
    
    if (is_blank(format))
    {
        printf("Neural Network: blank image detected, predicted number 0.\n");
        return 10;
    }


    LoadNeuralPath("ocr_neural_network/trained_neural.txt");
    char detected = FeedForwardChar(format);
    free(format);
    printf("Neural Network: predicted number %i.\n", detected - '0');

    if (atoi(&detected) >= 10)
    {
	    return 10;
    }

    return atoi(&detected);
}

// Predict number in image as per id.jpeg.
int predict_jpeg(int id)
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

    //printf("%s\n", file_name);
    int prediction = predict_number(file_name);
    //printf("coucou\n");

    return prediction;
}

// Predict number in all images as per id.jpeg with 1 <= id < 89.
int* predict_all()
{
    int* grid = calloc(81, sizeof(int));
    //printf("%i\n",grid[21]);
    int predicted = 0;
    int i = 0;

    printf("Prediction: starting prediction process of all images...\n");

	for (int id = 1; id < 89; ++id)
    {
        if (id % 10 == 9 && id != 9)
        {
            continue;
        }
        //printf("%i\n", i);
	predicted = predict_jpeg(id);
        grid[i] = predicted;

        //printf("predict : %i\n", predicted);

      //  printf("grid: %i\n", grid[i]);
	++i;
	}
    //printf("elemt2 %i \n", grid[2]);
    //size_t n = sizeof(grid)/sizeof(grid[0]);
   // printf("size : %li \n", n);
    return grid;
}
