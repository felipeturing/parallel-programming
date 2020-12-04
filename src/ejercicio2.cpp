#include <mpi.h>
#include <iostream>
#include <time.h>
#include <stdlib.h>
using namespace std;

int main(int argc, char **argv) {

    int size, rank, len = 1000;
    float globaldata[len];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int sizeofpartition = len/size;
    float localdata[sizeofpartition];

    if(rank == 0){
        srand(time(NULL));
        for (int i = 0; i < len; i++ ){
            globaldata[i]= 0 + rand()%(100 - 1);
        }
    }

    MPI_Scatter(&globaldata,sizeofpartition, MPI_INT, &localdata, sizeofpartition, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 1; i<sizeofpartition ; i++){
            localdata[0] = localdata[0] + localdata[i];
    }

    localdata[0]=localdata[0]/sizeofpartition;
    //cout<<"Proceso "<<rank<<" el promedio es  "<<localdata[0]<<endl;

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Gather(&localdata, 1, MPI_INT, &globaldata, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if(rank == 0){
        for (int i = 1; i<size ; i++){
            globaldata[0] = globaldata[0] + globaldata[i];
        }
        globaldata[0]=globaldata[0]/size;
        cout<<"Proceso "<<rank<<" el promedio total es  "<<globaldata[0]<<endl;
    }

    MPI_Finalize();
    return 0;
}
