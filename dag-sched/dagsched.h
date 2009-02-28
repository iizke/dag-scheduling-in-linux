/*
 * dagschedd.h
 *
 *  Created on: Feb 8, 2009
 *      Author: roseman
 */

#ifndef DAGSCHEDD_H_
#define DAGSCHEDD_H_

#define DAG_MSQ_NAME                "/HHNAM"
#define MSG_PRIO                    1
#define MAX_DAGQ_SIZE               10

#define CMD_ADD_TASK                0
#define CMD_REMOVE_TASK             1
#define CMD_ADD_CONNECTION          2
#define CMD_REMOVE_CONNECTION       3
#define CMD_ADD_MPI_CONNECTION      4
#define CMD_ADD_MPI_TASK            5
#define CMD_REMOVE_MPI_TASK         6
#define CMD_REMOVE_MPI_CONNECTION   7


struct msg_info {
    int cmd;
    int pid1;
    int rank1;
    int pid2;
    int rank2;
};

/* DSM: DAG Scheduling Module */
int dsm_add_task(mqd_t dagq_id, int pid);
int dsm_remove_task(mqd_t dagq_id, int pid);
int dsm_add_connection(mqd_t dagq_id, int from_rank, int to_rank);
int dsm_remove_connection(mqd_t dagq_id, int from_rank, int to_rank);

int dsm_add_mpitask(mqd_t dagq_id, int rank);
int dsm_remove_mpitask(mqd_t dagq_id, int rank);
int dsm_add_mpi_connection(mqd_t dagq_id, int from_rank, int to_rank);
int dsm_remove_mpi_connection(mqd_t dagq_id, int from_rank, int to_rank);
int dsm_init(mqd_t *dagq_id);
int dsm_halt(mqd_t dagq_id);
#endif /* DAGSCHEDD_H_ */
