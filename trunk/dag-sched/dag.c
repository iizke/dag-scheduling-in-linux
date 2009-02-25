/*
 * dag.c
 *
 *  Created on: Feb 4, 2009
 *      Author: roseman
 */
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "dag.h"

/**
 * TODO: edge_list_init
 * Description: Init edge list
 */
//int edge_list_init(struct edge_list *list)
//{
//    if (!list)
//        return -1;
//    list->size = 0;
//    list->list = NULL;
//    return 0;
//}

/**
 * TODO: edge_list_destroy
 * Description: clean all elements in list
 */
//int edge_list_destroy(struct edge_list *list)
//{
//    struct edge *next = NULL;
//    int i;
//
//    if (!list)
//        return -1;
//    next = list->list->next;
//    for (i=0; i<list->size; i++) {
//        free(list->list);
//        list->list = next;
//        next = next->next;
//    }
//    return 0;
//}

int
edge_list_destroy(struct edge_list *list)
{
    int i;
    if (!list) return -1;
    for (i = 0; i < MAX_EDGES; i++) {
        list->list[i].state = EDGE_STATE_INVALID;
        list->list[i].weight = 0;
    }
    list->size = 0;
    return 0;
}
/**
 * TODO: __edge_list_remove_edge
 * Description: remove edge in edge_list
 */
//int
//__edge_list_remove_edge(struct edge_list *list, struct edge *e)
//{
//    struct edge *prev = NULL;
//    struct edge *next = NULL;
//
//    if (!e)
//        return 0;
//    prev = e->prev;
//    next = e->next;
//    prev->next = next;
//    next->prev = prev;
//    if (prev == e){
//        list->list = NULL;
//        list->size = 0;
//        return 0;
//    }
//    e->child->nparents--;
//    e->parent->nchildren--;
//    free(e);
//    list->size--;
//    return 0;
//}

/**
 * TODO: edge_list_remove_edge
 * Description: remove edge in edge_list
 */
//int
//edge_list_remove_edge(struct edge_list *list, struct node_info *parent,
//                struct node_info *child)
//{
//    int i;
//    int size;
//    struct edge *e = NULL;
//    struct edge *next = NULL;
//
//    if (!list) {
//        printf("edge_list pointer is null \n");
//        return -1;
//    }
//    size = list->size;
//    e = list->list;
//    for (i=0; i<size; i++) {
//        if (!e)
//            return 0;
//        next = e->next;
//        if ((e->parent == parent) && (e->child == child)) {
//            __edge_list_remove_edge(list, e);
//            break;
//        }
//        e = next;
//    }
//    return 0;
//}

int
__edge_list_remove_edge(struct edge_list *list, int edge_id)
{
    if (edge_id < 0) return 0;
    if (list->list[edge_id].state == EDGE_STATE_VALID) {
        list->size--;
    }
    list->list[edge_id].state = EDGE_STATE_INVALID;
    list->list[edge_id].child->nparents--;
    list->list[edge_id].parent->nchildren--;
    list->list[edge_id].weight = 0;
    list->size--;
    return 0;
}

int
calc_edge_index(int pid, int cid)
{
    int id = (pid * (MAX_NODES - 1) + cid - 1 - (pid + 1) * pid / 2);
    //printf("calc_edge_index: p=%d, c=%d, e=%d \n",pid, cid, id);
    return id;
}

int
edge_list_remove_edge(struct dag* d, int pid, int cid)
{
    int edgeid;
    edgeid = calc_edge_index(pid, cid);
    __edge_list_remove_edge(&d->edge_list, edgeid);
    return 0;
}

/**
 * TODO: edge_list_add_edge
 * Description: Add an edge into edge_list
 */
//int edge_list_add_edge(struct edge_list *list, struct edge *e)
//{
//    struct edge *last = NULL;
//
//    if (!list || !e) {
//        printf("edge_list or edge pointer is null \n");
//        return -1;
//    }
//    last = list->list->prev;
//    last->next = e;
//    e->next = list->list;
//    list->list->prev = e;
//    e->prev = last;
//    e->child->nparents++;
//    e->parent->nchildren++;
//    list->size++;
//    return 0;
//}

int
edge_list_add_edge(struct edge_list *list, int eid)
{
    //	list[eid].state = EDGE_STATE_VALID;
    //	list->size++;
    return eid;
}

/**
 * TODO: edge_list_remove_node
 * Description: remove all edge that having node as an end-point
 */
//int
//edge_list_remove_node(struct edge_list *list, struct node_info *node)
//{
//    struct edge *e = NULL;
//    struct edge *next = NULL;
//    int epid;
//    int npid;
//    int size;
//    int i;
//
//    if ((!list) || (!node)) {
//        //printf("node or edge_list pointer is null \n");
//        return -1;
//    }
//    if (list->size == 0)
//        return 0;
//    e = list->list;
//    next = e->next;
//    size = list->size;
//    npid = node->pid;
//    for (i=0; i<size; i++) {
//        epid = e->child->pid;
//        if (epid == npid) {
//           __edge_list_remove_edge(list, e);
//           continue;
//        }
//        epid = e->parent->pid;
//        if (epid == npid) {
//           __edge_list_remove_edge(list, e);
//           continue;
//        }
//        e = next;
//        next = e->next;
//    }
//    return 0;
//}

