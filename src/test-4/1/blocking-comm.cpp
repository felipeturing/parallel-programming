#include <mpi.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

using namespace std;

float averageCalculation(float array[], unsigned int length);
void pseudoRandomArray(float (&array)[1000000]);

int main( int argc, char **argv){
    int rank, size, i;
    float global[1000000], globalAverage, localAverage;
    double start, end;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    unsigned int partition = 1000000/size;
    float local[partition], average[size];

    /** FASE 1 :
    *   Generamos la data pseudo-aleatoria y enviamos cada subarray[i*partition : (i+1)*partition - 1] a cada proceso i.
    *   excepto al maestro i = 0, porque este ya tiene la data.
    **/
    if(rank == 0){
        pseudoRandomArray(global);
//        for (i=0;i<10;i++) printf("%f\n",global[i]);
        start = MPI_Wtime(); // vamos a incluir el tiempo de comunicación (non-blocking comm) y el tiempo de los calculos
        for (i = 1; i < size; i++) MPI_Send(&global[i*partition], partition, MPI_FLOAT, i, 111, MPI_COMM_WORLD);
    }

    /** FASE 2 :
    *   Si estamos en el maestro promediamos los primeros numeros, de acuerdo a la partición, y lo guardamos en su propio promedio <<local>>.
    *   Sino en cada proceso recibimos la data del maestro, calculamos un promedio <<local>> por cada proceso y lo enviamos al maestro.
    **/
    if(rank == 0){
        localAverage = averageCalculation(global, partition);
    }else{
        MPI_Recv(&local, partition, MPI_FLOAT, 0, 111, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        localAverage = averageCalculation(local, partition);
        MPI_Send(&localAverage, 1, MPI_FLOAT, 0, 112, MPI_COMM_WORLD);
    }

    /** FASE 3:
    *   Recibimos los promedios de cada proceso y lo guardamos en el avarage[.], vease que solo average[0] corresponde al promedio calculado por el maestro.
    *   Luego promediamos la data de avarage[.], cuyo resultado es el promedio total del vector de 1000000 números.
    **/
    if(rank == 0){
        // global average calculation
        average[0] = localAverage;
        for (i = 1; i < size; i++) MPI_Recv(&average[i], 1, MPI_FLOAT, i, 112, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        globalAverage = averageCalculation(average, size);
        end = MPI_Wtime();
    }

    /** FASE 4:
    *   Mostrar los resultados en el proceso maestro.
    **/
    if(rank == 0){
        printf("\t(%4.8f, \t%.10f)\n", globalAverage, end-start);
    }

    MPI_Finalize();
    return 0;
}

float averageCalculation(float array[], unsigned int length){
    float average = 0.0;
    for(unsigned int i=0; i<length; i++) average += array[i];
    return average/length;
}

void pseudoRandomArray(float (&array)[1000000]){
    srand(time(NULL));
    for (unsigned int i = 0; i < sizeof(array)/sizeof(float); i++) array[i] = rand()/(RAND_MAX/1000.00);
}
