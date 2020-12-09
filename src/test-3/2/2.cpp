#include <mpi.h>
#include <iostream>
#include <time.h>
#include <stdio.h>
using namespace std;

int main(int argc, char **argv) {

    int size, rank, len = 1000, i;
    float global[len];
    double start, end;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int partition = len/size;
    float local[partition];

    if(rank == 0){
        srand(time(NULL));
        for (i = 0; i < len; i++ ) global[i]= 1 + rand()%(101 - 1);
    }

    MPI_Scatter(&global,partition, MPI_INT, &local, partition, MPI_INT, 0, MPI_COMM_WORLD);

    start = MPI_Wtime();
    for (i = 1; i<partition ; i++) local[0] = local[0] + local[i];
    local[0]=local[0]/partition;
    end = MPI_Wtime();

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Gather(&local, 1, MPI_INT, &global, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if(rank == 0){
        for (i = 1; i<size ; i++) global[0] = global[0] + global[i];
        for (i = len-1; i>(partition*size - 1); i--) global[0] = global[0] + global[i];// si hay data no repartida aqui la añadimos al calculo del promedio total.
        global[0]=global[0]/(size+(len-partition*size));
        printf("( \t%3.5f, \t%.10f, \t%d, \t%4d, \t%d)\n", global[0], end-start, size, partition, len-partition*size);
    }
    MPI_Finalize();
    return 0;
}
