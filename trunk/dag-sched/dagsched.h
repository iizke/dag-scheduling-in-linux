/*
 * dagschedd.h
 *
 *  Created on: Feb 8, 2009
 *      Author: roseman
 */

#ifndef DAGSCHEDD_H_
#define DAGSCHEDD_H_

#define DAG_MSQ_NAME            "IIZKE"
#define MSG_PRIO                1
#define MAX_DAGQ_SIZE           100
#define CMD_ADD_TASK            0
#define CMD_REMOVE_TASK         1
#define CMD_ADD_CONNECTION      2
#define CMD_REMOVE_CONNECTION   3

struct msg_info {
    int cmd;
    int pid1;
    int rank1;
    int pid2;
    int rank2;
};

/* DSM: DAG Scheduling Module */
int dsm_add_task(int dagq_id, int pid);
int dsm_remove_task(int dagq_id, int pid);
int dsm_add_mpitask(int dagq_id, int rank);
int dsm_add_connection(int dagq_id, int from_rank, int to_rank);
int dsm_remove_connection(int dagq_id, int from_rank, int to_rank);
int dsm_init(int *dagq_id);
int dsm_halt(int dagq_id);
#endif /* DAGSCHEDD_H_ */
