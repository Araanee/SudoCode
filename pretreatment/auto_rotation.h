#ifndef AUTO_ROTATION_H
#define AUTO_ROTATION_H

// Find out hough transform angle.
double hough_transform(const char* path);

// Launch automatic rotation.
int auto_rotate(const char* path);

#endif