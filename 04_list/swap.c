#include <stdio.h>
#include <stdlib.h>

void swap(int *p_a, int *p_b){
    int temp;

    temp = *p_a;
    *p_a = *p_b;
    *p_b = temp;
}

int main(){
    int cislo1, cislo2;

    cislo1 = 5;
    cislo2 = 10;

    printf("%d, %d ", cislo1, cislo2);

    swap(&cislo1, &cislo2);

    printf("%d, %d ", cislo1, cislo2);

    return EXIT_SUCCESS;
}