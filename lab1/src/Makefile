# For Linux or any machines with gcc compiler
CC = gcc 
CFLAGS = -std=c99 -Wall -pedantic
BIN = ../bin

all: Example

clean:
	/bin/rm *.o $(BIN)/*

OBJ = tiff.o allocate.o

Example: filter.o $(OBJ) 
	$(CC) $(CFLAGS) -o Example filter.o $(OBJ) -lm
	mv Example $(BIN)

