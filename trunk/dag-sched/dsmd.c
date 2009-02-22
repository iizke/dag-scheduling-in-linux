/*
 * dsmd.c
 *
 *  Created on: Feb 18, 2009
 *      Author: roseman
 */

#include <unistd.h>
#include <sched.h>
#include <mqueue.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <assert.h>
#include "dagsched.h"
#include "dag.h"
#include "avl.h"

#define MAX_NPARENTS    3
#define MAX_PRIO        19
#define MIN_PRIO        0

/**
 * TODO: do_dag_sched
 * Description: set priority and sched policy to task
 */
int do_dag_sched(struct node_info *node)
{
    struct sched_param param;
    int p;
    if (!node)
        return -1;
//    param.__sched_priority = 50 + MAX_NPARENTS*node->nchildren;
//    p = node->nparents;
//    if (node->nparents >= MAX_NPARENTS)
//        p = MAX_NPARENTS - 1;
//    param.__sched_priority -= p;
//    printf("prio cua %d la %d \n", node->pid, param.__sched_priority);
//    sched_setparam(node->pid, &param);
//    perror("sched_setparam");

    p = 10 + MAX_NPARENTS*node->nchildren;
    if (node->nparents >= MAX_NPARENTS)
        p += (MAX_NPARENTS - 1);
    else
        p += node->nparents;
    if (p < MIN_PRIO)
        p = MIN_PRIO;
    if (p > MAX_PRIO)
        p = MAX_PRIO;
    setpriority(PRIO_PROCESS, node->pid, p);
    perror("setpriority");
    return 0;
}

int process_msg(int dagq_id, struct dag *dag)
{
    struct mq_attr attr;
    int i;
    printf("Do process mesg \n");
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
                    dag_add_node(dag, msginfo.pid1, &node);
                    printf("Da add task %d \n", msginfo.pid1);
                    do_dag_sched(node);
                    break;
                case CMD_REMOVE_TASK:
                    dag_remove_node(dag, msginfo.pid1);
                    printf("Da remove task %d \n", msginfo.pid1);
                    break;
                case CMD_ADD_CONNECTION:
                    dag_add_edge(dag, msginfo.pid1, msginfo.pid2, &edge);
                    do_dag_sched(edge->child);
                    do_dag_sched(edge->parent);
                    break;
                case CMD_REMOVE_CONNECTION:
                    dag_remove_edge(dag, msginfo.pid1, msginfo.pid2);
                    dag_get_node(dag, msginfo.pid1, &node);
                    do_dag_sched(node);
                    dag_get_node(dag, msginfo.pid2, &node);
                    do_dag_sched(node);
                    break;
                default:
                    break;
            }
        }
    } while (attr.mq_curmsgs == 0);
    return 0;
}

struct compact_data {
    int dagq_id;
    struct dag *dag;
};

static void mq_thread(union sigval data)
{
    int dagq_id;
    struct dag *d;
    struct compact_data *cdata = data.sival_ptr;
    printf("mq_thread \n");
    assert(cdata != NULL);
    dagq_id = cdata->dagq_id;
    d = cdata->dag;
    process_msg(dagq_id, d);
}

int main (int argc, char **argv)
{
    int dagq_id;
    struct dag dag;
    struct sigevent sigev;
    struct compact_data cdata;
    dsm_init(&dagq_id);
    printf("queue id do dsm tao = %d \n", dagq_id);
    if (dagq_id == -1){
        perror("dsmd init failed");
        return -1;
    }
    dag_init(&dag);

//    while (1){
//        sleep(2);
//        process_msg(dagq_id, &dag);
//        printf("xong\n");
//    }

    sigev.sigev_notify = SIGEV_THREAD;
    sigev.sigev_notify_function = mq_thread;
    sigev.sigev_notify_attributes = NULL;
    cdata.dagq_id = dagq_id;
    cdata.dag = &dag;
    sigev.sigev_value.sival_ptr = &cdata;
    mq_notify(dagq_id, &sigev);
    perror("mq_notify");
    pause();
    //dsm_halt(dagq_id);
    return 0;
}
