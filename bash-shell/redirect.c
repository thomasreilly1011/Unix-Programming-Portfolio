/*
Written by Thomas Reilly <18483722> thomas.reilly@ucdconnect.ie.
*/
#include "shell.h"

/*
A function that changes the output of stdout to a given file.
Input: The file name of the file you would like to redirect to.
Ouput: void.
*/
void redirect(char *file_name) 
{
    int f = open(file_name, O_CREAT | O_WRONLY | O_TRUNC, 0666);
    int dup = dup2(f, 1);

    //Error handling
    if (f < 0 || dup < 0) {
        perror("Redirect Error");
        exit(1);
    }

    close(f);
}

/*
Input: An array containg a console command and arguments.
Output: The index of the argument of that holds the filename.
        If there is no redirect argument -1 is returned.
*/
int check_redirect(char** args) 
{
    int i=0;

    while (args[i]!= NULL) 
    {
        if(args[i][0] == '>' && args[i][1] == '\0') 
        {
            if (args[i+1] == NULL) 
            {
                return -1;
            }
            return i+1;
        }
        i++;
    }
    return -1;
}