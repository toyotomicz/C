/**
 * @file stack_size.c
 * @author František Pártl (fpartl@kiv.zcu.cz)
 * @brief Experimentální prográmek pro zjištění velikosti dostupného zásobníku.
 * @version 1-0
 * @date 2020-11-12
 */

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

/* Odkaz na web Wolfram Alpha: https://www.wolframalpha.com */

/**
 * @brief Funkce na zásobníku alokuje 1 B paměti a vypíše jeho adresu. Následuje rekurzivní volání,
 *        které musí nutně skončit přetečením zásobníku. Pokud pak odečteme první a poslední vypsanou
 *        adresu, získáme velikost dostupného zásobníku.
 * @return int Funkce vrací vždy `EXIT_SUCCESS`.
 */
int main() {
    char var = 'c';

    printf("Adresa znaku `var` na zasobniku funkce `main` je %p\n", &var);
    main();

    return EXIT_SUCCESS;
}
