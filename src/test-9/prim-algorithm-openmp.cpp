#include <iostream>
#include <limits.h> // brinda los limites de almacenamiento para diferentes tipos de datos
#include <omp.h>
#include <stdlib.h>
using namespace std;

// Numero de vertices del grafo simple
#define V 9

int num;
int minKey(int key[], int visited[]);
void printMST(int from[], int n, int **graph);
void primMST(int **graph);

int main(){
    //asignamos dinamicamente memoria para almacenar el grafo simple
    int **graph = (int **)malloc(V * sizeof(int *));
    for (int x=0; x<V; x++)
        graph[x] = (int *)malloc(V * sizeof(int));

    //iteradores
    int i, j;

    //generar la matriz de adyacencia dado el grafo simple de ejemplo (9 vertices, 14 aristas)
    graph[1][0]=4;
    graph[0][7]=8;
    graph[1][7]=11;
    graph[1][2]=8;
    graph[2][8]=2;
    graph[7][8]=7;
    graph[8][6]=6;
    graph[7][6]=1;
    graph[6][5]=2;
    graph[2][3]=7;
    graph[2][5]=4;
    graph[3][5]=14;
    graph[3][4]=9;
    graph[4][5]=10;

    //El grafo simple no contiene loops (es un grafo simple)
    for (i = 0; i < V; i++) graph[i][i] = 0;

    //La matriz de adyacencia de un grafo simple (no dirigido) es simétrica
    for (i = 0; i < V; i++) for (j = 0; j < V; j++) graph[j][i] = graph[i][j];

    // Imprimir la matriz de adyacencia del grafo simple
    for (i = 0; i < V; i++){
        for (j = 0; j < V; j++) printf("%d\t", graph[i][j]);
        printf("\n");
    }

    double start = omp_get_wtime();
    primMST(graph);
    double end = omp_get_wtime();
    printf("Time for par = %f\nThreads = %d\n", end - start, num);

    return 0;
}

int minKey(int key[], int visited[])
{
    int min = INT_MAX, index, i;
    #pragma omp parallel num_threads(4)
    {
        num = omp_get_num_threads();
        int index_local = index;
        int min_local = min;
        #pragma omp for nowait
            for (i = 0; i < V; i++)
            {
                if (visited[i] == 0 && key[i] < min_local)
                {
                    min_local = key[i];
                    index_local = i;
                }
            }
//        #pragma omp barrier
        #pragma omp critical
        {
            if (min_local < min)
                {
                    min = min_local;
                    index = index_local;
                }
            }
    }
    return index;
}

void printMST(int from[], int n, int **graph)
{
    int i;
    printf("Edge   Weight\n");
    for (i = 1; i < n; i++) printf("%d - %d    %d \n", from[i], i, graph[i][from[i]]);
}

void primMST(int **graph)
{
    int from[V];
    int key[V];
    int visited[V];
    int i, count;
    for (i = 0; i < V; i++) key[i] = INT_MAX, visited[i] = 0;

    key[0] = 0;
    from[0] = -1;

    for (count = 0; count < V - 1; count++)
    {
        int u = minKey(key, visited);
        visited[u] = 1;

        int v;
#pragma omp parallel for schedule(static)
        for (v = 0; v < V; v++)
        {
            if (graph[u][v] && visited[v] == 0 && graph[u][v] < key[v])
                from[v] = u, key[v] = graph[u][v];
        }
    }
}
