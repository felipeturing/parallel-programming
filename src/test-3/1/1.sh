#!/bin/bash
echo "Calculando PI variando procesos de 1 < p < 8  y manteniendo constante n = 10000."
mpic++ 1.cpp

echo -e "([PI], [TIEMPO], [PROCESOS], [INTERVALOS], [PRECISIÓN A 24 DECIMALES])\n"> out.txt
echo -e "Puntos para graficar t vs p con n = 10000\n" >> out.txt

for i in  2 3 4 5 6 7
do
    mpirun -np $i ./a.out 10000
done >> out.txt
echo Resultados en out.txt.

echo "Calculando PI variando número de intervalos de 10^2 < n < 10^7 y manteniendo constante p = 4"
echo -e "\n\nPuntos para graficar t vs n con p = 4\n" >> out.txt

for i in  3 4 5 6 7
do
    mpirun -np 4 ./a.out $((10**$i))
done >> out.txt
echo Resultados en out.txt.
