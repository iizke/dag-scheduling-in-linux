#include <mpi.h>
#include <stdio.h>
#define WORKTAG 1
#define DIETAG 2

int
main(int argc, char **argv)
{
  int myrank;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  if (myrank == 0) 
    printf("Master \n");
  else
    printf("slave \n");

  MPI_Finalize();
  return 0;
}

