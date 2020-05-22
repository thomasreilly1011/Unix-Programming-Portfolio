/*
Written by Thomas Reilly <18483722> thomas.reilly@ucdconnect.ie.
This file contains the source code for the worker program.
The worker takes in 1 argument, the ipaddress / host name of the computer running the master.
The worker makes a connection with the master server, reads in the relevant matrix data, performs the calculation and writes the result back to the master.
*/
#define _DEFAULT_SOURCE /* For NI_MAXHOST and NI_MAXSERV */
#include <netdb.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <errno.h>

#define PORTNUM "49998"    /* Port number for server */
#define BACKLOG 10
#define BUFSIZE 256

#include "matrix.h"

float **multiplyslicebymatrix(float **slice, float **B);
float multiplyRowbyCol(int row, int col, float **slice, float **B);
void connectToMaster();

/* The hostname / ip address of the master */
char* host; 

/* The socket file descriptor for connection to master */
int cfd;

/*
The M*N dimension of the slice recieved from the master.
N also corresponds to the N*N dimension of the B matrix from the master. 
*/
int M, N;

/*
The main worker function.
    Works to complete its multiplication task.
    Connects to the master.
    Submits its results to the master.
*/
int main(int argc, char *argv[]) {
    //Process the input variables.
    if (argc > 1) {
      printf("Too many arguments for worker: %d\n", argc-1);
      exit(EXIT_FAILURE);
   }
    if (argc == 0) {
        host = "127.0.0.1";
    } else {
        host = argv[1];
    }
    
    //Connect to the master
    connectToMaster();

    //Revieve dimensions
    int *buffer = malloc(sizeof(int));
    printf("Recieving data...\n");
    recv(cfd, buffer, sizeof(int), MSG_EOR);
    N = *buffer;
    recv(cfd, buffer, sizeof(int), MSG_EOR);
    M = *buffer;
    printf("n: %d, m: %d\n", N, M);

    //Recieve matrices
    float **slice_in = readMatrix(cfd, M, N);
    float **B = readMatrix(cfd, N, N);

    printf("Got the following Matrices from master:\n");
    printf("Slice: \n");
    printMatrix(slice_in, M, N);
    printf("B: \n");
    printMatrix(B, N, N);

    //Perform calculations
    float **slice_out = multiplyslicebymatrix(slice_in, B);
    printf("\nMultiplication Complete, result is as follows\n");
    printMatrix(slice_out, M, N);
    printf("Sending result to master\n");

    //Write the results back to the master.
    writeMatrix(slice_out, cfd, M, N);
}

/*
Multiplys the given horizontal slice by the corresponding columns.
Returns the result slice of this multiplication.
*/
float **multiplyslicebymatrix(float **slice, float **B) {
    float **slice_out = createEmptyMatrix(M, N);

    //We go though each of 'slice's rows
    //For each row we multiply it by every column in b to get a new element.
    for (size_t i=0; i<M; i++) {
        for (size_t j = 0; j<N; j++)
        {
            slice_out[i][j] = multiplyRowbyCol(i, j, slice, B);
        }
    }
    return slice_out;
}

/*
Multiplys the given row of slice by the given column of B
Returns the result of the multiplication.
*/
float multiplyRowbyCol(int row, int col, float **slice, float **B){
    int out = 0;
    for (size_t i = 0; i < N; i++)
    {
        out += slice[row][i] * B[i][col];
    }
    return out;
}

void connectToMaster() {
    struct addrinfo hints;
    struct addrinfo *result, *rp;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL; 
    hints.ai_addr = NULL;
    hints.ai_next = NULL; 
    hints.ai_family = AF_INET; 
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICSERV;

    if (getaddrinfo(host, PORTNUM, &hints, &result) != 0)
       exit(-1);

    for (rp = result; rp != NULL; rp = rp->ai_next) {
        cfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

        if (cfd == -1) {
            continue;   /* On error, try next address */
        }

        if (connect(cfd, rp->ai_addr, rp->ai_addrlen) != -1) {
            break; /* Success */
        }   

         /* close() failed, close this socket, try next address */
         close(cfd);
    }

    if (rp == NULL) {
        printf("Could not connect to master.\n");
        exit(-1);
    }
    printf("Connection with master made!\n");

    freeaddrinfo(result);
}
