#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>
#include "math.h"

Matrix* create_matrix(int r,int c) {
    Matrix* m = malloc(sizeof(Matrix));

    if (!m) {
        return NULL;
    }

    double* elements = calloc(r * c,sizeof(double));

    if (!elements) {
        free(m);
        return NULL;
    }

    m->elements = elements;
    m->c = c;
    m->r = r;

    return m;
}

void free_matrix(Matrix* m) {
    if (!m) {
        return;
    }

    free(m->elements);
    m->elements = NULL;
    m->c = 0;
    m->r = 0;
    free(m);
}

int matrix_set(Matrix* m,int r,int c,double x) {
    int index = r * m->c + c;
    
    m->elements[index] = x;
    return 0;
}

double matrix_get(Matrix* m, int r, int c) {
    return m->elements[r * m->c + c];
}

int arg_max(Matrix* m) {
    int max = 0;
    int first = 1;
    double max_value = 0.0;

    for (size_t i = 0; i < m->c * m->r; i++)
    {
        if (first || m->elements[i] > max_value) {
            max = i;
            max_value = m->elements[i];
            first = 0;
        }
    }

    return max;
    
}


Matrix* matrix_transpose(Matrix* m) {
    Matrix* T = create_matrix(m->c,m->r);

    for (int r = 0; r < T->r; r++)
    {
        for (int c = 0; c < T->c; c++)
        {
            matrix_set(T,r,c,matrix_get(m,c,r));
        }
        
    }
    return T;

}


Matrix* copy_matrix(Matrix* m) {
    Matrix* cp = create_matrix(m->r,m->c);

    for (int r = 0; r < cp->r; r++)
    {
        for (int c = 0; c < cp->c; c++)
        {
            matrix_set(cp,r,c,matrix_get(m,r,c));
        }
        
    }
    return cp;
    
}


void matrix_fill(Matrix* m, double x) {
    for (int r = 0; r < m->r; r++)
    {
        for (int c = 0; c < m->c; c++)
        {
            matrix_set(m,r,c,x);
        }
        
    }
}

void matrix_print(Matrix* m) {
    for (int r = 0; r < m->r; r++)
    {
        for (int c = 0; c < m->c; c++)
        {
            printf("%lf ",matrix_get(m,r,c));
        }
        printf("\n");
    }
}

Matrix* matrix_map(Matrix* m, double (*f) (double)) {

    Matrix* B = copy_matrix(m);
    for (int r = 0; r < m->r; r++)
    {
        for (int c = 0; c < m->c; c++)
        {
            double new_e = f(matrix_get(m,r,c));
            matrix_set(B,r,c,new_e);
        }
        
    }
    return B;
}

double matrix_abs_sum(Matrix* m) {
    double sum = 0.0;

    for (int r = 0; r < m->r; r++)
    {
        for (int c = 0; c < m->c; c++)
        {
            double e = pow(matrix_get(m,r,c),2);

        }
        
    }

}

void matrix_map_into(Matrix* m, double (*f) (double)) {
    for (int r = 0; r < m->r; r++)
    {
        for (int c = 0; c < m->c; c++)
        {
            double new_e = f(matrix_get(m,r,c));
            matrix_set(m,r,c,new_e);
        }
        
    }
}

double uniform_distribution(double low,double high) {
    
    double diff = high - low;

    int scale = 10000;
    int scaled_diff = (int) (scale * diff);
    return low + (1.0 * (rand() % scaled_diff) / scale);

}

void matrix_randomize(Matrix* m,int n) {
    // Xavier uniform
    double min = -6.0 / sqrt(n);
    double max = 6.0 / sqrt(n);

    for (int r = 0; r < m->r; r++)
    {
        for (int c = 0; c < m->c; c++)
        {
            double new_e = uniform_distribution(min,max);
            matrix_set(m,r,c,new_e);
        }
        
    }
}

Matrix* matrix_mul_vector(Matrix* A,Matrix* B) {
    /*
    return new matrix A * B, B has to be a vector (n x 1) -> we return a new vector (A->r x 1)
    is a little bit faster
    */
    if (A->c != B->r) {
        fprintf(stderr, "Invalid matrix mul operation\n");
        exit(1);
    }
    
    Matrix* C = create_matrix(A->r,1);

    double* Ae = A->elements;
    double* Be = B->elements;
    double* Ce = C->elements;

    int k = A->c;

    for (int r = 0; r < C->r; r++)
    {
        double sum = 0.0;
        for (int i = 0; i < k; i++)
        {
            double* Arow = Ae + r * A->c;
            sum += Ae[i] * Be[i];
        }
        Ce[r] = sum;

    }
    return C;
}


