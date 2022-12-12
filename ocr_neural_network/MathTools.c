#include "MathTools.h"

//return the image of x by the sigmoid function
double sigmoid(double x)
{
	return 1.0f / (1.0f + exp(-x));
}

double dSigmoid(double x)
{
	return x * (1.0f - x);
}

double ReLu(double x)
{
	if (x < 0)
		return 0;
	else
		return x;
}

//returns a random double between -1.0 and 1.0
double init_weight(int layerSize)
{
	double min = -1.0 / sqrt(layerSize);
	double max = 1.0 / sqrt(layerSize);

	double difference = max - min; // The difference between the two
	int scale = 10000;
	int scaled_difference = (int)(difference * scale);
	return min + (1.0 * (rand() % scaled_difference) / scale);
}

//returns a random double between 0.0 and 1.0
double init_bias()
{
	return ((double) rand() / ((double) RAND_MAX));
}

//initialize random seed with current time as seed
void init_random()
{
	time_t t;
	srand((unsigned)time(&t));
}