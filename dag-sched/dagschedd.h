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
int dsm_add_task(int pid);
int dsm_remove_task(int pid);
int dsm_add_mpitask(int rank);
int dsm_add_connection(int from_rank, int to_rank);
int dsm_remove_connection(int from_rank, int to_rank);

#endif /* DAGSCHEDD_H_ */
