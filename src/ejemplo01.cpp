/*
 * compiling command : mpic++ filename.cpp
 * running command   : mpirun -np [number of nodes] ./a.out
 * */
#include "mpi.h"
#include <iostream>
using namespace std;

int main(int argc,char *argv[])
{
	int rank, size;:
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	cout<<"Hello World!"<<"I am "<< rank<<" of "<<size<<endl;
//	printf()
	MPI_Finalize();
}
