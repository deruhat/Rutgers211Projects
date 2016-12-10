CC = gcc
CCFLAGS = -Wall -pedantic

all : mystery

mystery : mystery.o
	$(CC) $(CCFLAGS) -o mystery mystery.o
	
mystery.o : mystery.c mystery.h
	$(CC) $(CCFLAGS) -c mystery.c
	
clean:
	rm -f mystery mystery.o