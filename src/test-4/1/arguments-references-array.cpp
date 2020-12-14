#include <stdio.h>
#include <string.h>
#include <iostream>

using namespace std;

void plusOne(int (&array)[4]);

int main(int argc, char **argv){
    int array[4] = {1,2,3,4};
    plusOne(array);
    for(int i=0;i<4;i++) printf("%d\n", array[i]);
}

void plusOne(int (&array)[4]){
    for (int i=0;i<4;i++) array[i] = array[i] + 1;
}
