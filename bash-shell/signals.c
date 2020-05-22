#include "shell.h"

/*
If SIGINT is caught this function is run.
The response is to create a new line and flush the output stream.
*/
void catchsigint(int signo) 
{
    if (signo == SIGINT) 
    {
        printf("\n");
        fflush(stdout);
    }
}