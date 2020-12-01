#include <mpi.h>   /* PROVIDES THE BASIC MPI DEFINITION AND TYPES */
#include <iostream>
using namespace std;

int main(int argc, char **argv) {

  int my_rank; 
  int size, i,ti,number;
  MPI_Status stat;
  
  MPI_Init(&argc, &argv); /*START MPI */
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); /*DETERMINE RANK OF THIS PROCESSOR*/
  MPI_Comm_size(MPI_COMM_WORLD, &size); /*DETERMINE TOTAL NUMBER OF PROCESSORS*/
  
  if (my_rank ==0) {
	number=-1;
	MPI_Send(&number,1,MPI_INT,1,0,MPI_COMM_WORLD); 
cout<<"Imprimiendo desde el rank "<<my_rank<<" que envia number= "<<number<<endl; 
}
  else if (my_rank==1){
	MPI_Recv(&number,1,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
cout<<"Imprimiendo desde el rank "<<my_rank<<" que recibe number= "<<number<<endl; 
  }
  
MPI_Finalize();  /* EXIT MPI */
  
}
