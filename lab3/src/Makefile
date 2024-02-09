# For Linux or any machines with gcc compiler
CC = gcc 
CFLAGS = -std=c99 -Wall -pedantic
BIN = ../bin

all: Lab3

clean:
	/bin/rm *.o $(BIN)/*

OBJ = tiff.o allocate.o filter.o areafill.o

Lab3: filter.o $(OBJ) 
	$(CC) main.c $(CFLAGS) -o Lab3 $(OBJ) -lm
	mv Lab3 $(BIN)

