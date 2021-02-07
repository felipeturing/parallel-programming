#include <iostream>
#include <time.h>
#include <math.h>
#include "mpi.h"


int funcionDeComparacion(const void * a, const void * b);
void compararArriba(int j);
void compararAbajo(int j);
void SequentialSort(void);
void imprimir(int len);

int n = 524288;// n = 2^k
int rank, array[524288], array_size; //variables globales

int main(int argc, char **argv) {
    double start, end;
    int i, j, numprocs;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    array_size = n / numprocs;

    // Cada proceso genera su vector de elementos aleatorios (menos gasto frente a que el master lo genere todo)
    srand(time(NULL));
    for (i = 0; i < array_size; i++) array[i] = rand() % (100);

    MPI_Barrier(MPI_COMM_WORLD); // Bloquear hasta que todos los procesos hayan generado sus vectores

    int dimensions = (int)(log2(numprocs));

    if (rank == 0) {
        start = MPI_Wtime();
    }

    // Ordenamiento secuencial de cada vector en cada proceso
    qsort(array, array_size, sizeof(int), funcionDeComparacion);

    // Ordenamiento Bitonic
    for (i = 0; i < dimensions; i++) {
        for (j = i; j >= 0; j--) {
            if (((rank >> (i + 1)) % 2 == 0 && (rank >> j) % 2 == 0) || ((rank >> (i + 1)) % 2 != 0 && (rank >> j) % 2 != 0)) {
                compararAbajo(j); //comparar con el de abajo
            } else {
                compararArriba(j); //comparar con el de arriba
            }
        }
    }

    // Bloquear hasta que todos los procesos hayan terminado su ordenamiento
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        end = MPI_Wtime();
        printf("Tiempo total de ejecución: %f\n", end - start);
    }

    MPI_Finalize();
    return 0;
}

void imprimir(int len){
    for (int i = 0; i < len; i++) {
            printf("%d ",array[i]);
    }
    printf("\n");
}

int funcionDeComparacion(const void * a, const void * b) {
    return ( * (int *)a - * (int *)b );
}


void compararAbajo(int j) {
    int i, min;
    int send_counter = 0;
    int buffer_send[array_size + 1];
    MPI_Send(&array[array_size - 1],1,MPI_INT,rank ^ (1 << j), 0, MPI_COMM_WORLD);

    int recv_counter;
    int buffer_recieve[array_size + 1];
    MPI_Recv(&min,1,MPI_INT,rank ^ (1 << j),0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

    for (i = 0; i < array_size; i++) {
        if (array[i] > min) {
            buffer_send[send_counter + 1] = array[i];
            send_counter++;
        } else {
            break;
        }
    }

    buffer_send[0] = send_counter;
    MPI_Send(buffer_send,send_counter,MPI_INT,rank ^ (1 << j), 0, MPI_COMM_WORLD);
    MPI_Recv(buffer_recieve, array_size,MPI_INT, rank ^ (1 << j),   0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);

    for (i = 1; i < buffer_recieve[0] + 1; i++) {
        if (array[array_size - 1] < buffer_recieve[i]) {
            array[array_size - 1] = buffer_recieve[i];
        } else {
            break;
        }
    }
    qsort(array, array_size, sizeof(int), funcionDeComparacion);
}


void compararArriba(int j) {
    int i, max;
    int recv_counter;
    int buffer_recieve[array_size + 1];
    MPI_Recv(&max, 1, MPI_INT,rank ^ (1 << j), 0,MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    int send_counter = 0;
    int buffer_send[array_size + 1];
    MPI_Send(&array[0],1,MPI_INT,rank ^ (1 << j),0,MPI_COMM_WORLD);

    for (i = 0; i < array_size; i++) {
        if (array[i] < max) {
            buffer_send[send_counter + 1] = array[i];
            send_counter++;
        } else {
            break;
        }
    }

    MPI_Recv(buffer_recieve,array_size,MPI_INT,rank ^ (1 << j),0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    recv_counter = buffer_recieve[0];

    buffer_send[0] = send_counter;
    MPI_Send(buffer_send,send_counter,MPI_INT,rank ^ (1 << j),0,MPI_COMM_WORLD);


    for (i = 1; i < recv_counter + 1; i++) {
        if (buffer_recieve[i] > array[0]) {
            array[0] = buffer_recieve[i];
        } else {
            break;
        }
    }

    qsort(array, array_size, sizeof(int), funcionDeComparacion);
}
