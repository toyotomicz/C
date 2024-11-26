/**
 * @file string_comparison.c
 * @author František Pártl (fpartl@ntis.zcu.cz)
 * @brief Demonstrace porovnávání řetězcových konstant.
 * @date 2020-11-12
 */

#include <stdlib.h>
#include <stdio.h>


/**
 * \brief Demonstruje chování použití porovnávacího operátoru s řetězcovými konstantami.
 * \return int Vždy vrací `EXIT_SUCCESS`.
 */
int main() {
    const char *str_1 = "Hello world!\n";
    const char *str_2 = "Hello world!\n";

    if (str_1 == str_2) {
        printf("Strings are the same!\n");
    }

    return EXIT_SUCCESS;
}
