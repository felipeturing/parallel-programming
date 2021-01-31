#include <iostream>
#include <omp.h>
#include <time.h>
#define CHUNKSIZE 25
#define N 100

using namespace std;

int main(int argc, char **argv){
    int n[N], i, chunk=CHUNKSIZE, n_max, n_min;
    #pragma omp master
    {
        srand(time(NULL));
        for (i=0; i<N; i++) n[i] = 1 + rand()%(99 + 1 - 1);
        n_max = n[0];n_min = n[0];
    }
    #pragma omp parallel shared(n,n_max,n_min,chunk) private(i) num_threads(4)
    {
        #pragma omp for schedule(static, chunk) reduction(max:n_max) reduction(min:n_min) nowait
        for (i = 0; i < N; i++) {
            if(n[i] > n_max) n_max = n[i];
            if(n[i] < n_min) n_min = n[i];
        }
        #pragma omp barrier
        #pragma omp single
        {
            cout<<"El mín es: "<<n_min<<endl;
            cout<<"El max es: "<<n_max<<endl;
        }
    }
    return 0;
}
