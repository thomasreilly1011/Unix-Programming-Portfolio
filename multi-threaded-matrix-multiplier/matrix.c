/*
Written by Thomas Reilly <18483722> thomas.reilly@ucdconnect.ie.
Matrix utility functions.
These are available to both the master and the worker.
*/
#include "matrix.h"

/*
Rerturns a iteratively generated square, float matrix of size n * n.
Floats are generated iteratively from 0 to (n*n)-1, row by row.
*/
float **generateMatrix(int n) {
    //int (*matrix)[n] = malloc((sizeof *matrix) * n);
    float **matrix = createEmptyMatrix(n, n);
    int count=0;
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            //matrix[i][j] = rand() % 21;
            matrix[i][j] = count;
            count++;
        }
    }
    return matrix;
}

/*
Creates an empty integer matrix of size m*n
*/
float **createEmptyMatrix(int n, int m) {
    float *values = calloc(m*n, sizeof(int));
    float **matrix = malloc(n*sizeof(int*));
    for (size_t i=0; i<n; ++i)
    {
        matrix[i] = values + i*m;
    }
    return matrix;
}

/*
Prints out the rows of given matrix M.
*/
void printMatrix(float **M, int rows, int columns) {

    for (size_t i = 0; i < rows; i++){
        printf("[");
        for (int j = 0; j < columns; j++){
            if (j == columns-1) {
                printf("%f", M[i][j]);
            } else {
                printf("%f, ", M[i][j]);
            }
        }
        printf("]\n");
    }
    
}

/*
Adds the values of part into destination starting from the row indicated in dest_pos.
It is assumed that part and dest have the same number of columns.
part_rows and part_cols should be the number of rows and columns, respectively, in the matrix part.
*/
float **matrix_concat(float **part, float **dest, int part_rows, int part_cols, int dest_pos) {
    size_t pi=0, pj=0;
    for (size_t i = dest_pos; i < dest_pos+part_rows; i++, pi++)
    {
        pj=0;
        for (size_t j = 0; j < part_cols; j++, pj++)
        {
            dest[i][j] = part[pi][pj];
        }
    }
    return dest;
}

/*
Sends a matrix to a socket represented by the given file descriptor 'cfd'.
*/
void writeMatrix(float **M, int cfd, int rows, int cols) {
    float *buffer = malloc(sizeof(float));
    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < cols; j++)
        {
            *buffer = M[i][j];
            send(cfd, buffer, sizeof(float), MSG_EOR);
        }
    }
}

/*
Reads a matrix from a socket represented by the given file descriptor 'cfd'.
*/
float **readMatrix(int cfd, int rows, int cols) {
    float **out = createEmptyMatrix(rows, cols);
    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < cols; j++)
        {
            float *buffer;
            recv(cfd, buffer, sizeof(float), MSG_EOR);
            out[i][j] = *buffer;
        }
    }
    return out;
}