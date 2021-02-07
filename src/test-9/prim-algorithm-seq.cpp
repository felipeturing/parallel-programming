#include <iostream>
#include <limits.h> // brinda los limites de almacenamiento para diferentes tipos de datos
#include <time.h>
#include <stdlib.h>
using namespace std;

// Numero de vertices del grafo simple
#define V 9

//Cabeceras
int minKey(int key[], int visited[]);
void primMST(int **graph);
int printMST(int from[], int n, int **graph);



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

    clock_t begin = clock();
    primMST(graph);
    clock_t end = clock();

    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Tiempo de ejecución = %f\n", time_spent);

    return 0;
}


// función para buscar el vertice con la valor minimo de clave, del conjunto de vertices aun no incluidos en el MST
int minKey(int key[], int visited[]){
    // Inicializar el minimo valor
    int min = INT_MAX, min_index;
    int v;
    for (v = 0; v < V; v++)
    {
        if (visited[v] == 0 && key[v] < min)
        {
            min = key[v], min_index = v;
        }
    }
    return min_index;
}


// Una función para imprimir el MST almacenado en from[]
int printMST(int from[], int n, int **graph){
    int i;
    printf("Edge   Weight\n");
    for (i = 1; i < n; i++) printf("%d - %d    %d \n", from[i], i, graph[i][from[i]]);
    return 0;
}

void primMST(int **graph){
    int from[V];    // Array to store constructed MST
    int key[V];     // Key values used to pick minimum weight edge in cut
    int visited[V]; // To represent set of vertices not yet included in MST
    int i, count;
    // Initialize all keys as INFINITE
    for (i = 0; i < V; i++)
        key[i] = INT_MAX, visited[i] = 0;

    //start with first vertex
    key[0] = 0;   // Make key 0 so that this vertex is picked as first vertex
    from[0] = -1; // First node is always root of MST

    // The MST will have V vertices
    for (count = 0; count < V - 1; count++)
    {
        // Pick the minimum key vertex from the set of vertices
        // not yet included in MST
        int u = minKey(key, visited);

        // Add the picked vertex to the MST Set
        visited[u] = 1;
        int v;

        // Update key value and from index of the adjacent vertices of
        // the picked vertex. Consider only those vertices which are not yet
        // included in MST
        for (v = 0; v < V; v++)
            if (graph[u][v] && visited[v] == 0 && graph[u][v] < key[v])
                from[v] = u, key[v] = graph[u][v];
    }
     printMST(from, V, graph);
}
