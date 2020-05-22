# Hash for comments
# $ represents variables
# @ represents the tag
# ^ represents the dependencies (the things following the tag:)

CC=gcc
CFFLAGS=-g -Wall
LIBS=-lm
HFILES=shell.h
OBJ=main.o prompt.o parse_string.o redirect.o signals.o run_command.o

all: shell

shell: $(OBJ)
	$(CC) $(LIBS) $^ -o $@

%.o: %.c $(HFILES)
	$(CC) -c $(CFLAGS) $<

clean:
	rm -rf *.o shell
