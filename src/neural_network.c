#include "neural_network.h"
#include <stdio.h>
#include <stdlib.h>



NeuralNetwork* create_neural_network(double learning_rate,int input, int output, int hidden,ActivationFunction activation_function) {
    NeuralNetwork* nn = malloc(sizeof(NeuralNetwork));

    if (!nn) {
        return NULL;
    }

    int weight_amount = 2;
    nn->weight_amount = weight_amount;
    nn->weights = malloc(sizeof(Matrix*) * weight_amount); //TODO now our network only works with 1 hidden layer
    
    nn->input = input;
    nn->hidden = hidden;
    nn->output = output;
    nn->learning_rate = learning_rate;
    nn->activation_function = activation_function;

    nn->weights[0] = create_matrix(hidden,input);
    matrix_randomize(nn->weights[0],input + hidden);

    nn->weights[1] = create_matrix(output,hidden);
    matrix_randomize(nn->weights[1],hidden + output);

    return nn;

}

void free_neural_network(NeuralNetwork* nn) {
    if (!nn) {
        return;
    }

    for (size_t i = 0; i < nn->weight_amount; i++)
    {
        free_matrix(nn->weights[i]);
        nn->weights[i] = NULL;
    }

    free(nn->weights);

    free(nn);
    nn = NULL;

}


Matrix* neural_network_apply(NeuralNetwork* nn,Matrix* x) {
    Matrix* output = copy_matrix(x);

    for (size_t i = 0; i < nn->weight_amount; i++)
    {
        Matrix* new_output = matrix_mul(nn->weights[i],output);

        free_matrix(output);
        output = new_output;
        matrix_map(output,nn->activation_function);

    }
    
    return output;
}