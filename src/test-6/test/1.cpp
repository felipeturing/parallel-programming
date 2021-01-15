#include <iostream>
#include <cassert> // or assert.h
#include <stdlib.h>
#include <omp.h>
using namespace std;

int main(int argc, char **argv){

    int i=5,j=6;

    #pragma omp parallel private(i,j) num_threads(4)
    {
        i = 1;
        j = 2;
        int *ptr_i = &i;
        int *ptr_j = &j;
        assert(*ptr_i==1 && *ptr_j==2); //*ptr_i : para obtener el valor almacenado en la dirección de ptr_i
        printf("thread %d : (i,j) = (%d,%d)\n",omp_get_thread_num() ,i,j);
        //#pragma omp barrier
    }
    assert(i==1 && j==2); //aborta porque la región en paralelo no modifica las variables originales
    //printf("master : (i,j) = (%d,%d)\n", i,j);
    return 0;
}
