CC = gcc
CCFLAGS = -Wall -pedantic -lm

all : format

format : format.o
	$(CC) $(CCFLAGS) -o format format.o
	
format.o : format.c format.h
	$(CC) $(CCFLAGS) -c format.c
	
clean:
	rm -f format format.o