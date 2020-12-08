#include <mpi.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
using namespace std;

int main( int argc, char **argv){

    int size, rank, n = 10000000;
    double pi=0.0, sum = 0.0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = rank ; i < n; i += size) sum +=  4.0/(1.0 + pow(((double)i + 0.5)/(double)n,2));

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Reduce(&sum, &pi, 1, MPI_DOUBLE, MPI_SUM, 0,MPI_COMM_WORLD);

    if(rank == 0){
        pi = pi/(double)n;
        printf("PI : %.24f\n", pi);
        printf("Error (24 decimales) : %.16f\n", abs(pi - 3.141592653589793238462643));
    }

    MPI_Finalize();
    return 0;
}
