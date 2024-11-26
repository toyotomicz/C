/**
 * @file const_literals.c
 * @author František Pártl (fpartl@ntis.zcu.cz)
 * @brief Demonstrace ukládání řetězcových konstant.
 * @date 2020-11-12
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Oddělovací znak použitý při tokenizaci vstupů. */
#define STR_DELIMETER " "

/**
 * \brief Funkce provede tokenizaci vstupního řetězce pomocí znaku `STR_DELIMETER` a jednotlivé tokeny vypíše do
 * standardního výstupu.
 * TODO: nahradit simulaci vstupu skutečným argumentem
 * \return int Vrací EXIT_SUCCESS.
 */
int main() {
    const char *str = "... ---";

    char *tok = strtok(str, STR_DELIMETER);
    while (tok) {
        puts(tok);
        tok = strtok(NULL, STR_DELIMETER);
    }

    return EXIT_SUCCESS;
}
