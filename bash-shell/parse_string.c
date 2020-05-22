/*
Written by Thomas Reilly <18483722> thomas.reilly@ucdconnect.ie.
*/
#include "shell.h"

/*
Input: An input string from the shell
Output: An array of strings that appropriately divides the contents
of the input string into seperate tokens.
*/
char** parsestring(char* str) 
{
    char **output_str = malloc(sizeof(char*));
    int count=0;
    char* token = strtok(str, " ");
    
    
    //Go through all of the white-space seperated words in the input and add them to seperate elements of an array.
    while (token != NULL) 
    {
        output_str[count] = malloc(strlen(token) * sizeof(char));
        token[strlen(token)] = '\0';
        output_str[count] = token;
        count++;
        output_str = realloc(output_str, (count+1) * sizeof(char*));
        token = strtok(NULL, " ");
    }

    //Remove the unnesesary newline character from the last element in the array if it exists.
    output_str[count-1] = removenewline(output_str[count-1]);

    //Null terminate the last element and the array
    output_str[count] = NULL;
    
    return output_str;
}

/*
A function that removes any newline characters present in a given string.
Input: A string.
Output: An identical string to the input with the newline charater '\n' removed.
*/
char* removenewline(char* str) 
{
    char *newstr;
    int len = strlen(str);
    
    newstr = (char *) malloc((len-1)*sizeof(char));
    for(int i=0; i<strlen(str); i++) {
        if(str[i] == '\n') 
        {
            //Do nothing
        } else {
            newstr[i] = str[i];
            newstr[i+1] = '\0';
        }
    }
    

    return newstr;
}