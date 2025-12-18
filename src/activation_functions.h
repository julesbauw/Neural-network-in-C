#ifndef ACTIVATION_FUNCTION
#define ACTIVATION_FUNCTION


typedef double (*ActivationFunction) (double);

double sign_function(double z);

double sigmoid(double z);

#endif