#ifndef LOSS_FUNCTIONS
#define LOSS_FUNCTIONS

#include "matrix.h"


double MSE(Matrix* y, Matrix* y_hat);


Matrix* MSE_d(Matrix* y, Matrix* y_hat);




#endif