# For Linux or any machines with gcc compiler
CC = gcc 
CFLAGS = -std=c99 -Wall -pedantic
BIN = ../bin

all: Lab1

clean:
	/bin/rm *.o $(BIN)/*

OBJ = tiff.o allocate.o

Lab1: filter.o $(OBJ) 
	$(CC) $(CFLAGS) -o Lab1 filter.o $(OBJ) -lm
	mv Lab1 $(BIN)

