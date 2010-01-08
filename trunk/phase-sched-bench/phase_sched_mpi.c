/*
 * phase_sched_mpi.c
 *
 *  Created on: Jan 8, 2010
 *      Author: roseman
 */
#include "phase_sched_mpi.h"

static int
write2file(char *file, void *buf, int size)
{
    FILE *fp;
    fp = fopen(file, "wb");
    fwrite(buf, size, 1, fp);
    fclose(fp);
    return SUCCESS;
}

int
phase_sched_send_mpireq(struct phase_mpireq *req, int rank)
{
    if (!req)
        return ERR_PHASE_REQ_NULL;

    write2file(SYSFS_PHASE_SCHED_REQ_FILE,
               (void *) req,
               sizeof(struct phase_req));
    return SUCCESS;
}

int
psMPI_Send(void *buf,
           int count,
           MPI_Datatype datatype,
           int dest,
           int tag,
           MPI_Comm comm)
{
    return MPI_Send(buf, count, datatype, dest, tag, comm);
}

int
psMPI_Recv(void *buf,
           int count,
           MPI_Datatype datatype,
           int source,
           int tag,
           MPI_Comm comm,
           MPI_Status *status)
{
    int flag;
    flag = MPI_Recv(buf, count, datatype, source, tag, comm, status);
    return flag;
}

int
psMPI_Init(int *argc, char ***argv)
{
    int flag;
    int rank;
    struct phase_req req;

    flag = MPI_Init(argc, argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    req.cmd = PHASE_SCHED_CMD_NEW;
    req.src_id = get_pid();
    phase_sched_send_mpireq(&req, rank);
    return flag;
}
