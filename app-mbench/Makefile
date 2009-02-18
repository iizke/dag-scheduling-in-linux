CC=mpicc
CFLAGS=-W -Wall -g -O2 -s -pipe -I ../dag-sched
LDFLAGS=-lrt -lm
all : mbench-mpi mbench-ipc dag2mbenchScript automateDag 
mbench-ipc : mbench-ipc.o genload.o parser.o errors.o
mbench-mpi : mbench-mpi.o genload.o parser.o errors.o convert.o
dag2mbenchScript : dag2mbenchScript.o parser.o
automateDag : automateDag.o randf.o parser.o 

mbench-mpi.o : mbench-mpi.c 
mbench-ipc.o : mbench-ipc.c
errors.o : errors.c
genload.o : genload.c
parser.o : parser.c
convert.o : convert.c
randf.o : randf.c randf.h

clean : 
	rm -f *~ *.o mbench-mpi mbench-ipc mix test dag2mbenchScript

cleanlog : 
	rm mbench.log timelog templog
