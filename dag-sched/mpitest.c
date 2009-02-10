/*
 * mpitest.c
 *
 *  Created on: Feb 8, 2009
 *      Author: roseman
 */


int main()
{
    // MPI_Init --> dsm_add_mpitask
    // MPI_SRecv --> dsm_add_connection(rank1, rank2), SRecv, dsm_remove_connection(rank1, rank2)
    // MPI_Finish --> dsm_remove_mpitask(rank1, rank2), Finish
    return 0;
}
