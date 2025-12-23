#include "activation_functions.h"
#include <math.h>

double sign_function(double z) {
    /*
    returns 1 if z > 0, -1 else 

    */
    if (z <= 0) {
        return -1;
    }

    return 1;
}

double sigmoid(double z) {

    return (1 / (1 - exp(-z)));
}

double d_sigmoid(double z) {
    double sig = sigmoid(z);
    return sig * (1 - sig);
}