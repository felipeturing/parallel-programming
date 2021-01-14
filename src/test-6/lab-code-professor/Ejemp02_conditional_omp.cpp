#include <iostream>
#include<omp.h>
#define CHUNKSIZE 2
#define N 20

using namespace std;

int main(int argc, char *argv[])
{
int i, chunk;
float a[N], b[N], c[N];
omp_set_num_threads(4);
int TID = omp_get_thread_num();
cout<<"Hilo maestro es "<<TID<<endl;

for (i=0; i < N; i++){
  	a[i] = 2.0*i;
	b[i] = i * 1.0;
}
chunk = CHUNKSIZE;

#pragma omp parallel shared(a,b,c,chunk) private(i) // el iterador i generalmente siempre es del tipo privado
//#pragma omp parallel shared(a,b,c) private(i)
  {
  #pragma omp for schedule(static,chunk) nowait
  //#pragma omp for schedule(dynamic) nowait // using dynamic no me preocupo por que sea divisible la cantidad de data con el númeo de hilos, lo que si debería pasar usando static, sin embago en este último cada chunk(trozo) de tamaño CHUNK_SIZE se distribuye en cada hilo en orden circular
  for (int i=0; i < N; i++){
    c[i] = a[i] + b[i];
    printf("hilo activo: %i %i\n",omp_get_thread_num(),i);
  }
//  cout<<"hilo activo: "<<omp_get_thread_num()<<endl;

  }  /* end of parallel section */


//for (i=0; i < N; i++)
//	cout<<a[i]<<"\t"<<b[i]<<"\t"<<c[i]<<endl;

}
