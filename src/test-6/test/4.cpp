#include <iostream>
#include <omp.h>
using namespace std;

int main(int argc, char **argv){
    int a, i;

    #pragma omp master
    a = 0;

    #pragma omp parallel shared(a) num_threads(4) private(i)
    {
        #pragma omp for reduction(+:a) nowait
        for (i = 0; i < 10; i++) {
            a += i;
            printf("%d, %d, %d\n",omp_get_thread_num(),i,a);
        }
        #pragma omp barrier // Aquí debe ir la barrera puesto que necesite obtener el resultado de todos los hilos con el reduction para luego mostrar el resultado final, sino ejecuto esto no hay sincronización.
        #pragma omp single
            cout<<"La suma es: "<<a<<endl;
    }s
    return 0;
}
