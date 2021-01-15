#include <iostream>
#include <stdlib.h>
#include <omp.h>
#define CHUNKSIZE 25
#define N 100
using namespace std;

int main(int argc, char **argv){
    int n[N], i, chunk=CHUNKSIZE;
    for(i=0;i<N;i++) n[i]= 5*i ;

    #pragma omp parallel shared(n, chunk) private(i) num_threads(4)
    {
//        #pragma omp for ordered schedule(static, chunk) nowait
        #pragma omp for ordered schedule(dynamic) nowait
            for(i=0;i<N;i++){
                #pragma omp ordered
                {
                    printf("thread %d \t n[%d] : %d \n", omp_get_thread_num() ,i,n[i]);
                }
            }
        //#pragma omp barrier
    }
    return 0;
}
