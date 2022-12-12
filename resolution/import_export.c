#include <stdio.h>
#include <stdlib.h>
#include <err.h>

/*
void array_to_file(const char* path, int grid[81])
{
	FILE* fp = fopen(file_string, "w");
	
	for (int i = 0; i < 82; ++i)
	{

	}
}
*/

void import_grid(const char *file, int array[][9])
{
	FILE *f = NULL;

	f = fopen(file, "r");
	
	if (f != NULL)
    {
    	// The file exists and is readable
    	size_t i=0, j=0;
		char curr_char;
        
        do
        {
            curr_char = fgetc(f); // read char

            if (curr_char == EOF)
            	break;

            if (curr_char == '.')
            {
            	array[i][j] = 0;
            	j++;
            	continue;
            }
            if (curr_char == ' ')
            {	
            	continue;
            }
            if (curr_char == '\n')
            {
            	if (  j==0)
            		continue;
            	i++;
            	j = 0;
            	continue;
            }

            array[i][j] = curr_char - '0';
            j++;

        } while (curr_char != EOF);

        fclose(f);
    }
    else
    {
        errx(EXIT_FAILURE, "File could not be imported, check that it exists.");
    }

}


void export_grid(char *file, int array[][9])
{
	FILE *f = NULL;

	f = fopen(file,"w");

	if (f == NULL)
		errx(EXIT_FAILURE, "File could not be created.");

	for (size_t i=0; i<9; i++)
	{
		for (size_t j=0; j<9; j++)
		{
			if (j == 3 || j == 6)
				fputc(' ',f);
			if (array[i][j] == 0)
				fputc('.',f);
			else
				fputc(array[i][j]+'0',f);
		}
		fputc('\n',f);
		if (i == 2 || i == 5)
			fputc('\n',f);
	}

	fclose(f);
}


/*
int main(int argc, char **argv)
{
	if (argc)
		printf("%s","");

	int array[][9] = 
	{
		{1,2,3,4,5,6,7,8,9},
		{0,1,2,3,4,5,6,7,1},
		{9,0,1,2,3,4,5,6,7},
		{2,9,0,1,2,3,4,5,6},
		{7,8,9,0,1,2,3,4,5},
		{6,7,8,9,0,1,2,3,4},
		{5,6,7,8,9,0,1,2,3},
		{4,5,6,7,8,9,0,1,2},
		{7,4,5,6,7,8,9,0,1},

	};
	
	char *filename = "grid_00.txt";
	export_grid(filename,array);

	int arr[9][9];
	import_grid(argv[1],arr);

	for (size_t i=0; i<9; i++)
	{
		for (int j=0; j<9; j++)
		{
			printf("%d ",arr[i][j]);
		}
		printf("%s\n","\n");
	}
	
	return EXIT_SUCCESS;
}*/
