/*
 * dagschedd.h
 *
 *  Created on: Feb 8, 2009
 *      Author: roseman
 */

#ifndef DAGSCHEDD_H_
#define DAGSCHEDD_H_

#define DAG_MSQ_ID          2009

/* DSM: DAG Scheduling Module */
int dsm_add_task(int pid);
int dsm_remove_task(int pid);
int dsm_add_mpitask(int rank);
int dsm_add_connection(int from_rank, int to_rank);
int dsm_remove_connection(int from_rank, int to_rank);

#endif /* DAGSCHEDD_H_ */
