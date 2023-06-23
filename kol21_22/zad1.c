#include <stdio.h>
#include <stdlib.h>
typedef struct _matrix
{
    int rows;
    int cols;
    double **data;

} Matrix;

int create_matrix(Matrix *pmatrix, int rows, int cols)
{
    if (rows < 1 || cols < 1)
        return 0;
    pmatrix->cols = cols;
    pmatrix->rows = rows;
    pmatrix->data = malloc(rows * sizeof(double *));
    for (int i = 0; i < rows; i++)
    {
        pmatrix->data[i] = calloc(cols, sizeof(double));
    }

    return 1;
}
int dump_matrix(Matrix *pmatrix)
{
    for (int i = 0; i < pmatrix->rows; i++)
    {
        for (int j = 0; j < pmatrix->cols; j++)
        {
            printf("%.2lf ", (pmatrix->data)[i][j]);
        }
        printf("\n");
    }
}
int get(Matrix *pmatrix, int row, int col, double *pvalue)
{
    if (pmatrix->data == NULL || pmatrix->rows < row || pmatrix->cols < col || col < 0 || row < 0)
    {
        return 0;
    }
    *pvalue = (pmatrix->data)[row][col];
    return 1;
}
int set(Matrix *pmatrix, int row, int col, double val)
{
    if (pmatrix->data == NULL || pmatrix->rows < row || pmatrix->cols < col || col < 0 || row < 0)
    {
        return 0;
    }
    (pmatrix->data)[row][col] = val;
    return 1;
}

void create_identity_matrix(Matrix *pm, int size)
{
    pm->cols = size;
    pm->rows = size;
    (pm->data) = malloc(sizeof(double) * size);
    for (int i = 0; i < size; i++)
    {
        pm->data[i] = calloc(size, sizeof(double));
    }
    for (int i = 0; i < size; i++)
    {
        (pm->data)[i][i] = (double)(1.0);
    }
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
        result[i] = dot_prod_vect(pm->data[i], vect, pm->cols);
    }
}
void max_element_in_rows(double *maxdata, const Matrix *pm)
{
    for (int i = 0; i < pm->rows; i++)
    {
        maxdata[i] = (double)(0);
        for (int j = 0; j < pm->cols; j++)
        {

            if (maxdata[i] < (pm->data)[i][j])
            {
                maxdata[i] = (pm->data)[i][j];
            }
        }
    }
}

int main()
{
    Matrix pmatrix;
    int rows;
    int cols;
    double pvalue;
    scanf("%d %d", &rows, &cols);
    create_matrix(&pmatrix, rows, cols);
    dump_matrix(&pmatrix);
    printf("\n");
    set(&pmatrix, 2, 3, 9.9);
    set(&pmatrix, 0, 1, 4);
    set(&pmatrix, 2, 1, 7.5);
    dump_matrix(&pmatrix);
    get(&pmatrix, 2, 3, &pvalue);
    printf("%.3lf\n", pvalue);

    Matrix pm;
    create_identity_matrix(&pm, 5);
    dump_matrix(&pm);

    printf("%.2lf\n", dot_prod_vect(pmatrix.data[0], pmatrix.data[2], pm.cols));

    double *v = malloc(sizeof(double) * (pmatrix.cols));
    for (int i = 0; i < pmatrix.cols; i++)
    {
        scanf("%lf", &v[i]);
    }
    double *res = malloc(sizeof(double) * (pmatrix.rows));
    mul_vect(res, &pmatrix, v);
    for (int i = 0; i < pmatrix.rows; i++)
    {
        printf("%lf ", res[i]);
    }
    printf("\n");

    max_element_in_rows(v, &pmatrix);
    for (int i = 0; i < pmatrix.rows; i++)
    {
        printf("%lf\n", v[i]);
    }
}