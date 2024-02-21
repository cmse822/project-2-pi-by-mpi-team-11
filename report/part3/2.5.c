#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
int main(int argc, char *argv[]){
MPI_Init(&argc, &argv);
int u_number;
int p_size;
MPI_Comm_rank(MPI_COMM_WORLD , &u_number);
if(u_number==0)
{
MPI_Comm_size(MPI_COMM_WORLD , &p_size);
printf("size of group of is %d , this is reported by process %d \n", p_size, u_number);
}MPI_Finalize();
}

