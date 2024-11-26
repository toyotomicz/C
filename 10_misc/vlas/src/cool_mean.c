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
#include <time.h>


static void _array_rand_init(size_t rows, size_t cols, int array[rows][cols]) {
    srand(time(NULL));

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            array[i][j] = rand();
        }
    }
}

static int _array_arith_mean(size_t rows, size_t cols, int array[rows][cols]) {
    int acc = 0;

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            acc += array[i][j];
        }
    }

    return acc / (rows * cols);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        puts("usage: <rows> <cols>");
        return EXIT_FAILURE;
    }

    size_t rows = abs(atoi(argv[1]));  // Lépe použít třeba funkci `strtoul`.
    size_t cols = abs(atoi(argv[2]));

    int (*array)[rows][cols] = malloc(sizeof(*array));  // Tohle je cool a hlavně důvod existence VLAs!

    // Cool je, že `sizeof` nám vrací skutečnou velikost pole!
    printf("Velikost pole, na které ukazuje `array`: %zu\n", sizeof(*array));

    _array_rand_init(rows, cols, *array);
    printf("%d\n", _array_arith_mean(rows, cols, *array));

    return EXIT_SUCCESS;
}
