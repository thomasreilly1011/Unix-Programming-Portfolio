/*
Written by Thomas Reilly <18483722> thomas.reilly@ucdconnect.ie.
This file contains the source code for the master program.
The master takes two integer values n and p as input.
2x matrices, A and B (of size n*n) are generated and multiplied together by splitting A into p horizontal slices.
Each of these horizontal slices, along with a copy of B, are sent to a worker client that multiplied.
The results of each worker client are gatered together by the master to make the matrix, the product of A and B.
*/
#define _DEFAULT_SOURCE
#include <netdb.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <pthread.h>

#define PORTNUM "49998"
#define BACKLOG 10
#define BUFSIZE 256

#include "matrix.h"

void setUpSocketServer();
float** createPartition();
void handle_clients();
void *handleWorker(void* input);

/* Matricies */
float** A;
float** B;
float** C;

/* Input variables */
int p;
int n;
int slice_rows;

/* Socket variables */
int lfd;

int main(int argc, char *argv[])  {
   //Process the input variables.
   if (argc < 3) {
      printf("Too few arguments for master: %d\n", argc-1);
      exit(EXIT_FAILURE);
   } else if (argc > 3) {
      printf("Too many arguments for master: %d\n", argc-1);
      exit(EXIT_FAILURE);
   }
   n = atoi(argv[1]);
   p = atoi(argv[2]);
   printf("n: %d\n", n);
   printf("p: %d\n", p);
   
   //Make matrices A and B of iterative values from 0 to (n*n)-1
   //Also makes an empty matrix C.
   A = generateMatrix(n);
   B = generateMatrix(n);
   C = createEmptyMatrix(n, n);
   
   //Print them to the console.
   printf("Matrix A\n");
   printMatrix(A, n, n);
   printf("Matrix B\n");
   printMatrix(B, n, n);
   printf("\n");

   setUpSocketServer();

   handle_clients();

   //Print the final result of matrix C.
   printf("\nAll calculations are complete.\n");
   printf("Matrix C:\n");
   printMatrix(C, n, n);
}

/*
Performs the setup of the socket server saving the file descriptor in global variable lfd.
*/
void setUpSocketServer() {
   struct addrinfo hints;
   struct addrinfo *result, *rp;

   memset(&hints, 0, sizeof(struct addrinfo));
   hints.ai_canonname = NULL; 
   hints.ai_addr = NULL;
   hints.ai_next = NULL; 
   hints.ai_family = AF_INET; 
   hints.ai_socktype = SOCK_STREAM;
   hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;

   if (getaddrinfo("127.0.0.1", PORTNUM, &hints, &result) != 0) {
      printf("Error getting addess");
      exit(-1);
   }

   int optval = 1;
   for (rp = result; rp != NULL; rp = rp->ai_next) {
      lfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

      if (lfd == -1) {
         continue;   /* On error, try next address */
      }

      if (setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) {
         printf("Error in binding");
         exit(-1);
      }

      if (bind(lfd, rp->ai_addr, rp->ai_addrlen) == 0) {
         break; /* Success */
      }
      /* bind() failed, close this socket, try next address */
      close(lfd);
   }

   if (rp == NULL) {
      printf("RP == NULL exiting\n");
      exit(-1);
   } else {
      char host[NI_MAXHOST];
      char service[NI_MAXSERV];
      if (getnameinfo((struct sockaddr *)rp->ai_addr, rp->ai_addrlen,host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
         fprintf(stdout, "Listening on (%s, %s)\n", host, service);
      } else if (getnameinfo((struct sockaddr *)rp->ai_addr, rp->ai_addrlen,host, NI_MAXHOST, service, NI_MAXSERV, 0) == EAI_AGAIN) {
         printf("The name could not be resolved at this time.  Try again later.\n");
         exit(-1);
      } else {
         printf("Error getting name info\n");
         exit(-1);
      }
   }

   freeaddrinfo(result);

   if (listen(lfd, BACKLOG) == -1) {
      printf("Listening error\n");
      exit(-1);
   }

}
/*
Handle the creation of threads for p client workers.
*/
void handle_clients() {
   pthread_t tid;

   for (int i=0; i<p; i++)
   {
      int cfd = accept(lfd, NULL, NULL);
      if (cfd == -1) {
         continue;
      }

      printf("\nWorker found!\n");

      int args[2];
      args[0] = cfd;
      args[1] = i;

      pthread_create(&tid, NULL, handleWorker, args);
   }

   pthread_join(tid, NULL);
}

/*
Handles a specific worker thread corresponding to the clients file descriptor given in the input array at index 0.
Also in the input array is the partition index at index 1 (i.e a number from 0 to p-1 representing the partition of A that the worker is handling.)
*/
void *handleWorker(void* input) {
   //Process the input array.
   int* args = (int*)input;
   int cfd = args[0];
   int partition_index = args[1];

   //Create the slice.
   printf("Creating partition %d for the worker:\n", partition_index);
   float** slice = createPartition(partition_index);
   printMatrix(slice, slice_rows, n);

   //Write dimensions to the worker
   int *buffer = &n;
   send(cfd, buffer, sizeof(int), MSG_EOR);
   buffer = &slice_rows;
   send(cfd, buffer, sizeof(int), MSG_EOR);

   //Write the slice and B matrix to the worker.
   writeMatrix(slice, cfd, slice_rows, n);
   writeMatrix(B, cfd, n, n);

   //Read and print the result from the worker once it is ready.
   float** slice_out = readMatrix(cfd, slice_rows, n);
   printf("Result from the worker:\n");
   printMatrix(slice_out, slice_rows, n);

   //Add the result to C.
   printf("Adding this to Matrix C\n");
   C = matrix_concat(slice_out, C, slice_rows, n, partition_index*slice_rows);

   pthread_exit(NULL);
   return NULL;
}

/*
Creates a horizontal partition of matrix A starting at position (partition_index * slice_rows).
*/
float **createPartition(int partition_index) {
   
   slice_rows = n/p;
   size_t ai, aj;
   float **slice = createEmptyMatrix(slice_rows, n);
   ai=partition_index*slice_rows;
   for (size_t si=0;si<slice_rows; si++, ai++) {
      //Cycle through slice rows
      aj=0;
      for (size_t sj=0; sj<n; sj++, aj++) {
            //Cycle through slice elements
            slice[si][sj] = A[ai][aj];
      }
   }
   return slice;
}
