#ifndef IMPORT_EXPORT_H
#define IMPORT_EXPORT_H

// import a grid file into an array
void import_grid(const char *file, int array[][9]);

// export a grid array into a file
void export_grid(char *file, int array[9][9]);

#endif