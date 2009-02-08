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

#include "avl.h"

typedef struct avl_node NODE
typedef struct avl_table NODE_LIST

struct node_info {
    int pid;                /* process id provided by system */
    int rank;               /* virtual id */
    char app_name[32];      /* application name */
    struct edge *parent_edge_list;
    int nparents;           /* # of nodes depending on this node*/
    struct edge *children_edge_list;
    int nchildren;          /* # of nodes on which are depended by this node */
};

struct edge {
    struct edge *next;
    struct edge *prev;
    struct node_info *parent;
    struct node_info *child;
    int weight;             /* default value is 0 */
    char state;             /* valid or pending */
};

struct edge_list {
    struct edge *list;
    int size;
};

struct dag {
    NODE_LIST *node_list;
    struct edge_list *edge_list;
};

int dag_add_edge(struct dag *d, int from_pid, int to_pid, OUT struct edge **e);
int dag_remove_edge(struct dag *d, int from_pid, int to_pid);
int dag_add_node(struct dag *d, int pid, struct node_info **info);
int dag_remove_node(struct dag *d, int pid);
int dag_init();
int dag_clean();

/* Supports for MPI */
int dag_map_mpitask (int rank, OUT struct node **n);
int dag_get_mpitask (int rank, OUT struct node **n);

#endif /* DAG_H_ */
