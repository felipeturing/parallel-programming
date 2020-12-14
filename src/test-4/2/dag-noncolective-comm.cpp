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

void calculo(double i,double j,double k)
{
    double a,b,c,d,x,y,z; int p,rank;
    MPI_Comm_size(MPI_COMM_WORLD,&p);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    // fase 1
    if (rank==0) {
        a = T1(i);
        MPI_Recv(&b, 1, MPI_DOUBLE, 1, 111, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&c, 1, MPI_DOUBLE, 2, 111, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else if (rank==1) {
        b = T2(j);
        MPI_Send(&b, 1, MPI_DOUBLE, 0, 111, MPI_COMM_WORLD);
    } else { /* rank==2 */
        c = T3(k);
        MPI_Send(&c, 1, MPI_DOUBLE, 0, 111, MPI_COMM_WORLD);
    }


    // fase 2
    if (rank==0) {
        d = T4(a+b+c);
    }

    // fase 3
    if (rank==0) {
        MPI_Send(&d, 1, MPI_DOUBLE, 1, 112, MPI_COMM_WORLD);
        MPI_Send(&d, 1, MPI_DOUBLE, 2, 112, MPI_COMM_WORLD);
        x = T5(a/d);
    } else if (rank==1) {
        MPI_Recv(&d, 1, MPI_DOUBLE, 0, 112, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        y = T6(b/d);
    } else { /* rank==2 */
        MPI_Recv(&d, 1, MPI_DOUBLE, 0, 112, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        z = T7(c/d);
    }


    //suma final
    if (rank==0) {
        MPI_Recv(&y, 1, MPI_DOUBLE, 1, 113, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&z, 1, MPI_DOUBLE, 2, 113, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else if (rank==1) {
        MPI_Send(&y, 1, MPI_DOUBLE, 0, 113, MPI_COMM_WORLD);
    } else { /* rank==2 */
        MPI_Send(&z, 1, MPI_DOUBLE, 0, 113, MPI_COMM_WORLD);
    }

    if(rank==0) cout<< x+y+z<<endl;
}

int main(int argc, char*argv[]){
MPI_Init(&argc,&argv);
calculo(21,42,32);
MPI_Finalize();
}
