/*
 * phase_sched_mpi.h
 *
 *  Created on: Jan 8, 2010
 *      Author: roseman
 */

#ifndef PHASE_SCHED_MPI_H_
#define PHASE_SCHED_MPI_H_

#include <mpi.h>

struct phase_req {
    int cmd;
    int src_id;
    int dest_id;
    int weight;
};

int phase_sched_send_req(struct phase_req *req);
int psMPI_Send(void *buf, int count, MPI_Datatype datatype, int dest, int tag,
        MPI_Comm comm);
int psMPI_Recv(void *buf, int count, MPI_Datatype datatype, int source,
        int tag, MPI_Comm comm, MPI_Status *status);
int psMPI_Init(int *argc, char ***argv);

#endif /* PHASE_SCHED_H_ */
