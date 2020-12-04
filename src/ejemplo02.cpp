#include <mpi.h>   /* PROVIDES THE BASIC MPI DEFINITION AND TYPES */
#include <iostream>
using namespace std;

int main(int argc, char **argv) {

  int my_rank;
  int size, i,ti,number,number1,number2;
  MPI_Status stat;

  MPI_Init(&argc, &argv); /*START MPI */
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); /*DETERMINE RANK OF THIS PROCESSOR*/
  MPI_Comm_size(MPI_COMM_WORLD, &size); /*DETERMINE TOTAL NUMBER OF PROCESSORS*/

    number = 0;
  if (my_rank ==0) {
	number1=-1;
	number2=3;
	MPI_Send(&number1,1,MPI_INT,1,0,MPI_COMM_WORLD);
	MPI_Send(&number2,1,MPI_INT,2,0,MPI_COMM_WORLD);
    cout<<"Imprimiendo desde el rank "<<my_rank<<" que envia number= "<<number1<<" y "<<number2<<endl;
}
  else if (my_rank==1){
	MPI_Recv(&number1,1,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    cout<<"Imprimiendo desde el rank "<<my_rank<<" que recibe number= "<<number<<endl;
    //printf("Impriendo desde el rank",my_rank,"que envia number = ",number,"\n");
  }  else if (my_rank==2){
	MPI_Recv(&number2,1,MPI_INT,2,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    cout<<"Imprimiendo desde el rank "<<my_rank<<" que recibe number= "<<number<<endl;
    //printf("Impriendo desde el rank",my_rank,"que envia number = ",number,"\n");
  }else{
    cout<<"Imprimiendo desde el rank "<<my_rank<<" que recibe number= "<<number<<endl;
    //printf("Impriendo desde el rank",my_rank,"que envia number = ",number,"\n");
}

MPI_Finalize();  /* EXIT MPI */

}
