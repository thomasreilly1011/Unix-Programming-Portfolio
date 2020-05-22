/*
Written by Thomas Reilly <18483722> thomas.reilly@ucdconnect.ie.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

char** parsestring(char* str);

char* removenewline(char* str);

char* getprompt();

void change_directory(char *path);

void execute(char *command, char **args);

void catchsigint(int signo);

void redirect(char *file_name);

int check_redirect(char** args);