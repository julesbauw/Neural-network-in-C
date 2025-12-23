#ifndef NEURALNET
#define NEURALNET
#include "matrix.h"
#include "activation_functions.h"
#include "list.h"
#include "loss_functions.h"
#include "image.h"

#define BATCH_SIZE 32



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

Matrix* neural_network_forward(NeuralNetwork* nn,Matrix* x);

void neural_network_train(NeuralNetwork* nn, Image** trainings_data);

void neural_network_save(NeuralNetwork* nn,char* file_name);

NeuralNetwork* neural_network_load(char* file_name);


#endif