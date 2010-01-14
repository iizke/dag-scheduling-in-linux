CC=mpicc
CFLAGS=-W -Wall -g -O2 -s -pipe 
LDFLAGS=-lrt -lm
all : mbench-dagmpi dag2mbenchScript automateDag 

dag2mbenchScript : dag2mbenchScript.o parser.o
automateDag : automateDag.o randf.o parser.o 
mbench-dagmpi: mbench-dagmpi.o genload.o parser.o errors.o convert.o phase_sched_mpi.o

mbench-dagmpi.o : mbench-dagmpi.c 
errors.o : errors.c
genload.o : genload.c
parser.o : parser.c
convert.o : convert.c
randf.o : randf.c randf.h
phase_sched_mpi.o : phase_sched_mpi.c phase_sched_mpi.h

clean : 
	rm -f *~ *.o dag2mbenchScript mbench-dagmpi automateDag

cleanlog : 
	rm mbench.log timelog templog
