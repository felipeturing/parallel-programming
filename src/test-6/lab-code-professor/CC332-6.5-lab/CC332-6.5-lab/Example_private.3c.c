/*
* @@name:	private.3c
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
*/
#include <assert.h>
#include <stdio.h>
#include <omp.h>

void priv_example3()
{
  int i, a;
  omp_set_nested(1);
  #pragma omp parallel num_threads(4) private(a)
  {
     a = 1;
    #pragma omp parallel for private(a,i)
    //#pragma omp for private(a,i)
      for (i=0; i<10; i++)
     {
       a = 2;
      printf("2: hilo: %d, a: %d\n",omp_get_thread_num(),a);
     }
      printf("1: hilo: %d, a: %d\n",omp_get_thread_num(),a);
//    assert(a == 1);
  }
}

int main(){
priv_example3();
}
