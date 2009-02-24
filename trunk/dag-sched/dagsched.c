/*
 * dagschedd.c
 * DAG Schedule Daemon
 *  Created on: Feb 6, 2009
 *      Author: root
 */
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>

#include "dagsched.h"

int dsm_add_task(mqd_t dagq_id, int pid)
{
    struct msg_info msginfo;
    msginfo.cmd = CMD_ADD_TASK;
    msginfo.pid1 = pid;
    msginfo.rank1 = -1;
    mq_send(dagq_id, (char*)&msginfo, sizeof(struct msg_info), MSG_PRIO);
    printf("Da gui request add task %d \n", pid);
    return 0;
}

int dsm_remove_task(mqd_t dagq_id, int pid)
{
    struct msg_info msginfo;
    msginfo.cmd = CMD_REMOVE_TASK;
    msginfo.pid1 = pid;
    msginfo.rank1 = -1;
    mq_send(dagq_id, (char*)&msginfo, sizeof(struct msg_info), MSG_PRIO);
    printf("Da gui request remove task %d \n", pid);
    return 0;
}

int dsm_add_mpitask(mqd_t dagq_id, int rank)
{
    struct msg_info msginfo;
    int pid = getpid();
    msginfo.cmd = CMD_ADD_TASK;
    msginfo.pid1 = pid;
    msginfo.rank1 = rank;
    mq_send(dagq_id, (char*)&msginfo, sizeof(struct msg_info), MSG_PRIO);
    return 0;
}

int dsm_add_connection(mqd_t dagq_id, int from_pid, int to_pid)
{
    struct msg_info msginfo;
    msginfo.cmd = CMD_ADD_CONNECTION;
    msginfo.pid1 = from_pid;
    msginfo.rank1 = -1;
    msginfo.pid2 = to_pid;
    msginfo.rank2 = -1;
    mq_send(dagq_id, (char*)&msginfo, sizeof(struct msg_info), MSG_PRIO);
    return 0;
}

int dsm_remove_connection(mqd_t dagq_id, int from_pid, int to_pid)
{
    struct msg_info msginfo;
    msginfo.cmd = CMD_REMOVE_CONNECTION;
    msginfo.pid1 = from_pid;
    msginfo.rank1 = -1;
    msginfo.pid2 = to_pid;
    msginfo.rank2 = -1;
    mq_send(dagq_id, (char*)&msginfo, sizeof(struct msg_info), MSG_PRIO);
    return 0;
}

int dsm_add_mpi_connection(mqd_t dagq_id, int from_rank, int to_rank)
{
    struct msg_info msginfo;
    msginfo.cmd = CMD_ADD_MPI_CONNECTION;
    msginfo.pid1 = -1;
    msginfo.rank1 = from_rank;
    msginfo.pid2 = -1;
    msginfo.rank2 = to_rank;
    if (to_rank < 0)
        msginfo.pid2 = getpid();
    mq_send(dagq_id, (char*)&msginfo, sizeof(struct msg_info), MSG_PRIO);
    return 0;
}
/**
 * TODO: dsm_init
 * Description: Init DAG Scheduling Module
 */
int dsm_init(mqd_t *dagq_id)
{
    struct mq_attr attr;
    if (!dagq_id)
        return -1;
    attr.mq_curmsgs = 0;
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_DAGQ_SIZE;
    attr.mq_msgsize = sizeof(struct msg_info);
    //mq_unlink(DAG_MSQ_NAME);
    (*dagq_id) = mq_open(DAG_MSQ_NAME, O_RDWR | O_CREAT,
                    S_IRWXU | S_IRWXG, &attr);
    //dag_init(&dag);
    return 0;
}

int dsm_halt(mqd_t dagq_id)
{
    mq_close(dagq_id);
    return 0;
}
