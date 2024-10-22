#include <stdio.h>
#include <stdlib.h>

int main(){
    int i = 5, j = 5;

    printf("i = %d, j = %d", i, j);

    // i = i++;
    // printf("i = i++; => i = %d, j = %d", i, j);

    // i = ++i;
    // printf("i = ++i; => i = %d, j = %d", i, j);

    // if (i++ == ++i){
    //     puts("what?");
    // }
    if (i++ && ++i){
        puts("what?");
    }
    

    return EXIT_SUCCESS;
}