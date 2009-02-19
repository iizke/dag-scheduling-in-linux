CC=mpicc
CFLAGS=-lrt -O3 -Wall

all: testmpi
testmpi: testmpi.c dagsched.o
dagsched.o: dagsched.c dagsched.h
clean: 
	-rm -f *.o *~ testmpi
