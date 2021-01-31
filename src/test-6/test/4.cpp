#include <iostream>
#include <omp.h>
using namespace std;

int main(int argc, char **argv){
    int a, i;
    #pragma omp master
    a = 0;
    #pragma omp parallel shared(a) private(i) num_threads(4)
    {
        #pragma omp for schedule(dynamic) reduction(+:a) nowait
        for (i = 0; i < 10; i++) {
            a += i;
            //printf("%d, %d, %d\n",omp_get_thread_num(),i,a);
        }
        #pragma omp barrier
        #pragma omp single
        cout<<"La suma es: "<<a<<endl;
    }
    return 0;
}