// TODO:
int
edge_list_remove_node(struct edge_list *list, int nid)
{
    return 0;
}

/* TODO: dag_nodelist_compare
 * Description: comparison function of avl tree
 */
//static int
//dag_nodelist_compare(const void *_n1, const void *_n2, void *avl_param)
//{
    //    struct node_info *n1 = (struct node_info*)_n1;
    //    struct node_info *n2 = (struct node_info*)_n2;
    //    if (n1->pid > n2->pid)
    //        return 1;
    //    if (n1->pid > n2->pid)
    //        return -1;
//    return 0;
//}

/**
 * TODO: dag_add_node
 * Description: wrapper of _dag_add_node
 */
int
dag_add_mpi_node(struct dag *d, int rank, int pid)
{
    //    struct node_info *node_info = malloc (sizeof (struct node_info));
    //    if (!node_info) {
    //        (*n) = NULL;
    //        printf("Cannot malloc node_info structure \n");
    //        return -1;
    //    }
    //    node_info->pid = pid;
    //    node_info->nchildren = 0;
    //    node_info->nparents = 0;
    //    node_info->children_edge_list = NULL;
    //    node_info->parent_edge_list = NULL;
    //    node_info->rank = -1;
    //    avl_insert(d->node_list, node_info);
    //    printf("dag-add-node: add %d, dag co %d node \n",pid, d->node_list->avl_count);
    //    (*n) = node_info;

    // TODO: Recheck
    d->node_list.list[rank].pid = pid;
    if (d->node_list.list[rank].state == NODE_STATE_VALID) {
        return 0;
    }
    d->node_list.list[rank].nparents = 0;
    d->node_list.list[rank].nchildren = 0;
    d->node_list.list[rank].state = NODE_STATE_VALID;
    d->node_list.size++;
    return 0;
}

/**
 * TODO: dag_remove_node
 * Description: wrapper of _dag_remove_node
 */
int
dag_remove_mpi_node(struct dag *d, int rank)
{
    //    struct node_info node_info;
    //    struct node_info *node = NULL;
    //
    //    node_info.pid = pid;
    //    node = avl_find(d->node_list, &node_info);
    //    if (node == NULL)
    //        return 0;
    //    //printf("dag_remove_node: edge_list = %d, pid = %d, ank = %d \n", d->edge_list, node->pid, node->rank);
    //    // remove all edges
    //    edge_list_remove_node(d->edge_list, node);
    //    avl_delete(d->node_list, &node_info);
    //    printf("dag-remove-node: remove %d, dag co %d node \n",pid, d->node_list->avl_count);

    int i;
    if (d->node_list.list[rank].state == NODE_STATE_VALID) {
        if (d->node_list.size <= 0)
            printf("dag-remove-mpinode: problem node size <= 0 \n");
        else
            d->node_list.size--;
    }
    d->node_list.list[rank].state = NODE_STATE_INVALID;
    d->node_list.list[rank].nchildren = 0;
    d->node_list.list[rank].nparents = 0;

    // adjust edge list
    for (i = 0; i < rank; i++) {
        int eid = calc_edge_index(i, rank);
        if (d->edge_list.list[eid].state == EDGE_STATE_VALID) {
            if (d->edge_list.size <= 0)
                printf("dag-remove-mpinode: problem edge size <= 0 \n");
            else {
                d->edge_list.size--;
                d->node_list.list[i].nchildren--;
            }
        }
        d->edge_list.list[eid].state = EDGE_STATE_INVALID;
    }

    for (i = rank + 1; i < MAX_NODES; i++) {
        int eid = calc_edge_index(rank, i);
        if (d->edge_list.list[eid].state == EDGE_STATE_VALID) {
            if (d->edge_list.size <= 0)
                printf("dag-remove-mpinode: problem edge size <= 0 \n");
            else {
                d->edge_list.size--;
                d->node_list.list[i].nparents--;
            }
        }
        d->edge_list.list[eid].state = EDGE_STATE_INVALID;
    }
    return 0;
}

/**
 * TODO: dag_add_edge
 * Description: wrapper of _dag_add_edge
 */
