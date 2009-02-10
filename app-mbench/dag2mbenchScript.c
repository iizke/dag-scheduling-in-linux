/*
 * dag2mbenchScript.c
 *
 *  Created on: Feb 10, 2009
 *      Author: root
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"

struct id_list {
    struct id_list *next;
    int id;
};

struct task {
    struct task *next;
    struct id_list *parent_list;
    struct id_list *children_list;
    int id;
};

struct task_list {
    int size;
    struct task *list;
};

int idlist_init(struct id_list *l)
{
    if (!l)
        return -1;
    l->id = -1;
    l->next = NULL;
    return 0;
}

int task_list_init(struct task_list *l)
{
    if (!l)
        return -1;
    l->size = 0;
    l->list = NULL;
    return 0;
}

int task_init(struct task *t)
{
    if (!t)
        return -1;
    t->children_list = NULL;
    t->id = -1;
    t->next = NULL;
    t->parent_list = NULL;
    return 0;
}

int idlist_insert(struct id_list *list, int id)
{
    struct id_list *node;

    node = malloc(sizeof(struct id_list));
    if (!node)
        return -1;
    node->id = id;
    node->next = list;
    list = node;
    return 0;
}

int task_list_insert(struct task_list *list, struct task *task)
{
    if (!task || !list)
        return -1;
    task->next = list->list;
    list->list = task;
    list->size++;
    return 0;
}

int task_insert_parent(struct task *t, int id)
{
    if (!t || id < 0)
        return -1;
    return idlist_insert(t->parent_list, id);
}

int task_insert_child(struct task *t, int id)
{
    if (!t || id < 0)
        return -1;
    return idlist_insert(t->children_list, id);
}

static int read_input_file(char *filename, struct task_list *task_list)
{
    FILE *f;
    int cur_task_id = -1;
    char cur_list = '0';
    struct task *task = NULL;
    int err = 0;
    int id;
    char line[256];

    f = fopen(filename, "r");
    if (f == NULL)
        return -1;
    /*
     * Read script file to build action table
     */
    while (1) {
        err = fgets(line, 256, f);
        if (err == NULL) {
            printf("Read script file completely, go to do now \n");
            break;
        }
        if (!strcmp(line, "node")) {
            cur_task_id++;
            task = malloc(sizeof(struct task));
            if (!task)
                break;
            task_init(task);
            task->id = cur_task_id;
            continue;
        }
        if (!strcmp(line, "p")) {
            cur_list = 'p'; // parent list
            continue;
        }
        if (!strcmp(line, "c")) {
            cur_list = 'c'; // child list
            continue;
        }
        if (!strcmp(line,"end")) {
            task_list_insert(task_list, task);
            continue;
        }
        id = atoi(line);
        if (cur_list == 'p'){
            task_insert_parent(task, id);
            continue;
        } else if (cur_list == 'c') {
            task_insert_child(task, id);
            continue;
        } else
            continue;
    } /* while */
    return 0;
}

static int export2mbenchScript(struct task_list *list, char *output_name)
{
    FILE *f;
    int i;
    struct task *task = NULL;
    struct id_list *idlist = NULL;

    if (!list)
        return -1;
    f = fopen(output_name, "rw");
    if (f == NULL)
        return -1;
    task = list->list;
    for (i=0; i<list->size; i++) {
        fprintf(f, "begin");
        idlist = task->children_list;
        while (idlist) {
            fprintf(f, "srecv 1 %d \n", idlist->id);
            idlist = idlist->next;
        }
        fprintf(f, "wait \n");
        fprintf(f, "doload 1 1\n");
        idlist = task->parent_list;
        while (idlist) {
            fprintf(f, "send 1 %d \n", idlist->id);
            idlist = idlist->next;
        }
        task = task->next;
    }
    fclose(f);
    return 0;
}

int main(int argc, char **argv)
{
    struct cmdoptions opts;
    struct task_list tasklist;
    parser_arg(argc, argv, &opts);
    read_input_file(opts.script_file, &tasklist);
    export2mbenchScript(&tasklist, opts.output_file);
    return 0;
}
