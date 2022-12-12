#ifndef GRID_ROW_EXTRACTION_H
#define GRID_ROW_EXTRACTION_H

// Extract row from coordinates.
int extract_row(SDL_Surface* src_surface, int y, int src_width,
	int interval, int id);

// Extract all rows.
int grid_row_extract(const char* path);

#endif