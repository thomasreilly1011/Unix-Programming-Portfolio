/*
Written by Thomas Reilly <18483722> thomas.reilly@ucdconnect.ie.
*/
#include "shell.h"

/*
A function that generates a prompt in the form '[dd/mm hh:mm] # '
This prompt displays the date and time in bold green font.
It then displays the prompt character # in a bold blue font.
Input:  void.
Ouput:  A string that holds the command.
*/
char* getprompt() 
{
    char* prompt;
    time_t t;
    struct tm *tm;
    
    prompt = malloc(sizeof(char) * 45);

    //Get time and convert to local time struct.
    t = time(NULL);
    tm = localtime(&t);

    //Format the time as desired.
    strftime(prompt, 45, "\033[1;32m[%d/%m %H:%M]\033[0;34m # \033[0m", tm);

    return prompt;
}