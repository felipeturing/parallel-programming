#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <iostream>
using namespace std;

int  partition( float list[], int start, int end, int pivotIndex );
void quicksort( float list[], int start, int end );
void imprimir( float list[], int n );

int main( int argc, char **argv )
{
    int numprocs, rank, i, rc, namelen;
    unsigned int n;
	double start, end;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Init(&argc,&argv);
	MPI_Comm_size (MPI_COMM_WORLD,&numprocs);
	MPI_Comm_rank (MPI_COMM_WORLD,&rank);

	n = 52428;

	start = MPI_Wtime();
    float global[n];
	float short_bucket[numprocs][n];

	if( rank==0 ){
		MPI_Status status;
		unsigned int j, bucket_sizes[numprocs];			// Tamaños de cada bucket por proceso
		float bucket_lim = 1.0 / numprocs;

        srand(time(NULL));
		for( i=0; i<n; i++ ) global[i] = 1.0*rand() / RAND_MAX; //generar los números aleatorios
        //printf("Lista desordenada : ");
		//imprimir(global,n);


		for(i = 0; i < numprocs; i++) bucket_sizes[i] = 0; //inicializando los tamaños de cada bucket en cero

        //Particionando el vector global dentro de cada bucket
		for( i=0; i<n; i++ ){
			for(j=0; j<numprocs; j++){
				if((global[i] > (bucket_lim * j)) && (global[i] <= (bucket_lim * (j+1)))){
					short_bucket[j][bucket_sizes[j]] = global[i];
					bucket_sizes[j]++;
				}
			}
		}

		/*Enviar los buckets a cada proceso*/
		for(i=1; i<numprocs; i++){
			MPI_Send( &bucket_sizes[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD );//envias el tamaño de cada bucket
			MPI_Send( short_bucket[i], bucket_sizes[i], MPI_FLOAT, i, 0, MPI_COMM_WORLD );//envias el contenido de cada bucket
		}

		/*el proceso maestro ordena su bucket*/
		quicksort(short_bucket[0],0,bucket_sizes[0]);

		/*Recibir el bucket ordenado de cada proceso*/
		for(i=1; i<numprocs; i++){
		  MPI_Recv(short_bucket[i], bucket_sizes[i], MPI_FLOAT, i, 0, MPI_COMM_WORLD, &status );
		}

		/*Concatenar ordenadamente todos los buckets*/
		int index = 0;
		for(i=0; i<numprocs; i++){
			for(j=0; j< bucket_sizes[i]; j++){
				global[index++] = short_bucket[i][j];
			}
		}

		//printf("Lista ordenada : ");
		//imprimir(global,n);
	}
	else{
		int size;
		MPI_Status status;

		/*Recibe el tamaño de cada bucket*/
		MPI_Recv( &size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status );

		float bucket[size];
		MPI_Recv( bucket, size, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &status );

		//printf("P %d:  tamaño del bucket %d.  Elementos: ",rank,size);
		//imprimir(bucket,size);

		quicksort(bucket,0,size);
		//printf("P %d:  Elementos ordenados: ",rank);
		//imprimir(bucket,size);

		MPI_Send( bucket, size, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
	}


	end = MPI_Wtime();
	if(rank == 0)
		printf("Tiempo total de ejecución: %f segundos \n",end - start);
	MPI_Finalize();
	return 0;

}

void imprimir( float list[], int n )
{
	for( int i=0; i<n; i++ ) printf( " %g", list[i] );
	printf( "\n" );
}

int partition( float list[], int start, int end, int pivotIndex )
{
	float temp;
	float pivotValue = list[pivotIndex];

	list[pivotIndex] = list[end-1];
	list[end-1]      = pivotValue;

	int storeIndex = start, i;
	for( i=start; i<end-1; i++ )
	if( list[i] <= pivotValue ){
		temp = list[i]; list[i] = list[storeIndex]; list[storeIndex] = temp;
		storeIndex++;
	}
	temp = list[storeIndex]; list[storeIndex] = list[end-1]; list[end-1] = temp;
	return storeIndex;
}


void quicksort( float list[], int start, int end )
{
	if( end > start+1 ){
		int pivotIndex = start;
		int finalPivotIndex = partition( list, start, end, pivotIndex );
		quicksort( list, start, finalPivotIndex );
		quicksort( list, finalPivotIndex+1, end );
	}
}
