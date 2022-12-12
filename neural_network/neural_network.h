#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

// Activation function and its derivative.
double sigmoid(double x);

double dSigmoid(double x);

// Shuffle the dataset.
void shuffle(int *array, size_t n);

// Activation function.
double init_weight();

int main (void);

#endif