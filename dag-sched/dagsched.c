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


int dsm_add_task(int dagq_id, int pid)
{
    struct msg_info msginfo;
    msginfo.cmd = CMD_ADD_TASK;
    msginfo.pid1 = pid;
    msginfo.rank1 = -1;
    mq_send(dagq_id, (char*)&msginfo, sizeof(struct msg_info), MSG_PRIO);
    return 0;
}

int dsm_remove_task(int dagq_id, int pid)
{
    struct msg_info msginfo;
    msginfo.cmd = CMD_REMOVE_TASK;
    msginfo.pid1 = pid;
    msginfo.rank1 = -1;
    mq_send(dagq_id, (char*)&msginfo, sizeof(struct msg_info), MSG_PRIO);
    return 0;
}

int dsm_add_mpitask(int dagq_id, int rank)
{
    struct msg_info msginfo;
    int pid = getpid();
    msginfo.cmd = CMD_ADD_TASK;
    msginfo.pid1 = pid;
    msginfo.rank1 = rank;
    mq_send(dagq_id, (char*)&msginfo, sizeof(struct msg_info), MSG_PRIO);
    return 0;
}

int dsm_add_connection(int dagq_id, int from_pid, int to_pid)
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

int dsm_remove_connection(int dagq_id, int from_pid, int to_pid)
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

/**
 * TODO: dsm_init
 * Description: Init DAG Scheduling Module
 */
int dsm_init(int *dagq_id)
{
    struct mq_attr attr;
    if (!dagq_id)
        return -1;
    attr.mq_curmsgs = 0;
    attr.mq_flags = O_NONBLOCK;
    attr.mq_maxmsg = MAX_DAGQ_SIZE;
    attr.mq_msgsize = sizeof(struct msg_info) + 1;
    mq_unlink(DAG_MSQ_NAME);
    (*dagq_id) = mq_open(DAG_MSQ_NAME, O_RDWR | O_CREAT | O_NONBLOCK,
                    S_IRWXU | S_IRWXG, &attr);
    //dag_init(&dag);
    return 0;
}

int dsm_halt(int dagq_id)
{
    mq_close(dagq_id);
    return 0;
}
