#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrix.h"
#include "benchmark.h"
#include "neural_network.h"
#include "image.h"
#include "list.h"



void neural_netwerk_test(){
    srand(time(NULL));

    NeuralNetwork* nn =  create_neural_network(0.01,28 * 28,10,500,sigmoid);

    Matrix* x = create_matrix(28 * 28,1);
    matrix_set(x,0,0,0.1);
    Matrix* y = neural_network_apply(nn,x);

    matrix_print(y);

    free_matrix(x);
    free_matrix(y);
    free_neural_network(nn);
}



void image_test(char* file_name) {
    LinkedList* list = read_images_from_file(file_name,100);

    for (size_t i = 0; i < list->size; i++)
    {
        Image* im = (Image*) linked_list_get(list,i)->data;

        print_image(im);
    }
    
}


int main(int argc,char* argv[]) {
    // set the seed
    if (argc < 1) {
        return 1;
    }

    char* file_name = argv[1];

    image_test(file_name);
}




