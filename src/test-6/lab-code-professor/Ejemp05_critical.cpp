#include <omp.h>
#include <stdio.h>
int main()
{
int x;
x = 0;
omp_set_num_threads(4);
#pragma omp parallel shared(x)
  {
  #pragma omp critical
  x = x + 1;
printf("valor de x : %i en hilo %i de %i hilos \n",x,omp_get_thread_num(),omp_get_num_threads());
  }  /* end of parallel section */
printf("valor de x : %i  \n",x);

}
