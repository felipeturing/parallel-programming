#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
using namespace std;

#define MAXPOINTS 1000
#define MAXSTEPS 1000
#define MINPOINTS 20
#define PI 3.14159265
#define RtoL 111
#define LtoR 112

//global variables
int nsteps=0, tpoints=0, nlocal=0;
double values_maestro[MAXPOINTS + 2], local_values[MAXPOINTS +2], local_old_values[MAXPOINTS +2], local_new_values[MAXPOINTS + 2];

// prototypes
void init_param(int size);
void init_line(int rank, int size);
void do_math(int i);
void update(int left, int right, int rank, int size);

// main method
int main(int argc, char **argv){
    int rank, size, right, left;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    init_param(size);
    //printf("%.d\n", nlocal);
    init_line(rank, size);

    /* //esto lo hice para que el rank cero se encargue de calcular la foto inicial de la onda y enviar al resto los valors.
    MPI_Scatter(&values[1],nlocal, MPI_DOUBLE, &local_values[1], nlocal, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    for(int i=1 ; i<=nlocal;i++) {printf("rank %d :", rank); printf("%.3f\n",local_values[i]);}
    MPI_Barrier(MPI_COMM_WORLD);*/

    //for(int i=1 ; i<=nlocal;i++) {printf("rank %d :", rank); printf("%.3f\n",local_values[i]);}
    right = rank + 1; left = rank - 1;


    MPI_Finalize();
    return 0;
}



void init_param(int size){
    tpoints = 9;
    nsteps = 2;
    nlocal = tpoints/size;
}

void init_line(int rank, int size){
    double x, k = 0.002 + rank * nlocal, tmp = size * nlocal - 1;
    //printf("%.3f\n", k);
    //printf("%f\n", tmp);
    for (int i = 1; i <= nlocal; i++){
        x = k / tmp;
        local_values[i] = sin(2.0 * PI * x);
        k = k + 1.0;
    }
    for (int i = 1 ; i <= nlocal; i++) local_old_values[i] = local_values[i];
}

void do_math(int i){
    double dtime= 0.3, c= 1.0, dx= 1.0, tau, sqtau;
    tau = (c * dtime / dx);
    sqtau = tau * tau;
    local_new_values[i] = (2.0 * local_values[i]) - local_old_values[i] + (sqtau * (local_values[i - 1] - (2.0 * local_values[i]) + local_values[i + 1]));
}

void update(int left, int right, int rank, int size){
    for ( int  i  =  1 ;  i  <=  nsteps ; i++){
        if(rank = 0 ){
            MPI_Send(&local_values[nlocal], 1, MPI_DOUBLE, right, LtoR, MPI_COMM_WORLD); //
            MPI_Recv(&local_values[nlocal + 1], 1, MPI_DOUBLE, right, RtoL , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        else if(rank = size - 1 ){
            MPI_Send(&local_values[1], 1, MPI_DOUBLE, left, RtoL, MPI_COMM_WORLD);
            MPI_Recv(&local_values[0], 1, MPI_DOUBLE, left, LtoR , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        else{
            MPI_Recv(&local_values[0], 1, MPI_DOUBLE, left, LtoR , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&local_values[nlocal+1], 1, MPI_DOUBLE, left, LtoR , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(&local_values[0], 1, MPI_DOUBLE, left, RtoL, MPI_COMM_WORLD); //
            MPI_Send(&local_values[nlocal], 1, MPI_DOUBLE, right, LtoR, MPI_COMM_WORLD); //
        }

        for (int j  = 1; j <= nlocal; j++){
            if ((j == 1 && rank == 0) || (j == nlocal && rank = size - 1)){
                local_new_values[j] = 0.0;
            }
            else{
                do_math(j);
            }
        }

        for (int j = 1; j <= tpoints; j++){
            local_old_values[j] = local_values[j];
            local_values[j] = local_new_values[j];
        }

    }
}
