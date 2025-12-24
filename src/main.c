#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrix.h"
#include "benchmark.h"
#include "neural_network.h"
#include "image.h"
#include "list.h"
#include <string.h>


#define USAGE_ERROR "Usage error, use command ./main [train|test] neural_network_file data_file\n"


NeuralNetwork* train(char* file_name) {

    int batches = 2000;
    int epochs = 20;

    Image** batch = read_images_from_file(file_name,BATCH_SIZE * batches);

    printf("batch succesfully readed\n");

    srand(time(NULL));

    NeuralNetwork* nn =  create_neural_network(0.01,28 * 28,10,500,sigmoid);

    printf("network created!\n");

    for (size_t ep = 0; ep < epochs; ep++)
    {
        /* code */
        double avg_loss = 0;
        for (size_t i = 0; i < batches; i++)
        {
            /* code */
            avg_loss += neural_network_train(nn,batch + i * BATCH_SIZE);
        }
        
        printf("epoch %ld loss: %f\n",ep,avg_loss);
    }
    
    
    for (size_t i = 0; i < BATCH_SIZE; i++)
    {
        free_image(batch[i]);
        batch[i] = NULL;
    }
    
    free(batch);    

    return nn;
}



void test(char* file_name,NeuralNetwork* nn) {
    int batches = 200;
    int epochs = 15;

    Image** batch = read_images_from_file(file_name,10000);

    printf("test data succesfully readed!\n");

    double correct = 0;

    for (size_t i = 0; i < 10000; i++)
    {
        Image* data = batch[i];

        Matrix* prediction = neural_network_forward(nn,data->input);

        int guess = arg_max(prediction);
        int real = arg_max(data->label);

        if (i % 100 == 0) printf("guess:%d , %d\n",guess,real);

        if (real == guess) {
            correct += 1.0;
        }
    }
    
    printf("%f correct!\n",correct / 10000);
    
    for (size_t i = 0; i < 10000; i++)
    {
        free_image(batch[i]);
        batch[i] = NULL;
    }
    
    free(batch);   
}


int main(int argc,char* argv[]) {
    // set the seed
    if (argc < 4) {
        fprintf(stderr,USAGE_ERROR);
        exit(1);
    }

    char* action = argv[1];

    char* neural_network_file = argv[2];

    char* data_file = argv[3];
    if (strcmp(action,"train") == 0) {
        if (argc < 3) {
            fprintf(stderr,USAGE_ERROR);
            exit(1);
        }

        NeuralNetwork* nn =  train(data_file);
        
        neural_network_save(nn,neural_network_file);

        free_neural_network(nn);

    }
    else if (strcmp(action,"test") == 0) {

        NeuralNetwork* nn = neural_network_load(neural_network_file);

        test(data_file,nn);
        free_neural_network(nn);
    }
    else {
        fprintf(stderr,USAGE_ERROR);
        exit(1);
    }

    return 0;
}




