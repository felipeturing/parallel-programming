#include <iostream>
#include <omp.h>
using namespace std;

int main (int argc, char **argv){
   int nthreads = 4,id;
   omp_set_num_threads(nthreads);

    #pragma omp parallel private(id)
    {
        id = omp_get_thread_num();
    #pragma omp barrier
//    cout<<"Saludos desde el hilo = "<<id<<endl;
//    cout<<" con "<<omp_get_num_threads()<<" hilos"<<endl;
    printf("Saludos desde el hilo %d \n",id);
	if (id == 0){
	    nthreads = omp_get_num_threads();
        printf("Cantidad de hilos = %d \n",nthreads);
	    //cout<<"Cantidad de hilos = "<<nthreads<<endl;
 	}
   }
}
