#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include "import_export.h"


int isAvailable(int sudoku[][9], int row, int col, int num)
{
    int rowStart = (row/3) * 3;
    int colStart = (col/3) * 3;
    int i;

    for(i=0; i<9; ++i)
    {
        if (sudoku[row][i] == num) return 0;
        if (sudoku[i][col] == num) return 0;
        if (sudoku[rowStart + (i%3)][colStart + (i/3)] == num) return 0;
    }
    return 1;
}

int solve(int sudoku[][9], int row, int col)
{
    int i;
    if(row<9 && col<9)
    {
        if(sudoku[row][col] != 0)
        {
            if((col+1)<9) return solve(sudoku, row, col+1);
            else if((row+1)<9) return solve(sudoku, row+1, 0);
            else return 1;
        }
        else
        {
            for(i=0; i<9; ++i)
            {
                if(isAvailable(sudoku, row, col, i+1))
                {
                    sudoku[row][col] = i+1;
                    if((col+1)<9)
                    {
                        if(solve(sudoku, row, col +1)) return 1;
                        else sudoku[row][col] = 0;
                    }
                    else if((row+1)<9)
                    {
                        if(solve(sudoku, row+1, 0)) return 1;
                        else sudoku[row][col] = 0;
                    }
                    else return 1;
                }
            }
        }
        return 0;
    }
    else return 1;
}


int Solve(char *filename)
{
    int array[9][9];
    import_grid(filename,array);

    int res = solve(array, 0, 0);

    char *file = malloc(14*sizeof(char));
    strcpy(file, filename);
    strcat(file, ".result");

    export_grid(file,array);

    printf("Resolution: Done !\n");
    return res;
}
