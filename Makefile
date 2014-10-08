
CC = mpicc 
CFLAGS = -Wall -g -O3
LDFLAGS = -lprand
PROGS = pmcpi
#PROGS	= pmcpi smcpi
#OBJS=monte_carlo_pi.o timing.o

all: $(PROGS)

pmcpi: spmd_monte_carlo_pi.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)
	
#smcpi: monte_carlo_pi.o timing.o
#	$(CC) $(CFLAGS) -o $@ $(OBJS)

clean:
	rm -f *.o a.out core $(PROGS) pmcpi
