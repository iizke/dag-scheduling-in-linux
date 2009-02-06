/*
 * dag.h
 *
 *  Created on: Feb 4, 2009
 *      Author: roseman
 */

#ifndef DAG_H_
#define DAG_H_

#define IN
#define OUT

struct node {
    /* Double linked list */
    struct node *next;
    struct node *prev;
    /* DATA */
    int pid;                /* process id provided by system */
    int rank;               /* virtual id */
    char app_name[32];      /* application name */
    int nparents;           /* # of nodes depending on this node*/
    int nchildren;          /* # of nodes on which are depended by this node */
};

struct edge {
    struct edge *next;
    struct edge *prev;
    struct node *parent;
    struct node *child;
    int weight;             /* default value is 0 */
    char state;             /* valid or pending */
};

struct dag_operations;

struct dag {
    struct node *node_list;
    struct edge *edge_list;
    struct dag_operations *dag_ops;
};

int dag_add_edge(int from_pid, int to_pid, OUT struct edge **e);
int dag_remove_edge(int from_pid, int to_pid);
int dag_add_node(int pid, struct node **n);
int dag_remove_node(int pid);
int dag_init();
int dag_clean();

/* Supports for MPI */
int dag_map_mpitask (int rank, OUT struct node **n);
int dag_get_mpitask (int rank, OUT struct node **n);

#endif /* DAG_H_ */
