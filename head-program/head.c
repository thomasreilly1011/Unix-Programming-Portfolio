/*
Written by Thomas Reilly (18483722) thomas.reilly@ucdconnect.ie
A program that mimics that of head by printing the first n lines of a given file.
Given option e or o it will print only the even or odd lines respectively.

Program works:
Returns the first n lines of a file.
The odd and even functionality works.
The version option works returning the version number and my details.
If the help option is called or the program is called with invalid arguments inscructions
are given to the user on how to corrctly use the program.
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

const int MAX_LEN = 4095;

int main(int argc, char *argv[]) {
	int opt, nlines, nflags;
	_Bool even, odd;
	char *fileName = malloc(255 * sizeof(char));
	char *line = malloc(MAX_LEN * sizeof(char));
	FILE *fp;
	
	nflags = 0;
	nlines = 0;
	even = false;
	odd = false;

	//Read program arguments
	while((opt = getopt(argc, argv, "n:Vheo")) != -1) {
		switch (opt) {
			case 'n':
				nlines = atoi(optarg);
				nflags += 2;
				break;
			case 'V':
				printf("Version 1.0\nAuthor: Thomas Reilly, thomas.reilly@ucdconnect.ie, 18483722\n");
				exit(EXIT_SUCCESS);
				break;
			case 'h':
				fprintf(stderr, "Usage [-n nlines] [-e|-o] even|odd\n");
				exit(EXIT_SUCCESS);
				break;
			case 'e':
				even = true;
				nflags++;
				if (even && odd) {
                        		fprintf(stderr, "Error: Cannot have both even and odd options\n");
                	        	exit(EXIT_FAILURE);
		                }

				break;
			case 'o':
				odd = true;
				nflags++;
				if (even && odd) {
                        		fprintf(stderr, "Error: Cannot have both even and odd options\n");
                		        exit(EXIT_FAILURE);
		                }

				break;
			default:
				fprintf(stderr, "Usage [-n nlines] [-e|-o] even|odd\n");
				exit(EXIT_FAILURE);
		}
	}

	//Handle stdin
	if ((fileName = argv[nflags+1]) == NULL) {
		char input[MAX_LEN][nlines];
		if(odd) { //If odd, read the first line and then every second line
                        fgets (input[0], MAX_LEN, stdin);
                } 
                for (int i=0; i<nlines; i++) {
                        if (even) {//If even, read every second line
                                fgets (line, MAX_LEN, stdin);
                                fgets (input[i], MAX_LEN, stdin);
			} else if (odd) {
				fgets (line, MAX_LEN, stdin);
				fgets (input[i+1], MAX_LEN, stdin);
                        } else { //Normal Input - read everything
                                fgets (input[i], MAX_LEN, stdin);
                        }
                }
		for (int i=0; i<nlines; i++) {
			printf("%s", input[i]);
		}
                exit(EXIT_SUCCESS);
        } 
	else //Handle file input
	{
		//Opens the file and assigns its address to the filePointer
        	fp = fopen(fileName, "r");
        	if (fp == NULL) {
                	fprintf(stderr, "Can't open input file %s\n", fileName);
                	exit(1);
        	}	


        	if(odd) {//If odd, read the first line and then every second line
                	fgets (line, MAX_LEN, fp);
			printf("%s", line);
        	}
        	for (int i=0; i<=nlines; i++) {
                	if (feof(fp)) {
                        	break;
                	}
                	if (even || odd) { //If odd or even read every seond line
                        	fgets (line, MAX_LEN, fp);
                        	fgets (line, MAX_LEN, fp);
                        	printf("%s", line);
                	} else { // Normal input - read every line
                        	fgets (line, MAX_LEN, fp);
                        	printf("%s", line);
                	}
        	}

        	exit(EXIT_SUCCESS);
	
        }

}
