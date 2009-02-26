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
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>
#include "dagsched.h"
#include "dag.h"
#include "avl.h"

#define MAX_NPARENTS    5
#define MAX_PRIO        19
#define MIN_PRIO        0

/**
 * TODO: do_dag_sched
 * Description: set priority and sched policy to task
 */
int
do_dag_sched(struct node_info *node)
{
    //    struct sched_param param;
    int p;
    if (!node) return -1;
    //    param.__sched_priority = 50 + MAX_NPARENTS*node->nchildren;
    //    p = node->nparents;
    //    if (node->nparents >= MAX_NPARENTS)
    //        p = MAX_NPARENTS - 1;
    //    param.__sched_priority -= p;
    //    printf("prio cua %d la %d \n", node->pid, param.__sched_priority);
    //    sched_setparam(node->pid, &param);
    //    perror("sched_setparam");

//
//    p = 19 + MAX_NPARENTS * node->nchildren;
//    if ((2*node->nparents) >= MAX_NPARENTS)
//        p -= (MAX_NPARENTS - 1);
//    else
//        p -= (20*node->nparents);
//    if (p < MIN_PRIO) p = MIN_PRIO;
//    if (p > MAX_PRIO) p = MAX_PRIO;
//    //    printf("pid = %d, prio p = %d \n", node->pid, p);
    if (node->nchildren > 0)
        p = 19;
    else if (node->nparents == 0)
        p = 19;
    else
        p = 0;

    setpriority(PRIO_PROCESS, node->pid, p);
    printf("do dag sched: node %d, p = %d, c= %d, prio = %d \n", node->pid, node->nparents, node->nchildren, p);
    //perror("setpriority");
    return 0;
}

int
process_msg(int dagq_id, struct dag *dag)
{
    struct mq_attr attr;
    int i;
    int size;

    mq_getattr(dagq_id, &attr);
    size = attr.mq_curmsgs;
    do {
        struct msg_info msginfo;
        for (i = 0; i < size; i++) {
            //struct node_info *node = NULL;
            //struct edge *edge = NULL;
            mq_receive(dagq_id, (char*) &msginfo, sizeof(struct msg_info), NULL);
            switch (msginfo.cmd) {
                case CMD_ADD_TASK:
                    //dag_add_node(dag, msginfo.pid1, &node);
                    //do_dag_sched(node);
                    break;
                case CMD_REMOVE_TASK:
                    //dag_remove_node(dag, msginfo.pid1);
                    break;
                case CMD_ADD_CONNECTION:
                    //dag_add_edge(dag, msginfo.pid1, msginfo.pid2, &edge);
                    //do_dag_sched(edge->child);
                    //do_dag_sched(edge->parent);
                    break;
                case CMD_REMOVE_CONNECTION:
                    //dag_remove_edge(dag, msginfo.pid1, msginfo.pid2);
                    //dag_get_node(dag, msginfo.pid1, &node);
                    //do_dag_sched(node);
                    //dag_get_node(dag, msginfo.pid2, &node);
                    //do_dag_sched(node);
                    break;
                case CMD_ADD_MPI_CONNECTION:
                    dag_add_mpi_edge(dag, msginfo.rank1, msginfo.rank2);
                    do_dag_sched(&(dag->node_list.list[msginfo.rank1]));
                    do_dag_sched(&(dag->node_list.list[msginfo.rank2]));
                    //dag_get_pid(dag, msginfo.pid1, msginfo.pid2, &edge);
                    //do_dag_sched(edge->child);
                    //do_dag_sched(edge->parent);
                    break;
                case CMD_ADD_MPI_TASK:
                    dag_add_mpi_node(dag, msginfo.rank1, msginfo.pid1);
                    do_dag_sched(&(dag->node_list.list[msginfo.rank1]));
                    break;
                case CMD_REMOVE_MPI_CONNECTION:
                    dag_remove_mpi_edge(dag, msginfo.rank1, msginfo.rank2);
                    do_dag_sched(&(dag->node_list.list[msginfo.rank1]));
                    do_dag_sched(&(dag->node_list.list[msginfo.rank2]));
                    break;
                case CMD_REMOVE_MPI_TASK:
                    dag_remove_mpi_node(dag, msginfo.rank1);
                    // TODO: Should we reset all task's priority?
                    break;
                default:
                    break;
            }
        }
        mq_getattr(dagq_id, &attr);
        size = attr.mq_curmsgs;
    }
    while (size > 0);
    return 0;
}

struct compact_data {
    int dagq_id;
    struct dag *dag;
};

static void
mq_thread(union sigval data)
{
    int dagq_id;
    struct dag *d;
    struct compact_data *cdata = data.sival_ptr;
    struct sigevent sigev;
    //struct node_info *node;
    assert(cdata != NULL);
    dagq_id = cdata->dagq_id;
    d = cdata->dag;
    printf("mq_thread: dag node list co %d node \n", d->node_list.size);
    sigev.sigev_notify = SIGEV_THREAD;
    sigev.sigev_notify_function = mq_thread;
    sigev.sigev_notify_attributes = NULL;
    sigev.sigev_value.sival_ptr = cdata;

    process_msg(dagq_id, d);
    mq_notify(dagq_id, &sigev);
    //perror("mq_notify");
    //pause();
}

int
main(int argc, char **argv)
{
    int dagq_id;
    struct dag * dag = (struct dag*) malloc(sizeof(struct dag));
    struct sigevent sigev;
    struct compact_data cdata;
    mq_unlink("/HHNAM");
    dsm_init(&dagq_id);
    if (dagq_id == -1) {
        perror("dsmd init failed");
        return -1;
    }
    dag_init(dag);

//    while (1) {
//        sleep(1);
//        process_msg(dagq_id, dag);
//        printf("dag con %d node \n", dag->node_list.size);
//    }

    sigev.sigev_notify = SIGEV_THREAD;
    sigev.sigev_notify_function = mq_thread;
    sigev.sigev_notify_attributes = NULL;
    cdata.dagq_id = dagq_id;
    cdata.dag = dag;
    sigev.sigev_value.sival_ptr = &cdata;
    mq_notify(dagq_id, &sigev);
    //perror("mq_notify");
    pause();
    //dsm_halt(dagq_id);
    return 0;
}
