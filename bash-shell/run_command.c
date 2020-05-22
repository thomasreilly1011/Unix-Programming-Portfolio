/*
Written by Thomas Reilly <18483722> thomas.reilly@ucdconnect.ie.
*/
#include "shell.h"

/*
Changes the directory to the path given. 
If the path given is NULL the directory is changed to the home directory.
Input: Path of the directory to change to.
Output: void.
*/
void change_directory(char *path) 
{
    if (path == NULL) 
    {
        chdir(getenv("HOME"));
    } else if (chdir(path) == -1) 
    {
        //Error Handling
        perror("Directory Changing Error");
        exit(1);
    }
}

/*
A function that creates a new process to execute the given command with given arguments.
Input:  A string representing the command to be executed
        An array of strings holding the command and the arguments.
Output: void.
*/
void execute(char *command, char **args) 
{
    __pid_t child_pid;
    int child_status;
    int filepos;

    child_pid = fork();
    if (child_pid == 0) 
    {
        if ((filepos = check_redirect(args)) >= 0) 
        {
            redirect(args[filepos]);
        }
        execvp(command, args);
        printf("Unknown command\n");
        exit(1);
    } else 
    {
        wait(&child_status);
    }
    fflush(stdout);
}