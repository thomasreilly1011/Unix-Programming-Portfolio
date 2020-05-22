#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>

float** generateMatrix(int n);
float** createEmptyMatrix(int m, int n);
void printMatrix(float** M, int rows, int columns);
float** matrix_concat(float** part, float** dest, int part_rows, int part_cols, int dest_pos);
void writeMatrix(float** M, int cfd, int rows, int cols);
float** readMatrix(int cfd, int rows, int cols);
