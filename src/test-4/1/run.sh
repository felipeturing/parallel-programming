#!/bin/bash
echo Comparando comunicación bloqueando y no-bloqueando.
echo Se hace 10 mediciones del tiempo en cada iteración por tipo de comunicación y número de procesos p=1, 2, 4 y 8.

mpic++ nonblocking-comm.cpp -o nonblock
mpic++ blocking-comm.cpp -o block

echo -e "Formato : ([PROMEDIO TOTAL], [TIEMPO])\n" > results.txt
for i in 1 2 4 8
do
    echo -e "Número de procesos : $i\n"
    echo  -e "\tBlocking"
    for j in 1 2 3 4 5 6 7 8 9 10
    do
        mpirun -np $i ./block
    done
    echo -e "\n"
    echo -e "\tNon-Blocking"
    for j in 1 2 3 4 5 6 7 8 9 10
    do
        mpirun -np $i ./nonblock
    done
    echo -e "\n\n"
done >> results.txt
echo Resultados en results.txt.
