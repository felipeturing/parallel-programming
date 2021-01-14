#include <omp.h>
#include<iostream>
using namespace std;
#define N     10

int main()
{
int i;
float a[N], b[N], c[N], d[N];
/* Some initializations */
for (i=0; i < N; i++) {
  a[i] = i * 1;
  b[i] = i + 2;  }
omp_set_num_threads(4);

#pragma omp parallel shared(a,b,c,d) private(i)
  {
//cout<<omp_get_thread_num()<<endl;
  #pragma omp sections nowait //hay una barrera ímplicita al final de cada sección esto, a menos que se utilize la clausula nowait, poner nowait hace que la otra section se ejecute sin la necesidad que la sections <<anteriores>> terminen. Ojo : No se intercambia valores entre las secciones
    {
    #pragma omp section //cada hilo ejecuta exclusivamente solo una section
	    {
    for (i=0; i < N; i++){
      	c[i] = a[i] + b[i];
    	printf("seccion 1. Hilo: %i it: %i \n ",omp_get_thread_num(),i);
	    }
	  }
    #pragma omp section //cada hilo ejecuta exclusivamente solo una section
	    {
    for (i=0; i < N; i++){
        d[i] = a[i] * b[i];
    	printf("seccion 2. Hilo: %i it: %i \n ",omp_get_thread_num(),i);
   // printf("seccion 2. Hilo: %i \n",omp_get_thread_num());
	    }
	    }
    }  /* end of sections */
  }  /* end of parallel section */

//for (i=0; i < N; i++)
//	cout<<a[i]<<"\t"<<b[i]<<"\t"<<c[i]<<"\t"<<d[i]<<endl;
}
