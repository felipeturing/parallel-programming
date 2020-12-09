#!/bin/bash
echo Calculando el promedio de 1000 números aleatorios entre 1 y 100, en 1 a 8 procesos.
mpic++ 2.cpp

echo -e "([PROMEDIO TOTAL], [TIEMPO], [PROCESOS],[CANTIDAD DE NUMEROS REPARTIDOS POR PROCESO],[CANTIDAD DE NUMEROS NO REPARTIDOS])\n"> out.txt
echo -e "Puntos para graficar t vs p\n" >> out.txt
for i in 1 2 3 4 5 6 7 8
do
	mpirun -np $i ./a.out
done >> out.txt
echo -e "\n\nNota : Los números no repartidos también se promedian al final, en el proceso maestro." >> out.txt
echo Resultados en out.txt.
