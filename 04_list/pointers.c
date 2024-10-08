#include <stdio.h>
#include <stdlib.h>

int main(){
    int cislo;
    int *pointer_na_cislo;
    int **pointer_na_pointer_na_cislo;

    cislo = 5;
    pointer_na_cislo = &cislo;
    pointer_na_pointer_na_cislo = &pointer_na_cislo;

    printf("%d, %d, %d", cislo, *pointer_na_cislo, **pointer_na_pointer_na_cislo);

    return EXIT_SUCCESS;
}