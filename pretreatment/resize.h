#ifndef RESIZE_H
#define RESIZE_H

// Resize image as per id.jpeg.
void resize_jpeg(int id);

// Resize all image as per id.jpeg with 1 <= id < 89.
void resize_all();

// Post image rotation resizing.
void post_rot_resize(char* path);

#endif