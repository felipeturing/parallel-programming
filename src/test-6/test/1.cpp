#include <iostream>
#include <omp.h>
using namespace std;

int main(int argc, char **argv){
    //thread master start
    int n_threads=4,id;
    //omp_set_num_threads(n_threads);

    //thread master end

    // fork start
    #pragma omp parallel private(id) num_threads(4) //mi pc solo tiene dos hilos por defecto
    {
        id = omp_get_thread_num();
        #pragma omp barrier//barrera que todos los hilos deben chocar antes de seguir avanzando
        if(id != 0) printf("Saludos desde el hilo %d\n", id); //printf no puede <<partirse>> como el operador sobrecargardo cout<<..<<endl
    }
    //end fork


    //thread master start
        cout<<"Saludos desde maestro\n"<<endl;//acá no hay problema de impresión desordenada puesto que esto se ejecuta solo en el maestro, que por defecto es el hilo 0
    //thread master end
    return 0;
}
