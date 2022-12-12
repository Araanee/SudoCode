#ifndef PREDICTING_H
#define PREDICTING_H

#define NUM_INPUTS 784
#define NUM_HIDDEN_NODES 300
#define NUM_OUTPUTS 10
#define NUM_TRAINING_SETS 330017
#define NUM_TEST_SETS 10000

int is_blank(double* n);

int predict_number(char* path);

// Predict number in image as per id.jpeg.
int predict_jpeg(int id);

// Predict number in all images as per id.jpeg with 1 <= id < 89.
int* predict_all();

#endif
