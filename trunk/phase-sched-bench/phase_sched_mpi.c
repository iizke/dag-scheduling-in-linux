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
phase_sched_send_mpireq(struct phase_mpireq *req)
{
    if (!req)
        return ERR_PHASE_REQ_NULL;

    write2file(SYSFS_PHASE_SCHED_REQ_FILE,
               (void *) req,
               sizeof(struct phase_mpireq));
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
    struct phase_mpireq req;
    MPI_Comm_rank(MPI_COMM_WORLD, &req.udest_id);
    req.cmd = PHASE_SCHED_CMD_ADD;
    req.usrc_id = source;
    req.weight = 1;
    phase_sched_send_mpireq(&req);
    flag = MPI_Recv(buf, count, datatype, source, tag, comm, status);
    req.cmd = PHASE_SCHED_CMD_DEL;
    phase_sched_send_mpireq(&req);
    return flag;
}

int
psMPI_Init(int *argc, char ***argv)
{
    int flag;
    struct phase_mpireq req;

    flag = MPI_Init(argc, argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &req.usrc_id);
    req.cmd = PHASE_SCHED_CMD_NEW;
    req.src_pid = get_pid();
    phase_sched_send_mpireq(&req);
    return flag;
}
