#ifndef NEURALNET
#define NEURALNET
#include "matrix.h"
#include "activation_functions.h"



typedef struct {
    int input;
    int hidden;
    int output;
    int weight_amount;
    double learning_rate;
    ActivationFunction activation_function;

    Matrix** weights;

} NeuralNetwork;

NeuralNetwork* create_neural_network(double learning_rate,int input, int output, int hidden,ActivationFunction activation_function);

void free_neural_network(NeuralNetwork* nn);

Matrix* neural_network_apply(NeuralNetwork* nn,Matrix* x);



#endif