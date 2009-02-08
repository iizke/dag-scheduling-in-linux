/*
 * dagschedd.c
 * DAG Schedule Daemon
 *  Created on: Feb 6, 2009
 *      Author: root
 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "dag.h"
#include "dagschedd.h"

struct dag dag;
int dagq_id = msgget(DAG_MSQ_ID, IPC_CREAT | 666);

int dsm_add_task(int pid)
{
    return 0;
}

int dsm_remove_task(int pid)
{
    return 0;
}

int dsm_add_mpitask(int rank)
{
    return 0;
}

int dsm_add_connection(int from_pid, int to_pid)
{
    return 0;
}

int dsm_remove_connection(int from_pid, int to_pid)
{
    return 0;
}

int main (int argc, char **argv)
{
    while (1){
        // Get msg from queue
        // Modify priority, scheduling policy and cpu-affinity
    }
    return 0;
}
