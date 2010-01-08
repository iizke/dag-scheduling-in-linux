#ifndef PARSER_H
#define PARSER_H

struct cmdoptions {
    int nprocs; // number of processes
    int nice; // new nice of process
    int batch; // 1 is batch, 0 is non-batch
    char script_file[32]; // script file contains actions of all processes
    char output_file[32];
    int density;
};

int parser_arg(int argc, char **argv, struct cmdoptions *opts);

#endif /* PARSER_H */
