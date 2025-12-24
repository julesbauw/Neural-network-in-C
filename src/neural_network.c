#include "neural_network.h"
#include <stdio.h>
#include <stdlib.h>


#define FREAD_SAFE(ptr, size, nmemb, file) \
    do { \
        size_t fread_error = fread(ptr, size, nmemb, file); \
        if (fread_error != (nmemb)) { \
            fprintf(stderr, "Error: fread failed (%zu/%zu)\n", fread_error, (size_t)(nmemb)); \
            exit(1); \
        } \
    } while(0)




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

double neural_network_train(NeuralNetwork* nn, Image** batch) {
    /*
    returns avg loss of the batch
    */

    Matrix* W1_d = create_matrix(nn->hidden,nn->input);
    Matrix* W2_d = create_matrix(nn->output,nn->hidden);


    double sum_loss = 0;

    for (size_t i = 0; i < BATCH_SIZE; i++)
    {
        Image* img = batch[i];

        Matrix* input = img->input;
        Matrix* y = img->label;

        Matrix* z1 = matrix_mul(nn->weights[0],input);
        Matrix* a1 = matrix_map(z1,nn->activation_function);

        Matrix* z2 = matrix_mul(nn->weights[1],a1);
        Matrix* y_hat = matrix_map(z2,nn->activation_function);
        
        double loss = MSE(y,y_hat);

        sum_loss += loss;

        // output nodes
        Matrix* d_loss = MSE_d(y,y_hat);
        
        matrix_map_into(z2,d_sigmoid);
        
        Matrix* delta = matrix_hadamard_product(d_loss,z2);
        
        Matrix* transpose_hidden = matrix_transpose(z1);
        
        Matrix* grad = matrix_mul(delta,transpose_hidden);

        // hidden nodes

        Matrix* W2_T = matrix_transpose(nn->weights[1]); // N2 x N0

        Matrix* temp = matrix_mul(W2_T,delta); // N2 x 1 matrix

        Matrix* d_s_t = matrix_map(z1,d_sigmoid); // N2 x 1 matrix
        
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
        free_matrix(a1);
        free_matrix(z1);
        free_matrix(z2);
        free_matrix(y_hat);

    }
    
    matrix_mulc_into(W1_d, - nn->learning_rate / BATCH_SIZE);
    matrix_mulc_into(W2_d, - nn->learning_rate / BATCH_SIZE);
    
    // printf("%f\n",matrix_abs_sum(W1_d));
    // printf("%f\n",matrix_abs_sum(W2_d));
    

    // update weights!
    matrix_add_into_A(nn->weights[0],W1_d);
    matrix_add_into_A(nn->weights[1],W2_d);

    free_matrix(W1_d);
    free_matrix(W2_d);

    return sum_loss / BATCH_SIZE;

}


// save and load network


void write_matrix(Matrix* m, FILE* fptr) {
    fwrite(&m->r,sizeof(int),1,fptr);
    
    fwrite(&m->c,sizeof(int),1,fptr);

    for (size_t i = 0; i < m->c * m->r; i++)
    {
        fwrite(&m->elements[i],sizeof(double),1,fptr);
    }
}

void neural_network_save(NeuralNetwork* nn,char* file_name) {
    FILE* fptr = fopen(file_name,"wb");
    
    if (!fptr) {
        perror("fopen");
        return;
    }


    fwrite(&nn->input,sizeof(int),1,fptr);
    
    fwrite(&nn->hidden,sizeof(int),1,fptr);

    fwrite(&nn->output,sizeof(int),1,fptr);

    fwrite(&nn->weight_amount,sizeof(int),1,fptr);

    fwrite(&nn->learning_rate,sizeof(double),1,fptr);

    for (size_t i = 0; i < nn->weight_amount; i++)
    {
        write_matrix(nn->weights[i],fptr);
    }

    fclose(fptr);
    
}

Matrix* read_matrix(FILE* fptr) {

    int r;
    FREAD_SAFE(&r,sizeof(int),1,fptr);

    int c;
    FREAD_SAFE(&c,sizeof(int),1,fptr);

    Matrix* m = malloc(sizeof(Matrix));

    if (!m) {
        return NULL;
    }

    m->elements = malloc(sizeof(double) * c * r);
    if (!m->elements) {
        free(m);
        return NULL;
    }
    m->c = c;
    m->r = r;

    for (size_t i = 0; i < r*c; i++)
    {
        FREAD_SAFE(&m->elements[i],sizeof(double),1,fptr);
    }

    return m;
    
}

NeuralNetwork* neural_network_load(char* file_name) {
    FILE* fptr = fopen(file_name,"rb");

    if (!fptr) {
        perror("fopen");
        return NULL;
    }

    NeuralNetwork* nn = malloc(sizeof(NeuralNetwork));

    if (!nn) {
        return NULL;
    }

    int input; 
    FREAD_SAFE(&input,sizeof(int),1,fptr);
    nn->input = input;

    int hidden; 
    FREAD_SAFE(&hidden,sizeof(int),1,fptr);
    nn->hidden = hidden;

    int output; 
    FREAD_SAFE(&output,sizeof(int),1,fptr);
    nn->output = output;    

    int weight_amount; 
    FREAD_SAFE(&weight_amount,sizeof(int),1,fptr);
    nn->weight_amount = weight_amount;

    double learning_rate; 
    FREAD_SAFE(&learning_rate,sizeof(double),1,fptr);
    nn->learning_rate = learning_rate;


    //TODO, a way to save multiple activation functions!! maybe add enum??
    nn->activation_function = sigmoid;

    nn->weights = malloc(sizeof(Matrix*) * weight_amount);

    for (size_t i = 0; i < weight_amount; i++)
    {
        nn->weights[i] = read_matrix(fptr);
    }

    fclose(fptr);


    return nn;
}