Matrix* matrix_mul(Matrix* A,Matrix* B) {
    /*
    return new matrix A * B
    */
    if (A->c != B->r) {
        fprintf(stderr, "Invalid matrix mul operation (%d x %d) (%d x %d)\n",A->r,A->c,B->r,B->c);
        exit(1);
    }
    
    Matrix* C = create_matrix(A->r,B->c);

    double* Ae = A->elements;
    double* Be = B->elements;
    double* Ce = C->elements;

    int k = A->c;

    for (int r = 0; r < C->r; r++)
    {
        for (int c = 0; c < C->c; c++)
        {
            double el = 0;

            for (int i = 0; i < k; i++)
            {
                el += Ae[r * A->c + i] * Be[i * B->c + c];
            }
            Ce[r * C->c + c] = el;
        }
        
    }
    return C;
}

Matrix* matrix_add(Matrix* A,Matrix* B) {
    /*
    return new matrix A + B
    */
    if (A->c != B->c || A->r != B->r) {
        fprintf(stderr, "Invalid matrix add operation\n");
        exit(1);
    }

    Matrix* C = create_matrix(A->r,A->c);

    double* Ae = A->elements;
    double* Be = B->elements;
    double* Ce = C->elements;

    for (int r = 0; r < C->r; r++)
    {
        for (int c = 0; c < C->c; c++)
        {
            double el = Ae[r * A->c + c] + Be[r * B->c + c];
            Ce[r * C->c + c] = el;
        }
        
    }
    return C;
}

Matrix* matrix_sub(Matrix* A,Matrix* B) {
    /*
    return new matrix A - B
    */
    if (A->c != B->c || A->r != B->r) {
        fprintf(stderr, "Invalid matrix sub operation\n");
        exit(1);
    }

    Matrix* C = create_matrix(A->r,A->c);

    double* Ae = A->elements;
    double* Be = B->elements;
    double* Ce = C->elements;

    for (int r = 0; r < C->r; r++)
    {
        for (int c = 0; c < C->c; c++)
        {
            double el = Ae[r * A->c + c] - Be[r * B->c + c];
            Ce[r * C->c + c] = el;
        }
        
    }
    return C;

}

Matrix* matrix_mulc(Matrix* A,double B) {
    Matrix* C = create_matrix(A->r,A->c);

    double* Ae = A->elements;
    double* Ce = C->elements;

    for (int r = 0; r < C->r; r++)
    {
        for (int c = 0; c < C->c; c++)
        {
            double el = Ae[r * A->c + c] * B;
            Ce[r * C->c + c] = el;
        }
        
    }
    return C;
}

Matrix* matrix_addc(Matrix* A,double B) {
    Matrix* C = create_matrix(A->r,A->c);
    
    double* Ae = A->elements;
    double* Ce = C->elements;

    for (int r = 0; r < C->r; r++)
    {
        for (int c = 0; c < C->c; c++)
        {
            double el = Ae[r * A->c + c] + B;
            Ce[r * C->c + c] = el;
        }
        
    }
    return C;
}

Matrix* matrix_subc(Matrix* A,double B) {
    return matrix_addc(A,-B);
}


Matrix* matrix_hadamard_product(Matrix* A,Matrix* B) {
    /*
    
    Hadamard product, A and B should have the same shape!!!
    */
    Matrix* C = create_matrix(A->r,A->c);


    double* Ae = A->elements;
    double* Be = B->elements;
    double* Ce = C->elements;

    for (int r = 0; r < C->r; r++)
    {
        for (int c = 0; c < C->c; c++)
        {
            Ce[r * C->c + c] = Ae[r * A->c + c] * Be[r * B->c + c];
        }
        
    }
    return C;
}


void matrix_addc_into(Matrix* A,double B) {
    
    double* Ae = A->elements;

    for (int r = 0; r < A->r; r++)
    {
        for (int c = 0; c < A->c; c++)
        {
            Ae[r * A->c + c] += B;
        }
        
    }
}

void matrix_subc_into(Matrix* A,double B) {
    matrix_addc_into(A,-B);
}

void matrix_mulc_into(Matrix* A,double B) {
    
    double* Ae = A->elements;

    for (int r = 0; r < A->r; r++)
    {
        for (int c = 0; c < A->c; c++)
        {
            Ae[r * A->c + c] *= B;
        }
        
    }
}



Matrix* matrix_add_into_A(Matrix* A,Matrix* B) {
    /*
    Adds B to A
    */


    double* Ae = A->elements;
    double* Be = B->elements;

    for (int r = 0; r < A->r; r++)
    {
        for (int c = 0; c < A->c; c++)
        {
            Ae[r * A->c + c] += Be[r * B->c + c];
        }
        
    }
}