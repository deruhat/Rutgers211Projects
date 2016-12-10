CC = gcc
CCFLAGS = -Wall -pedantic -ansi

all : sim

sim : sim.o
	$(CC) $(CCFLAGS) -o sim sim.o
	
sim.o : sim.c sim.h
	$(CC) $(CCFLAGS) -c sim.c
	
clean:
	rm -f sim sim.o