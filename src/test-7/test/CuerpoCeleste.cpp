#include <stdio.h>
#include <omp.h>
#include <mpi.h>
#define N 10

class CuerpoCeleste
{
    public:
    float pos[3][N];
    float vel[3][N];
    float m[N];
}

int main(int argc, char const *argv[])
{
    return 0;
}
