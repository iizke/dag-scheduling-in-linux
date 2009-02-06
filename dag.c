/*
 * dag.c
 *
 *  Created on: Feb 4, 2009
 *      Author: roseman
 */
#include <sys/types.h>
#include <unistd.h>
#include "dag.h"

#define WEIGHT_TO_PRIO(w)   (15-w)

struct mpi_task_list mpi_task_list;
struct dag dag;

/** TODO
 *
 */
int mpi_task_list_add(struct mpi_task_list *list, int pid, int rank)
{
    return 0;
}

int mpi_map_rank2pid(char *app_name, int rank)
{
    int pid;
    int flag;

    pid = getpid();
    flag = mpi_task_list_add(&mpi_task_list, pid, rank);
    return 0;
}

