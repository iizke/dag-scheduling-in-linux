/*
 * dag.h
 *
 *  Created on: Feb 4, 2009
 *      Author: roseman
 */

#ifndef DAG_H_
#define DAG_H_

struct node {
    /* Double linked list */
    struct node *next;
    struct node *prev;
    /* DATA */
    int weight;
    int pid;
    /* rank is virtual id */
    int rank;
};

struct edge {
    struct edge *next;
    struct edge *prev;
    struct node *parent;
    struct node *child;
    /* Weight: default value is 0 */
    int weight;
    /* state: valid or pending */
    char state;
};

struct dag_operations;

struct dag {
    struct node *node_list;
    struct edge *edge_list;
    struct dag_operations *dag_ops;
};

struct mpi_task {
    struct mpi_task *next;
    struct mpi_task *prev;
    int pid;
    int rank;
}; 

struct mpi_task_list {
    struct mpi_task *list;
};

int dag_add_edge(struct dag *d, int from_pid, int to_pid);
int dag_remove_edge(struct dag *d, int from_pid, int to_pid);
int mpi_map_rank2pid(char* app_name, int rank);
int mpi_task_list_add(struct mpi_task_list *list, int pid, int rank);

#endif /* DAG_H_ */
