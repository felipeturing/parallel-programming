#include <mpi.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

using namespace std;

int main( int argc, char **argv){

    int size, rank, n = atoi(argv[1]);
    double pi=0.0, sum = 0.0, start, end;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    start=MPI_Wtime();
    for (int i = rank ; i < n; i += size) sum +=  4.0/(1.0 + pow(((double)i + 0.5)/(double)n,2));
    sum *= 1.0/(double)n;
    end=MPI_Wtime();

    MPI_Barrier(MPI_COMM_WORLD); //One of the primary uses of MPI_Barrier is to synchronize a program so that portions of the parallel code can be timed accurately.

    /** T4 :
        MPI_Reduce(&datasend, &result, count, MPI_FLOAT, MPI_SUM, dest, MPI_COMM_WORLD);
    **/
    MPI_Reduce(&sum, &pi, 1, MPI_DOUBLE, MPI_SUM, 0,MPI_COMM_WORLD);

    if(rank == 0){
        printf("(\t%.24f, \t%.8f, \t%d, \t%8d, \t%.24e)\n", pi, end-start, size, n, abs(pi - 3.141592653589793238462643));
    }
    MPI_Finalize();
    return 0;
}
