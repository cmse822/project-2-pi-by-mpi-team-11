#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
int main(int argc, char *argv[]){
MPI_Init(&argc, &argv);
char *node_name = malloc(sizeof(char)*MPI_MAX_PROCESSOR_NAME);
int m = MPI_MAX_PROCESSOR_NAME;
MPI_Get_processor_name(node_name,&m);
printf("Running on node %s \n", node_name);
MPI_Finalize();
}
