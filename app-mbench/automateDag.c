/*
 * automateDag.c
 * Create DAG Task Graph automatically
 *
 *  Created on: Feb 11, 2009
 *      Author: root
 */

#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "randf.h"

struct dag {
    int ntasks;
    int *edge_state_list;
    int density;
};

int mapping(int p, int c, int ntasks)
{
    int val = (2 * ntasks - 3 - p) * p + 2 * c;
    val = val / 2;
    return val;
}

int dag_generate(struct dag *d)
{
    int i, j;
    int size;
    int seed;

    if (d->ntasks <= 0)
        return -1;
    size = d->ntasks * d->ntasks;
    if (!d->edge_state_list) {
        d->edge_state_list = malloc(d->ntasks * d->ntasks * sizeof(int));
        if (!d->edge_state_list)
            return -1;
        for (i = 0; i < size; i++)
            d->edge_state_list[i] = 0;
    }
    seed = rand();
    for (i=0; i < (d->ntasks - 1); i++){
        for (j=i+1; j < d->ntasks; j++) {
            char state;
            state = my_srand(d->density);
            d->edge_state_list[i*d->ntasks + j] = state;
            d->edge_state_list[j*d->ntasks + i] = -state;
        }
    }
    size = d->ntasks * d->ntasks;
        for (i=0; i<size; i++)
            printf("i %d, val = %d \n",i,d->edge_state_list[i]);

    return 0;
}

int dag_export(struct dag *d, char *filename)
{
    FILE *f;
    int i;
    int j;
    char parent[256];
    char children[256];
    int size;

    f = fopen(filename, "w+");
    if (!f)
        return -1;
    if (d->ntasks <= 0)
        return -1;

    for (i=0; i < d->ntasks; i++){
        char *p_tmp = parent;
        char *c_tmp = children;
        int numchar;
        p_tmp[0] = '\0';
        c_tmp[0] = '\0';
        for (j=0; j < d->ntasks; j++) {
            int state = d->edge_state_list[i*d->ntasks + j];
            printf("i = %d, j = %d, index = %d, state = %d \n", i, j,i*d->ntasks + j,state);
            if (state > 0) {
                numchar = sprintf(c_tmp, "%d ", j);
                c_tmp += numchar;
                c_tmp[0] = '\0';
            } else if (state < 0) {
                numchar = sprintf(p_tmp, "%d ", j);
                p_tmp += numchar;
                p_tmp[0] = '\0';
            }
        }
        fprintf(f, "node \n");
        fprintf(f, "p %s \n", parent);
        fprintf(f, "c %s \n", children);
    }
    fclose(f);
    return 0;
}

int dag_init(struct dag *d, int ntasks)
{
    int i;
    int size;
    if (ntasks < 0)
        ntasks = 0;
    if (!d)
        return -1;
    d->ntasks = ntasks;
    d->density = 100;
    d->edge_state_list = malloc(d->ntasks * d->ntasks * sizeof(int));
    if (!d->edge_state_list)
        return -1;
    size = d->ntasks * d->ntasks;
    for (i = 0; i < size; i++)
        d->edge_state_list[i] = 0;
    return 0;
}

int main (int argc, char **argv)
{
    struct cmdoptions opts;
    struct dag dag;

    parser_arg(argc, argv, &opts);
    dag_init(&dag, opts.nprocs);
    dag.density = opts.density;
    dag_generate(&dag);
    dag_export(&dag, opts.output_file);

    return 0;
}
