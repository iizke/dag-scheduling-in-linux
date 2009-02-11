/*
 * automateDag.c
 * Create DAG Task Graph automatically
 *
 *  Created on: Feb 11, 2009
 *      Author: root
 */

#include "parser.h"

struct edge {
    struct edge *next;
    struct edge *prev;
    int parent_id;
    int child_id;
};

struct edge_list {
    struct edge *list;
    int size;
};

struct dag {
    int ntasks;
    struct edge_list *elist;
};

int cal_edge_num(int ntasks, int density)
{
    int nedges = ntasks * (ntasks - 1) / 2;
    int val = nedges * density / 100;
    if (val < nedges)
        val++;
    else
        val = nedges;
    return val;
}

int mapping(int p, int c, int ntasks)
{
    int val = (2 * ntasks - 3 - p) + 2 * c;
    val = val / 2;
    return val;
}

int main (int argc, char **argv)
{
    struct cmdoptions opts;

    parser_arg(argc, argv, &opts);

    return 0;
}
