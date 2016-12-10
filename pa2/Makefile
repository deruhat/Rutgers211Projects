CC = gcc
CCFLAGS = -Wall -pedantic -lm

all : calc

calc : calc.o
	$(CC) $(CCFLAGS) -o calc calc.o
	
calc.o : calc.c calc.h
	$(CC) $(CCFLAGS) -c calc.c
	
clean:
	rm -f calc calc.o