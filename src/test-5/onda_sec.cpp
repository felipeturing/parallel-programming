#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
using namespace std;

#define MAXPOINTS 1000
#define MAXSTEPS 1000
#define MINPOINTS 20
#define PI 3.14159265

//global variables
int nsteps=0, tpoints=0;
double values[MAXPOINTS + 2], oldval[MAXPOINTS + 2], newval[MAXPOINTS + 2];

// prototypes
void init_param(void);
void init_line(void);
void do_math(int i);
void update(void);

// main method
int main(int argc, char *argv[])
{
    init_param();
    init_line();
    update(); // en este método también mostramos la foto inicial y la foto de la onda en cada paso
    return 0;
}


//definitions
void init_param(void)
{
    tpoints = 800; // por cada paso (i.e por cada foto) vamos a 50 puntos de la función.
    nsteps = 1000; // estas son como 300 fotos de la onda que vamos a considerar en tiempos t = k,2k,3k,etc.
    printf("Using points = %d, steps = %d\n", tpoints, nsteps);
}

void init_line(void)
{
    double x, k=0.002, tmp=tpoints - 1;

    // setting la foto inicial
    for (int j = 1; j <= tpoints; j++){
        x = k / tmp;
        values[j] = sin(2.0 * PI * x); // foto inicial es f(x) = sin(2pix) con condiciones de frontera values[0]=0 y values[tpoints]=0
        k = k + 1.0;
    }

    //la foto inicial se guardar en los valores antiguos(t-dt) puesto que a manipulares los valores en el tiempo t
    for (int i = 1; i <= tpoints; i++){
        oldval[i] = values[i];
    }

    printf("Foto inicial de la onda \n");
    for (int k = 1; k <= tpoints; k++){
            printf("%.3f\n", values[k]);
    }
}
void do_math(int i)
{
    double dtime= 0.3, c= 1.0, dx= 1.0, tau, sqtau;

    tau = (c * dtime / dx);
    sqtau = tau * tau;

    // la discretización
    newval[i] = (2.0 * values[i]) - oldval[i] + (sqtau * (values[i - 1] - (2.0 * values[i]) + values[i + 1]));
}
void update(void)
{
    //actualizar los punto por cada foto y por cada punto
    for ( int  i  =  1 ;  i  <=  nsteps ; i++){
        for (int j  = 1; j <= tpoints; j++){
            if ((j == 1) || (j == tpoints)){
                newval[j] = 0.0;
            }
            else{
                do_math(j);
            }
        }
        // pasamos lo actual a los pasado y lo calculado nuevo a lo actual
        for (int j = 1; j <= tpoints; j++){
            oldval[j] = values[j];
            values[j] = newval[j];
        }
        printf("Foto de la onda en el paso : %d\n", i);
        for (int k = 1; k <= tpoints; k++)
        {
            printf("%.3f\n", values[k]);
        }
    }
}
