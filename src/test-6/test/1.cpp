#include <iostream>
#include <cassert> // or assert.h
#include <omp.h>
using namespace std;

int main(int argc, char **argv){
    int i=5,j=6;
    #pragma omp master
    printf("master : (i,j) = (%d,%d)\n", i, j);
    #pragma omp parallel private(i,j) num_threads(4)
    {
        i = 1;
        j = 2;
        int *ptr_i = &i, *ptr_j = &j;
        assert(*ptr_i==1 && *ptr_j==2);
        printf("thread %d : (i,j) = (%d,%d)\n",omp_get_thread_num() ,i,j);
    }
    #pragma omp master
    {
        assert(i==1 && j==2);
        printf("master : (i,j) = (%d,%d)\n", i,j);
    }
    return 0;
}
