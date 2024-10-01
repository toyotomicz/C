#include <stdio.h>
#include <stdlib.h>

int main(int argc,char *argv[]){
    unsigned int dec;
    int i;
    char inverted_bin[sizeof(dec) * 8 + 1];
    if(argc < 2){
        puts("Napis dec > ");
        return EXIT_FAILURE;
    }

    dec = abs(atoi(argv[1]));

    printf("%d:d =", dec);

    /* zbiram zbyky po deleni a ukladam do pole inverted_bin */
    for(i = 0 ; dec != 0 ; dec /= 2, ++i){
        inverted_bin[i] = dec % 2 + '0';
    }

    for(--i ; i >= 0 ; --i){
        printf("%c", inverted_bin[i]);
    }
    printf(":b");

    return EXIT_SUCCESS;
}