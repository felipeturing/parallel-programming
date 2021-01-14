#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <mpi.h>
#include <time.h>
#define MAX_LENGHT_ARRAY_INT
using namespace std;

int main( int argc, char **argv){
    int v_local[MAX_LENGHT_ARRAY_INT], size, rank, n_local, max_local, min_local;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if(rank==0){
        int n_global = 10, max_global, min_global,v_global[MAX_LENGHT_ARRAY_INT];
        srand(time(NULL));
        printf("\n");
        for(int i=0; i<n_global;i++){
            v_global[i]= rand()%10;
            printf("%d", v_global[i]);
        }
    }

    MPI_Bcast(&n_local,1,n_global/size,1,MPI_INT,0,MPI_COMM_WORLD) // envio a cada p  el n_local
    MPI_Scather(&v_local, n_local,v_global,n_local,MPI_INT,0,MPI_COMM_WORLD); //envio a cada p  n_local elemento del vector v_global sucesivamente

    hallar_max_by_process(v_local, n_local, &max_local);
    hallar_min_by_process(v_local, n_local, &min_local);


    MPI_Reduce(max_global,1,max_local,1, MPI_MAX, MPI_COMM_WORLD);

    if(rank ==0) printf("El max y el min es %d y %d respectivamente.",max_global,min_global);

    return 0;
}
