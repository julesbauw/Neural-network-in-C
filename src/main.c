#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrix.h"
#include "benchmark.h"
#include "neural_network.h"
#include "image.h"
#include "list.h"



void test(char* file_name) {
    Image** batch = read_images_from_file(file_name,BATCH_SIZE);

    printf("batch succesfully readed\n");

    srand(time(NULL));

    NeuralNetwork* nn =  create_neural_network(0.01,28 * 28,10,500,sigmoid);

    printf("network created!\n");

    neural_network_train(nn,batch);

    neural_network_train(nn,batch);
    neural_network_train(nn,batch);
    neural_network_train(nn,batch);
    neural_network_train(nn,batch);


    free_neural_network(nn);
    
    for (size_t i = 0; i < BATCH_SIZE; i++)
    {
        free_image(batch[i]);
        batch[i] = NULL;
    }
    
    free(batch);    
}


int main(int argc,char* argv[]) {
    // set the seed
    if (argc < 1) {
        return 1;
    }

    char* file_name = argv[1];

    test(file_name);
}




