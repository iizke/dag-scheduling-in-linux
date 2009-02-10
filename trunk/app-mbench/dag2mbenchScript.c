/*
 * dag2mbenchScript.c
 *
 *  Created on: Feb 10, 2009
 *      Author: root
 */
#define __GNU_SOURCE
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

    if (!list)
        return -1;
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

int string2idlist(char *s, int size, struct id_list *list)
{
    return 0;
}

static int read_input_file(char *filename, struct task_list *task_list)
{
    FILE *f;
    int count = 0;
    int err = 0;
    char node[8];
    char line[256];

    f = fopen(filename, "r");
    if (f == NULL)
        return -1;
    /*
     * Read script file to build action table
     */
    while (1) {
        int size;
        struct task *task = malloc(sizeof(struct task));
        task_init(task);
        err = getline(&node, &size, f);
        if (err == -1) {
            printf("Read script file completely, go to do now \n");
            break;
        }

        if (!strcmp(node, "node")) {
            count++;
            task->id = count;
            getline(&line, &size, f);
            string2idlist(line, size, task->parent_list);
            getline(&line, &size, f);
            string2idlist(line, size, task->parent_list);
            task_list_insert(task_list, task);
        }
    } /* while */
    return 0;
}

static int export2mbenchScript(struct task_list *tasklist, char *output_name)
{
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
