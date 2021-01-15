#include <iostream>
#include <omp.h>
using namespace std;

int main(int argc, char **argv){
    int a, i;
    #pragma omp parallel num_threads(4)
    {
        a = 0;
        #pragma omp for reduction(+:a) nowait
        for (i = 0; i < 10; i++) {
            a += i;
            printf("%d, %d, %d\n",omp_get_thread_num(),i,a);
        }
        //#pragma omp barrier
    }

    #pragma omp master
    cout<<"La suma es: "<<a<<endl;

    return 0;
}
// Como a no es una variable compartida y se le asignan el valor 0 en cada hilo e i tampoco es privada entonces ocurre que el resultado no es el correcto;
