#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../utils/auxiliary.h"
#include "../ocr_neural_network/predicting.h"
#include "../resolution/import_export.h"
void array_to_file(char* filename, int* prediction)
{
	int array[9][9];

	for (int i =0; i<9;i++){
		for (int j=0;j<9;j++){
			int pred = prediction[i*9+j];
			printf("%d\n", pred);
			if (pred==22)
				
				array[i][j]=2;
			else if (pred==10)
				array[i][j]=0;
			else
				array[i][j]=prediction[i*9+j];
			
		}
	}
	export_grid(filename,array);
	/*
	FILE* fp = fopen(filename, "w");
	
	for (int j=0; j<81; j++)
	{
		if (j% 3 == 0)
		{
			if (j != 0)
			{
				if (j % 9 == 0)
					fputc('\n', fp);
				else
					fputc(' ', fp);
			}
		}

		if (j == 27 || j == 54)
			fputc('\n', fp);

		if (prediction[j] == 10)
		{
			fputc('.', fp);
		}
		else
			fputs(itoa(*(prediction+j),10),fp); 
	}
	
	fputc('\n', fp);
	fclose(fp);
	printf("Reconstruction: Done !\n");
*/}
