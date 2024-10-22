/**
 * \file tree.c
 * \author František Pártl (fpartl\ntis.zcu.cz)
 * \brief Zdrojový soubor aplikace, která využívá námi vytvořenou implementaci binárního
 *        vyhledávacího stromu.
 * \version 1.0
 * \date 2020-09-07
 */

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

#include "bst.h"

/**
 * \brief Hlavní přístupový bod aplikace. 
 * \return int Funkce vždy vrací `EXIT_SUCCESS`.
 */
int main() {
    struct bst_node *tree = NULL;  /* Ukazatelem s hodnotou NULL označuji prázdný strom. */
    size_t i;

    /* Do stromu přidáme nějaké prvky. */
    /* Jak bude takový strom vypadat? Přihlédneme-li k naší implementaci a vkládaným prvkům?*/
    for (i = 0; i < 10; i++) {
        bst_add(&tree, i);
    }

    /* Výpis stromu si zkuste za domácí úkol. */
    /*
    bst_print(tree);
    printf("\n");
    */

    printf("Velikost stromu je %d uzlu.\n", bst_size(tree));
    printf("Vyvazenost stromu je %d.\n", bst_dsize(tree));

    bst_free(&tree);    /* Zkuste si program zkontrolovat s nástrojem Valgrind a smazat tento řádek. */
    return EXIT_SUCCESS;
}
