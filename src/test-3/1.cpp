#include <mpi.h>
#include <iostream>
#include <time.h>

using namespace std;

int main(int argc, char **argv) {

    int size, rank, len = 1000;
    float global[len];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int partition = len/size;
    float local[partition];

    if(rank == 0){
        srand(time(NULL));
        for (int i = 0; i < len; i++ ) global[i]= 1 + rand()%(101 - 1);
    }

    MPI_Scatter(&global,partition, MPI_INT, &local, partition, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 1; i<partition ; i++) local[0] = local[0] + local[i];
    local[0]=local[0]/partition;

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Gather(&local, 1, MPI_INT, &global, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if(rank == 0){
        for (int i = 1; i<size ; i++) global[0] = global[0] + global[i];
        for (int i = len-1; i>(partition*size - 1); i--) global[0] = global[0] + global[i];// como la cantidad de data no siempre es multiplo de la cantidad de procesos, probablemente habra data no repartida aquí la añadimos al calculo del promedio global
        global[0]=global[0]/(size+(len-partition*size));

        cout<<"Proceso 0 : Repartiendo "<<partition<< " numeros a cada uno de el/los "<<size<<" proceso/s"<<endl;
        cout<<"Proceso 0 : Promedio total considerando los resultados de todos los procesos, mas "<< len-partition*size << " ultimo/s numero/s que no fue/ron repartido/s : " <<global[0]<<endl;
    }

    MPI_Finalize();
    return 0;
}
