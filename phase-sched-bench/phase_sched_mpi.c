/*
 * phase_sched_mpi.c
 *
 *  Created on: Jan 8, 2010
 *      Author: roseman
 */

int
phase_sched_send_req(struct phase_req *req)
{
    return 0;
}

int
psMPI_Send(
           void *buf,
           int count,
           MPI_Datatype datatype,
           int dest,
           int tag,
           MPI_Comm comm)
{
    return MPI_Send(void *buf, int count, MPI_Datatype datatype, int dest, int tag,
                MPI_Comm comm);
}

int
psMPI_Recv(
           void *buf,
           int count,
           MPI_Datatype datatype,
           int source,
           int tag,
           MPI_Comm comm,
           MPI_Status *status)
{
    return MPI_Recv(void *buf, int count, MPI_Datatype datatype, int source,
                int tag, MPI_Comm comm, MPI_Status *status);
}

int
psMPI_Init(int *argc, char ***argv)
{
    return MPI_Init(int *argc, char ***argv);
}
