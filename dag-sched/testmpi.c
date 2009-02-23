
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
    MPI_Init(&argc, &argv);
    pid = getpid();
    dsm_init(&qid);
    printf("queue id = %d \n", qid);
    if (qid < 0)
        goto end;
    dsm_add_task(qid, pid);
    sleep(5);
    printf("DO UU TIEN CUA %d LA %d \n",pid, getpriority(PRIO_PROCESS, pid));
    dsm_remove_task(qid, pid);
end:
    perror("BUG");
    dsm_halt(qid);
    MPI_Finalize();
    return 0;
}