int
dag_add_edge(struct dag *d, int from_pid, int to_pid, OUT struct edge **e)
{
    //    struct edge *edge = NULL;
    //    struct node_info from;
    //    struct node_info to;
    //    struct node_info *parent = NULL;
    //    struct node_info *child = NULL;
    //
    //    edge = malloc(sizeof(struct edge));
    //    if (!edge) {
    //        printf("Cannot malloc an edge structure \n");
    //        return -1;
    //    }
    //    from.pid = from_pid;
    //    to.pid = to_pid;
    //    parent = avl_find(d->node_list, &from);
    //    child = avl_find(d->node_list, &to);
    //    edge->child = child;
    //    edge->parent = parent;
    //    edge->next = edge;
    //    edge->prev = edge;
    //    edge->weight = 0;
    //    edge->state = 0;
    //    edge_list_add_edge(d->edge_list, edge);
    return 0;
}

int
dag_add_mpi_edge(struct dag *d, int from_rank, int to_rank)
{
    int eid;
    if (!d || from_rank >= MAX_NODES || to_rank >= MAX_NODES) return -1;
    if (d->node_list.list[from_rank].state == NODE_STATE_INVALID) {
        d->node_list.list[from_rank].nchildren = 0;
        d->node_list.list[from_rank].nparents = 0;
        d->node_list.list[from_rank].state = NODE_STATE_VALID;
        d->node_list.size++;
    }
    if (d->node_list.list[to_rank].state == NODE_STATE_INVALID) {
        d->node_list.list[to_rank].nchildren = 0;
        d->node_list.list[to_rank].nparents = 0;
        d->node_list.list[to_rank].state = NODE_STATE_VALID;
        d->node_list.size++;
    }
    eid = calc_edge_index(from_rank, to_rank);
    if (d->edge_list.list[eid].state == EDGE_STATE_INVALID) {
        d->edge_list.list[eid].state = EDGE_STATE_VALID;
        d->edge_list.list[eid].child = &d->node_list.list[to_rank];
        d->edge_list.list[eid].parent = &d->node_list.list[from_rank];
        d->edge_list.size++;
        d->node_list.list[from_rank].nchildren++;
        d->node_list.list[to_rank].nparents++;
    }
    return 0;
}
/**
 * TODO: dag_remove_edge
 * Description: wrapper of _dag_remove_edge
 */
int
dag_remove_edge(struct dag *d, int from_pid, int to_pid)
{
    //    struct node_info from;
    //    struct node_info to;
    //    struct node_info *parent = NULL;
    //    struct node_info *child = NULL;
    //
    //    from.pid = from_pid;
    //    to.pid = to_pid;
    //    parent = avl_find(d->node_list, &from);
    //    child = avl_find(d->node_list, &to);
    //    edge_list_remove_edge(d->edge_list, parent, child);
    return 0;
}

int
dag_remove_mpi_edge(struct dag *d, int fromrank, int torank)
{
    int eid = calc_edge_index(fromrank, torank);
    if (d->edge_list.list[eid].state == EDGE_STATE_VALID) {
        d->edge_list.list[eid].state = EDGE_STATE_INVALID;
        if (d->edge_list.size <= 0) {
            printf("dag-remove-mpiedge: problem edge size <= 0 \n");
        } else {
            d->edge_list.size--;
            // TODO: Check node state (donot implement)
            d->node_list.list[fromrank].nchildren--;
            d->node_list.list[torank].nparents--;
        }
    }
    return 0;
}
/**
 * TODO: dag_get_node
 * Description:
 * @return: error code
 */
//int
//dag_get_node(struct dag *d, int pid, struct node_info **n)
//{
//    struct node_info node_info;
//
//    node_info.pid = pid;
//    (*n) = avl_find(d->node_list, &node_info);
//    return 0;
//}

//int
//dag_get_node(struct dag *d, int rank, struct node_info **n)
//{
//    (*n) = &d->node_list.list[rank];
//    if ((*n)->state == NODE_STATE_INVALID) return -1;
//    return 0;
//}

/**
 * TODO: dag_init
 * Description:
 */
//int
//dag_init(struct dag *d)
//{
//    d->node_list = avl_create(dag_nodelist_compare, NULL, NULL);
//    edge_list_init(d->edge_list);
//    return 0;
//}

int node_init(struct node_info *n)
{
    if (!n)
        return -1;
    n->nchildren = 0;
    n->nparents = 0;
    n->pid = -1;
    n->rank = -1;
    n->state = NODE_STATE_INVALID;
    return 0;
}

int edge_init(struct edge *e)
{
    if (!e)
        return -1;
    e->child = NULL;
    e->parent = NULL;
    e->state = EDGE_STATE_INVALID;
    e->weight = 0;
    return 0;
}

int
dag_init(struct dag *d)
{
    int i;
    for (i=0; i<MAX_NODES; i++)
        node_init(&(d->node_list.list[i]));
    for (i=0; i<MAX_EDGES; i++)
        edge_init(&(d->edge_list.list[i]));
    d->edge_list.size = 0;
    d->node_list.size = 0;
    return 0;
}

/**
 * TODO: dag_clean
 * Description:
 */
int
dag_clean(struct dag *d)
{
    //    avl_destroy(d->node_list, NULL);
    //    edge_list_destroy(d->edge_list);

    dag_init(d);
    return 0;
}
