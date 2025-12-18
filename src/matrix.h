#ifndef MATRIX
#define MATRIX
#include <stdlib.h>


typedef struct
{
    double* elements;
    int r; // #rows
    int c; // #colums
} Matrix;


Matrix* create_matrix(int r,int c);

void free_matrix(Matrix* m);

Matrix* copy_matrix(Matrix* m);

Matrix* matrix_transpose(Matrix* m);

int matrix_set(Matrix* m,int r,int c,double x);

double matrix_get(Matrix* m,int r,int c);

void matrix_fill(Matrix* m, double x);

void matrix_print(Matrix* m);

void matrix_map(Matrix* m, double (*f) (double));

void matrix_randomize(Matrix* m,int n);
// operations 

Matrix* matrix_mul(Matrix* A,Matrix* B);

Matrix* matrix_mul_vector(Matrix* A,Matrix* B);


Matrix* matrix_add(Matrix* A,Matrix* B);

Matrix* matrix_sub(Matrix* A,Matrix* B);

Matrix* matrix_mulc(Matrix* A,double B);

Matrix* matrix_addc(Matrix* A,double B);

Matrix* matrix_subc(Matrix* A,double B);

#endif