CC = gcc
CCFLAGS = -Wall -pedantic -ansi -lm

all : y86emul y86dis

y86emul : y86emul.o
	$(CC) $(CCFLAGS) -o y86emul y86emul.o
	
y86emul.o : y86emul.c y86emul.h
	$(CC) $(CCFLAGS) -c y86emul.c
	
y86dis : y86dis.o
	$(CC) $(CCFLAGS) -o y86dis y86dis.o
	
y86dis.o : y86dis.c y86dis.h
	$(CC) $(CCFLAGS) -c y86dis.c
	
clean:
	rm -f y86emul y86emul.o y86dis y86dis.o