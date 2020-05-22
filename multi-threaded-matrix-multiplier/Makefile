CC=gcc
CFFLAGS=-g -Wall
LIBS=-lm
HFILES=matrix.h
OBJ=master.o worker.o matrix.o

all: both

both: 
	$(CC) $(LIBS) -pthread -o master master.c matrix.c
	$(CC) $(LIBS) -o worker worker.c matrix.c

master:
	$(CC) $(LIBS) -pthread -o master master.c matrix.c

worker:
	$(CC) $(LIBS) -o worker worker.c matrix.c

clean:
	rm -rf master worker