#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
int main(int argc, char *argv[]){
MPI_Init(&argc, &argv);
int u_number;
int p_size;
MPI_Comm_rank(MPI_COMM_WORLD , &u_number);
MPI_Comm_size(MPI_COMM_WORLD , &p_size);
printf("Process %d in group of size %d \n", u_number, p_size);
MPI_Finalize();
}
