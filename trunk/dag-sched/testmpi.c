
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <mqueue.h>
#include <fcntl.h>
#include <mpi.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "dagsched.h"

int main(int argc, char **argv)
{
    int pid;
    mqd_t qid = 0;
    int rank;
    int *buf = NULL;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    buf = malloc(sizeof(*buf) * 100);
    pid = getpid();
    dsm_init(&qid);
    printf("queue id = %d \n", qid);
    if (qid < 0)
        goto end;
    dsm_add_mpitask(qid, rank);
    if (rank == 0){
        sleep (5);
        MPI_Send(&buf, 100, MPI_INT, 1, 0, MPI_COMM_WORLD);
        dsm_remove_mpi_connection(qid, 1, 0);
    } else if (rank == 1) {
        MPI_Recv(&buf, 100, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        // add connection
        dsm_add_mpi_connection(qid, 1, 0);
    }
    sleep(1);
    printf("PRIO OF %d IS %d \n",pid, getpriority(PRIO_PROCESS, pid));
    dsm_remove_mpitask(qid, rank);
end:
    //perror("BUG");
    dsm_halt(qid);
    MPI_Finalize();
    return 0;
}

