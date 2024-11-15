/**
 * \file loader.c
 * \author František Pártl (fpartl@ntis.zcu.cz)
 * \brief Příklad, na kterém si ukážeme načítání dat ze souborů do dynamicky realokovaného pole.
 * \version 2.0
 * \date 2022-11-10
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "config.h"
#include "person.h"
#include "vector.h"
#include "loader_examples.h"

/**
 * \brief Funkce pro měření doby vykonávání jiné funkce, zadané ukazatelem `fnc`. Čas je měřen v milisekundách.
 * \param fnc_label Slovní popisek měřené funkce.
 * \param fnc Ukazatel na funkci, u které chceme měřit dobu běhu.
 */
void measure_fnc_time(const char *fnc_label, void(*fnc)()) {
    clock_t start, stop;
    double time;

    if (!fnc_label || !fnc) {
        return;
    }

    /**
     * Tento způsob měření výkonu naší implementace je velice nepřesný, protože funkce `clock` vrací počet
     * tiků procesoru. Hodiny ovšem tikají, i když náš proces neběží -- například, když čeká na zdroje.
     */
    start = clock();
    fnc();
    stop = clock();

    time = ((double)(stop - start)) / (CLOCKS_PER_SEC / 1000);
    printf("%s: %f ms\n", fnc_label, time);
}

/**
 * \brief Hlavní přístupový bod aplikace.
 * \return int Vždy vrací hodnotu `EXIT_SUCCESS`.
 */
int main() {
    measure_fnc_time("void int_loader_example()", int_loader_example);
    measure_fnc_time("void dynamic_person_loader_example()", dynamic_person_loader_example);
    measure_fnc_time("void static_person_loader_example()", static_person_loader_example);

    return EXIT_SUCCESS;
}
