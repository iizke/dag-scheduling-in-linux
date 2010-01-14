/*
 * phase_sched_mpi.h
 *
 *  Created on: Jan 8, 2010
 *      Author: roseman
 */

#ifndef PHASE_SCHED_MPI_H_
#define PHASE_SCHED_MPI_H_

#include <mpi.h>

#define SUCCESS                 0
#define ERR_PHASE_REQ_NULL      (-1)

#define SYSFS_PHASE_REQ_FILE    "/sys/phase-sched/mpireq"

#define PHASE_SCHED_CMD_ADD     0
#define PHASE_SCHED_CMD_DEL     1
#define PHASE_SCHED_CMD_NEW     2

struct phase_mpireq {
    int cmd;
    int usrc_id;
//    union {
        int udest_id;
        int src_pid;
//    };
    int weight;
};

int phase_sched_send_req(struct phase_mpireq *req);
int psMPI_Send(void *buf, int count, MPI_Datatype datatype, int dest, int tag,
        MPI_Comm comm);
int psMPI_Recv(void *buf, int count, MPI_Datatype datatype, int source,
        int tag, MPI_Comm comm, MPI_Status *status);
int psMPI_Init(int *argc, char ***argv);
void psMPI_Finalize();

#endif /* PHASE_SCHED_H_ */
