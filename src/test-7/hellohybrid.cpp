#include <stdio.h>
#include <omp.h>
#include "mpi.h"

using namespace std;

int main(int argc, char *argv[]) {
  int numprocs, rank, namelen;
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  int iam = 0, np = 1;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Get_processor_name(processor_name, &namelen);

  #pragma omp parallel default(shared) private(iam, np) num_threads(2)
  {
//    np = omp_get_num_threads();
    iam = omp_get_thread_num();
//    printf("Hola desde el hilo %d del total de %d hilos, del proceso %d del total de %d proceso en  %s\n", iam, np, rank, numprocs, processor_name);
    printf("Hola desde el hilo %d en el proceso %d en la computadora %s\n", iam, rank, processor_name);
  }

  MPI_Finalize();
  return 0;
}
