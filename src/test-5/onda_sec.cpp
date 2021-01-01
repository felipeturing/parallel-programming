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
double values[MAXPOINTS + 2]={0}, oldval[MAXPOINTS + 2]={0}, newval[MAXPOINTS + 2]={0};

// prototypes
void init_param(void);
void init_line(void);
void do_math(int i);
void update(void);
void print_onda(void);

// main method
int main(int argc, char **argv){
    init_param();
    init_line(); // en este método también podríamos mostrar la foto inicial de la onda
    update(); // en este método también podríamos mostrar la foto de la onda por cada step, para realizar la animación
    print_onda();
    return 0;
}


//definitions
void init_param(void){
    tpoints = 800;
    nsteps = 1000;
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

    //la foto inicial se guardar en los valores antiguos puesto que a manipularemos los valores en el tiempo t
    for (int i = 1; i <= tpoints; i++){
        oldval[i] = values[i];
    }

    /*printf("Foto inicial de la onda \n");
    print_onda();*/
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
        // pasamos lo actual a lo pasado y lo nuevo a lo actual
        for (int j = 1; j <= tpoints; j++){
            oldval[j] = values[j];
            values[j] = newval[j];
        }

        /*printf("Foto de la onda en el paso : %d\n", i);
        print_onda();
        */
    }
}

void print_onda(void){
    double x, k=0.002, tmp = tpoints - 1;// esto me va servir para graficar los puntos en el eje del tiempo
    for (int j = 1; j <= tpoints; j++){
        x = k / tmp;
        printf("(%.7f,\t %.7f)\n", 2.0 * PI * x , values[j]);//imprime los puntos de la onda
        k = k + 1.0;
    }
}
