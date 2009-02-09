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

#include "dag.h"
#include "dagschedd.h"

struct dag dag;
mqd_t dagq_id;

int dsm_add_task(int pid)
{
    struct msg_info msginfo;
    msginfo.cmd = CMD_ADD_TASK;
    msginfo.pid1 = pid;
    msginfo.rank1 = -1;
    mq_send(dagq_id, (char*)&msginfo, sizeof(struct msg_info), MSG_PRIO);
    return 0;
}

int dsm_remove_task(int pid)
{
    struct msg_info msginfo;
    msginfo.cmd = CMD_REMOVE_TASK;
    msginfo.pid1 = pid;
    msginfo.rank1 = -1;
    mq_send(dagq_id, (char*)&msginfo, sizeof(struct msg_info), MSG_PRIO);
    return 0;
}

int dsm_add_mpitask(int rank)
{
    struct msg_info msginfo;
    int pid = getpid();
    msginfo.cmd = CMD_ADD_TASK;
    msginfo.pid1 = pid;
    msginfo.rank1 = rank;
    mq_send(dagq_id, (char*)&msginfo, sizeof(struct msg_info), MSG_PRIO);
    return 0;
}

int dsm_add_connection(int from_pid, int to_pid)
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

int dsm_remove_connection(int from_pid, int to_pid)
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
int dsm_init()
{
    struct mq_attr attr;
    attr.mq_curmsgs = 0;
    attr.mq_flags = O_NONBLOCK;
    attr.mq_maxmsg = MAX_DAGQ_SIZE;
    attr.mq_msgsize = sizeof(struct msg_info) + 1;
    mq_unlink(DAG_MSQ_NAME);
    dagq_id = mq_open(DAG_MSQ_NAME, O_RDWR | O_CREAT | O_NONBLOCK,
                    S_IRWXU | S_IRWXG, &attr);
    dag_init(&dag);
    return 0;
}

/**
 * TODO: do_dag_sched
 * Description: set priority and sched policy to task
 */
int do_dag_sched(struct node_info *node)
{
    return 0;
}

int dsm_process_msg()
{
    struct mq_attr attr;
    int i;
    do {
        int size;
        struct msg_info msginfo;
        mq_getattr(dagq_id, &attr);
        size = attr.mq_curmsgs;
        for (i = 0; i < size; i++) {
            struct node_info *node = NULL;
            struct edge *edge = NULL;
            mq_receive(dagq_id, (char*)&msginfo, sizeof(struct msg_info), NULL);
            switch (msginfo.cmd){
                case CMD_ADD_TASK:
                    dag_add_node(&dag, msginfo.pid1, &node);
                    do_dag_sched(node);
                    break;
                case CMD_REMOVE_TASK:
                    dag_remove_node(&dag, msginfo.pid1);
                    break;
                case CMD_ADD_CONNECTION:
                    dag_add_edge(&dag, msginfo.pid1, msginfo.pid2, &edge);
                    do_dag_sched(edge->child);
                    do_dag_sched(edge->parent);
                    break;
                case CMD_REMOVE_CONNECTION:
                    dag_remove_edge(&dag, msginfo.pid1, msginfo.pid2);
                    dag_get_node(&dag, msginfo.pid1, &node);
                    do_dag_sched(node);
                    dag_get_node(&dag, msginfo.pid2, &node);
                    do_dag_sched(node);
                    break;
                default:
                    break;
            }
        }
    } while (attr.mq_curmsgs == 0);
    return 0;
}

/*
int main (int argc, char **argv)
{
    dagq_id = msgget(DAG_MSQ_ID, IPC_CREAT | 666);
    while (1){
        // Get msg from queue
        // Modify priority, scheduling policy and cpu-affinity
    }
    return 0;
}
*/
