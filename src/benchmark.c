#include "benchmark.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "matrix.h"
#define LOOPS 9999999999999999

void benchmark() {

    
    /* here, do your time-consuming job */
    
    Matrix* A = create_matrix(2 ,2);

    Matrix* B = create_matrix(2 ,2);

    matrix_fill(A,2);
    matrix_set(A,0,0,7);
    matrix_fill(B,5);

    matrix_set(B,0,0,3);
    matrix_set(B,1,1,99);


    Matrix* C = matrix_mul(A,B);

    matrix_print(C);

    return;
    // clock_t begin = clock();

    
    
    
    // clock_t end = clock();
    // double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    
    // free_matrix(A);

    // printf("%lf\n",time_spent);
}