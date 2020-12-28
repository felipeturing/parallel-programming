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
double master_root_rank_values[MAXPOINTS + 2]={0}, local_values[MAXPOINTS +2]={0}, local_old_values[MAXPOINTS +2]={0}, local_new_values[MAXPOINTS + 2]={0};

// prototypes
void init_param(int size);
void init_line(int rank, int size);
void do_math(int i);
void update(int left, int right, int rank, int size);
void print_foto_final_de_la_onda(void);

// main method
int main(int argc, char **argv){
    int rank, size, right, left;
    double start, end;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    start = MPI_Wtime();
        init_param(size);
        init_line(rank, size);
        right = rank + 1; left = rank - 1;
        update(left, right, rank, size);
    end = MPI_Wtime();
    if(rank == 0) {print_foto_final_de_la_onda(); printf("\n\n Runtime : %.10f\n", end-start);}
    MPI_Finalize();
    return 0;
}

// functions definition
void init_param(int size){
    tpoints = 800;
    nsteps = 1000;
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
        if(rank == 0 ){
            MPI_Send(&local_values[nlocal], 1, MPI_DOUBLE, right, LtoR, MPI_COMM_WORLD);
            MPI_Recv(&local_values[nlocal + 1], 1, MPI_DOUBLE, right, RtoL , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        else if(rank == size - 1 ){
            MPI_Send(&local_values[1], 1, MPI_DOUBLE, left, RtoL, MPI_COMM_WORLD);
            MPI_Recv(&local_values[0], 1, MPI_DOUBLE, left, LtoR , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        else{
            MPI_Send(&local_values[1], 1, MPI_DOUBLE, left, RtoL, MPI_COMM_WORLD);
            MPI_Send(&local_values[nlocal], 1, MPI_DOUBLE, right, LtoR, MPI_COMM_WORLD);
            MPI_Recv(&local_values[0], 1, MPI_DOUBLE, left, LtoR , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&local_values[nlocal+1], 1, MPI_DOUBLE, right, RtoL , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        for (int j  = 1; j <= nlocal; j++){
            if ((j == 1 && rank == 0) || (j == nlocal && rank == size - 1)){
                local_new_values[j] = 0.0;
            }
            else{
                do_math(j);
            }
        }

        for (int j = 1; j <= nlocal; j++){
            local_old_values[j] = local_values[j];
            local_values[j] = local_new_values[j];
        }

        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Gather(&local_values[1], nlocal, MPI_DOUBLE, &master_root_rank_values[1], nlocal, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        /*if(rank == 0){
            printf("Foto de la onda en el paso : %d\n", i);
            print_foto_final_de_la_onda();
        }*/
    }
}

void print_foto_final_de_la_onda(void){
    double x, k=0.002, tmp = tpoints - 1;// esto me va servir para graficar los puntos en el eje del tiempo
    for (int j = 1; j <= tpoints; j++){
        x = k / tmp;
        printf("(%.7f,\t %.7f)\n", 2.0 * PI * x , master_root_rank_values[j]);
        k = k + 1.0;
    }
}
