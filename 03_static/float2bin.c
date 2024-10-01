#include <stdio.h>
#include <stdlib.h>

union float int {
    float f;
    unsigned int i;
}

int main(int argc, char **argv[]){
    union float int x;
    if (argc < 2){
        puts("zadejte float > ");
        return EXIT_FAILURE;
    }
    x.f = atof(argv[1]);

    (x.i << 31) & 1;

    return EXIT_SUCCESS;
}