all: sim

SRCS	= sim.c
OBJS	= ${SRCS:.c=.o}
.c.o:
	gcc -c -Wall -ansi -pedantic $<

sim: $(OBJS)
	gcc $(OBJS) -o sim

clean:
	rm -rf $(OBJS)  sim