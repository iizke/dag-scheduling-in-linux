/*
 * automateDag.c
 * Create DAG Task Graph automatically
 *
 *  Created on: Feb 11, 2009
 *      Author: root
 */

#include "parser.h"
#include "randf.h"

struct dag {
    int ntasks;
    int *edge_state_list;
    int density;
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
    int val = (2 * ntasks - 3 - p) * p + 2 * c;
    val = val / 2;
    return val;
}

int unmapping(int ntasks, int *p, int *c)
{
    return 0;
}

int dag_generate(struct dag *d)
{
    int i;
    int size;
    int seed;

    if (ntasks <= 0)
        return -1;
    if (!d->edge_state_list) {
        d->edge_state_list = malloc(d->ntasks * sizeof(int));
        if (!d->edge_state_list)
            return -1;
    }
    size = d->ntasks * (d->ntasks - 1) / 2;
    for (i=0; i<size; i++){
        seed = rand();
        d->edge_state_list[i] = srand(seed, density);
    }
    return 0;
}

int dag_export(struct dag *d, char *filename)
{
    FILE *f;
    return 0;
}

int dag_init(struct dag *d, int ntasks)
{
    if (ntasks < 0)
        ntasks = 0;
    if (!d)
        return -1;
    d->ntasks = ntasks;
    d->edge_state_list = malloc(d->ntasks * sizeof(int));
    d->density = 100;
    return 0;
}


int main (int argc, char **argv)
{
    struct cmdoptions opts;
    struct dag dag;

    parser_arg(argc, argv, &opts);
    dag_init(&dag, opts.nprocs);
    dag_generate(&dag);
    dag_export(&dag, opts.output_file);

    return 0;
}
