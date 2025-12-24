#include "loss_functions.h"
#include <math.h>


double MSE(Matrix* y, Matrix* y_hat) {
    /*
    We assume that y nd y_hat have the same shape
    */
    double sum = 0;

    for (size_t i = 0; i < y->r; i++)
    {
        for (size_t j = 0; j < y->c; j++)
        {
            sum += pow(matrix_get(y,i,j) - matrix_get(y_hat,i,j),2);
        }
        
    }
    
    return 0.5 * sum;
}

Matrix* MSE_d(Matrix* y, Matrix* y_hat) {
    /*
    y and y_hat should have the same shape!!!!
    */
    Matrix* MSE_d = create_matrix(y->r,y->c);
    for (size_t i = 0; i < y->r; i++)
    {
        for (size_t j = 0; j < y->c; j++)
        {
            matrix_set(MSE_d,i,j,-(matrix_get(y,i,j) - matrix_get(y_hat,i,j)));
        }
        
    }
    
    return MSE_d;
}