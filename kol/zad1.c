#include <stdio.h>
#include <stdlib.h>
#include <math.h>
typedef struct _matrix
{
    int rows;
    int cols;
    double *data;

} Matrix;

void free_matrix(Matrix **pmatrix)
{
    for (int i = 0; i < (*pmatrix)->rows; i++)
    {
        for (int j = 0; j < (*pmatrix)->cols; j++)
        {
            free(&((*pmatrix)->data)[i * ((*pmatrix)->cols) + j]);
        }
    }
    free(*pmatrix);
    pmatrix = NULL;
}
Matrix *create_matrix(int rows, int cols)
{
}

Matrix *random_matrix(int rows, int cols)
{
    Matrix *new_matrix = create_matrix(rows, cols);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            double x = (double)(rand()) / (double)(RAND_MAX);
            new_matrix->data[i * cols + j] = x;
        }
    }
    return new_matrix;
}
double dot_prod_vect(const double *v1, const double *v2, int size)
{
    double sum = 0;
    for (int i = 0; i < size; i++)
    {
        sum = sum + (v1[i] * v2[i]);
    }
    return sum;
}

void mul_vect(double *result, const Matrix *pm, const double *vect)
{
    for (int i = 0; i < pm->rows; i++)
    {
        result[i] = dot_prod_vect(&((pm->data)[i * (pm->cols)]), vect, pm->cols);
    }
}
void print_row(double *row, size_t n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%lf ", row[i]);
    }
}
int glob_columns;
int compar(const void *a, const void *b){
    int *v1=(int *)a;
    int *v2=(int *)b;
    int sum1=0;
    int sum2=0;
    for(int i=0;i<glob_columns;i++){
        sum1+=v1[i];
        sum2+=v2[i];

    }
    return sum1-sum2;
}
void find_row(Matrix M,double key){
    int i=0;

    for(int i=0;i<1000;i++){
        
    }
    bsearch((void*) &key,M.data,M.rows,M.cols,compar);
}
int main()
{
}