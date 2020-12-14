#include<iostream>
#include<mpi.h>

using namespace std;

double T1(double i){return i;}
double T2(double i){return i;}
double T3(double i){return i;}
double T4(double i){return i;}
double T5(double i){return i;}
double T6(double i){return i;}
double T7(double i){return i;}

void calculo(double i,double j,double k){
    double a,x,d,suma; int p,rank;

    MPI_Comm_size(MPI_COMM_WORLD,&p);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    //fase 1
    if(rank == 0) a=T1(i);
    else if(rank == 1) a=T2(j);
    else a=T3(k);
    MPI_Reduce(&a, &suma, 1, MPI_DOUBLE, MPI_SUM, 0,MPI_COMM_WORLD);

    //fase 2
    if (rank==0) d = T4(suma);
    MPI_Bcast(&d, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    //fase 3
    if(rank == 0) x=T5(a/d);
    else if(rank == 1) x=T6(a/d);
    else x=T7(a/d);

    //suma final
    MPI_Reduce(&x, &suma, 1, MPI_DOUBLE, MPI_SUM, 0,MPI_COMM_WORLD);
    if(rank==0) cout<<suma<<endl;
}
int main(int argc, char*argv[]){
    MPI_Init(&argc,&argv);
    calculo(21,42,32);
    MPI_Finalize();
}
