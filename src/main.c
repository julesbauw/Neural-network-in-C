#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrix.h"
#include "benchmark.h"
#include "neural_network.h"

int main() {
    // set the seed
    srand(time(NULL));

    NeuralNetwork* nn =  create_neural_network(0.01,1,1,1,sigmoid);

    Matrix* x = create_matrix(1,1);
    matrix_set(x,0,0,0.1);
    Matrix* y = neural_network_apply(nn,x);

    matrix_print(y);

    free_matrix(x);
    free_matrix(y);
    free_neural_network(nn);
}



