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


Matrix* neural_network_forward(NeuralNetwork* nn,Matrix* x) {
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

void neural_network_train(NeuralNetwork* nn, Image** batch) {

    Matrix* W1_d = create_matrix(nn->hidden,nn->input);
    Matrix* W2_d = create_matrix(nn->output,nn->hidden);


    double sum_loss = 0;

    for (size_t i = 0; i < BATCH_SIZE; i++)
    {
        Image* img = batch[i];

        Matrix* input = img->input;
        Matrix* y = img->label;

        Matrix* hidden = matrix_mul(nn->weights[0],input);
        Matrix* hidden_act = copy_matrix(hidden);
        matrix_map(hidden_act,nn->activation_function);

        Matrix* output = matrix_mul(nn->weights[1],hidden_act);
        Matrix* y_hat = copy_matrix(output);
        matrix_map_into(y_hat,nn->activation_function);
        
        double loss = MSE(y,y_hat);

        sum_loss += loss;

        // output nodes
        Matrix* d_loss = MSE_d(y,y_hat);
        
        matrix_map_into(output,d_sigmoid);
        
        Matrix* delta = matrix_hadamard_product(d_loss,output);
        
        Matrix* transpose_hidden = matrix_transpose(hidden_act);
        
        Matrix* grad = matrix_mul(delta,transpose_hidden);

        // hidden nodes

        Matrix* W2_T = matrix_transpose(nn->weights[1]); // N2 x N0

        Matrix* temp = matrix_mul(W2_T,delta); // N2 x 1 matrix

        Matrix* d_s_t = matrix_map(hidden,d_sigmoid); // N2 x 1 matrix
        
        Matrix* delta2 = matrix_hadamard_product(d_s_t,temp); // N2 x 1 matrix

        Matrix* tr_input = matrix_transpose(input); // 1 x N1

        Matrix* grad2 = matrix_mul(delta2,tr_input); // N1 x N2

        matrix_add_into_A(W1_d,grad2);
        matrix_add_into_A(W2_d,grad);

        free_matrix(grad);
        free_matrix(grad2);
        free_matrix(W2_T);
        free_matrix(temp);
        free_matrix(d_s_t);
        free_matrix(delta2);
        free_matrix(tr_input);

        free_matrix(delta);
        free_matrix(transpose_hidden);
        free_matrix(d_loss);
        free_matrix(hidden);
        free_matrix(hidden_act);
        free_matrix(output);
        free_matrix(y_hat);

    }
    
    printf("%f\n",sum_loss / BATCH_SIZE);

    matrix_mulc_into(W1_d, - nn->learning_rate / BATCH_SIZE);
    matrix_mulc_into(W2_d, - nn->learning_rate / BATCH_SIZE);

    
    // update weights!
    matrix_add_into_A(nn->weights[0],W1_d);
    matrix_add_into_A(nn->weights[1],W2_d);
    

    free_matrix(W1_d);
    free_matrix(W2_d);

}

