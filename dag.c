/*
 * dag.c
 *
 *  Created on: Feb 4, 2009
 *      Author: roseman
 */
#include <sys/types.h>
#include <unistd.h>
#include "dag.h"

struct dag dag;

/**
 * TODO: _dag_add_node
 * Description:
 */
static int _dag_add_node (struct dag *d, int pid, OUT struct node **n)
{
    return 0;
}

/**
 * TODO: dag_add_node
 * Description: wrapper of _dag_add_node
 */
int dag_add_node (int pid, struct node **n)
{
    return _dag_add_node(&dag, pid, n);
}

/**
 * TODO: _dag_remove_node
 * Description:
 */
static int _dag_remove_node (struct dag *d, int pid)
{
    return 0;
}

/**
 * TODO: dag_remove_node
 * Description: wrapper of _dag_remove_node
 */
int dag_remove_node (int pid)
{
    return _dag_remove_node(&dag, pid);
}

/**
 * TODO: _dag_add_edge
 * Description:
 */
static int _dag_add_edge (struct dag *d, int from_pid, int to_pid,
                            OUT struct edge **e)
{
    return 0;
}

/**
 * TODO: dag_add_edge
 * Description: wrapper of _dag_add_edge
 */
int dag_add_edge (int from_pid, int to_pid, OUT struct edge **e)
{
    return _dag_add_edge(&dag, from_pid, to_pid, e);
}

/**
 * TODO: _dag_remove_edge
 * Description:
 */
static int _dag_remove_edge (struct dag *d, int from_pid, int to_pid)
{
    return 0;
}

/**
 * TODO: dag_remove_edge
 * Description: wrapper of _dag_remove_edge
 */
int dag_remove_edge (int from_pid, int to_pid)
{
    return _dag_remove_edge(&dag, from_pid, to_pid);
}

/**
 * TODO: _dag_get_node
 * Description: get node from pid
 * @return: error code
 */
static int _dag_get_node (struct dag *d, int pid, struct node **n)
{
    return 0;
}

/**
 * TODO: dag_get_node
 * Description: wrapper of _dag_get_node. Get node from pid
 * @return: error code
 */
int dag_get_node (int pid, struct node **n)
{
    return _dag_get_node(&dag, pid, n);
}

/**
 * TODO: dag_init
 * Description:
 */
int dag_init ()
{
    return 0;
}

/**
 * TODO: dag_clean
 * Description:
 */
int dag_clean ()
{
    return 0;
}

/**
 * TODO: dag_map_mpitask
 * Description: mapping rank of task (user-land) to pid (system-land)
 * @return: error code
 *
 * Complexity: O(n)
 */
int dag_map_mpitask (int rank, OUT struct node ** n)
{
    int pid;
    int flag;

    pid = getpid();
    flag = dag_add_node(pid, n);
    if (flag < 0)
        return flag;
    (*n)->rank = rank;
    return 0;
}

/**
 * TODO: dag_get_mpitask
 * Description: get a node in DAG
 * @return: error code
 */
int dag_get_mpitask (int rank, OUT struct node ** n)
{
    return 0;
}
