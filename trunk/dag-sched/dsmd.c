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
#include "dagsched.h"
#include "dag.h"
/**
 * TODO: do_dag_sched
 * Description: set priority and sched policy to task
 */
int do_dag_sched(struct node_info *node)
{
    struct sched_param param;
    if (!node)
        return -1;
    param.__sched_priority = 10 - node->nchildren + node->nparents;
    sched_setparam(node->pid, &param);
    return 0;
}

int process_msg(int dagq_id, struct dag *dag)
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
                    dag_add_node(dag, msginfo.pid1, &node);
                    do_dag_sched(node);
                    break;
                case CMD_REMOVE_TASK:
                    dag_remove_node(dag, msginfo.pid1);
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

int main (int argc, char **argv)
{
    int dagq_id;
    struct dag dag;
    dsm_init(&dagq_id);
    dag_init(&dag);
    while (1){
        sleep(2);
        printf("sched deamon running ...\n");
        process_msg(dagq_id, &dag);
    }
    dsm_halt(dagq_id);
    return 0;
}
