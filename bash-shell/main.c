/*
Written by Thomas Reilly <18483722> thomas.reilly@ucdconnect.ie.
A program that mimics the bash shell, taking in a commandline and creating a new process to run it.
Contains directory changing functionality. The command 'cd' followed by a path handles this.
In the event of a SIGINT, the signal is ignored.
If '>' followed by a file name is supplied in the commandline arguments, the output of that command is
redirected to the given file name, creating a new file if it does not yet exist.
Displays date and time in the command prompt (with nice pretty colours for bonus marks :) ).
*/
#include "shell.h"

int main (void) 
{
    char* in = malloc(sizeof(char));
    char** parstr;

    //Display welcome message and prompt.
    printf("\033[1;32mWelcome To My Shell!\n");
    printf("%s", getprompt());

    //Catch SIGINT
    if(signal(SIGINT, catchsigint) == SIG_ERR) 
    {
        printf("\nCant catch SIGINT\n");
    }
    
    //Start listening for commands
    while (fgets(in, 149, stdin) != NULL) 
    {
        parstr = parsestring(in);

        //If the command given is 'cd' change the directory.
        if (parstr[0][0] == 'c' && parstr[0][1] == 'd' && parstr[0][2] == '\0') 
        {
            change_directory(parstr[1]);
        } else {
            //Otherwise execute the system command.
            execute(parstr[0], parstr);
        }
        //Display the prompt for the next command.
        printf("%s", getprompt());
    }

    return 0;
}