/**
 * @file const_literals.c
 * @author František Pártl (fpartl@ntis.zcu.cz)
 * @brief Ukázka nešťastného použití polí variabilní délky (C99 VLA).
 * @date 2020-11-12
 */

/**
 * Trocha internetu ke čtení:
 *
 * https://nullprogram.com/blog/2019/10/27/
 * https://stackoverflow.com/questions/12407754/what-technical-disadvantages-do-c99-style-vlas-have
 * https://stackoverflow.com/questions/22530363/whats-the-point-of-vla-anyway
 * https://softwareengineering.stackexchange.com/questions/190546/a-good-c-variable-length-array-example
 * https://stackoverflow.com/questions/70792452/why-is-this-vla-variable-length-array-definition-unreliable
 * https://pvs-studio.com/en/docs/warnings/v2598/
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h> /* Potřebujeme něco čím budemem inicializovat generátor pseudonáhodných čísel. */


static void _array_rand_init(int arr[], size_t arr_len) {
    srand(time(NULL));

    for (size_t i = 0; i < arr_len; ++i) {
        arr[i] = rand();
    }
}

static int _arith_mean(int arr[], size_t arr_len) {
    int acc = 0;

    for (size_t i = 0; i < arr_len; ++i) {
        acc += arr[i];
    }

    return acc / arr_len;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        puts("usage: <positive-number>");
        return EXIT_FAILURE;
    }

    size_t array_size = abs(atoi(argv[1]));

    int array[array_size];  // Fuj! Použití VLA s automatickou alokací...

    printf("Velikost pole `array`: %zu\n", sizeof(array));

    _array_rand_init(array, array_size);
    printf("%d\n", _arith_mean(array, array_size));

    return EXIT_SUCCESS;
}
