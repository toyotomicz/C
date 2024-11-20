/**
 * \file search_experiment.c
 * \author František Pártl (fpartl@ntis.zcu.cz)
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <time.h>
#include <math.h>

#include "config.h"
#include "commodity.h"
#include "searchers/searchers.h"

/**
 * \brief Funkce ze zadaného souboru načte maximálně max_n komodit do pole daného ukazatelem out. Kvůli možnosti
 *        přetečení zásobníku jsou záznamy načítány do dynamické paměti. Výsledek je tedy třeba uvolnit pomocí
 *        funkce free.
 * \param input_file Umístění souboru s načítanými daty.
 * \param out Ukazatel na pole, kam budou data načtena.
 * \param max_n Maximální počet načítaných prvků.
 * \return size_t Počet reálně načtených prvků, nebo 0 při chybě.
 */
size_t load_commodities(const char *input_file, struct commodity **out, const size_t max_n) {
    char *token, line[MAX_LINE_LENGTH] = { 0 };
    struct commodity *temp;
    FILE *file;
    size_t i;

    if (!input_file || !out || max_n == 0) {
        return 0;
    }

    file = fopen(input_file, "r");
    if (!file) {
        return 0;
    }

    temp = malloc(sizeof(struct commodity) * max_n);
    if (!temp) {
        fclose(file);
        return 0;
    }

    for (i = 0; i < max_n && fgets(line, MAX_LINE_LENGTH, file); ++i) {
        /* Pro jednoduchost zde nejsem provedeny žádné kontroly, které by zde samozřejmě být měly! */
        token = strtok(line, CSV_DELIMETER);
        strncpy(temp[i].id, token, MAX_ID_LENGTH);

        token = strtok(NULL, CSV_DELIMETER);
        strncpy(temp[i].name, token, MAX_NAME_LENGTH);

        token = strtok(NULL, CSV_DELIMETER);
        temp[i].quantity = atoi(token);

        token = strtok(NULL, CSV_DELIMETER);
        temp[i].price = atoi(token);

        token = strtok(NULL, CSV_DELIMETER);
        temp[i].merchant = atoi(token);
    }

    fclose(file);
    *out = temp;
    return i;
}

/**
 * \brief Funkce načítá hledané identifikátory ze souboru ids_file do pole out. Načteno bude maximálně max_n identifikátorů.
 * \param ids_file Soubor s identifikátory.
 * \param out Ukazatel na pole, kam budou identifikátory načteny.
 * \param max_n Maximální počet načtených identifikátorů.
 * \return size_t Reálný počet načtených identifikátorů.
 */
size_t load_identifiers(const char *ids_file, comm_id_array_type **out, const size_t max_n) {
    comm_id_array_type *temp;
    FILE *file;
    size_t i;

    if (!ids_file || !out || max_n == 0) {
        return 0;
    }

    file = fopen(ids_file, "r");
    if (!file) {
        return 0;
    }

    temp = malloc(sizeof(comm_id_array_type) * max_n);
    if (!temp) {
        fclose(file);
        return 0;
    }

    /* Opět pro jednoduchost bez kontrol. */
    for (i = 0; i < max_n && fgets(temp[i], sizeof(*temp), file); ++i) {
        temp[i][strcspn(temp[i], "\r\n")] = 0;
    }

    fclose(file);
    *out = temp;
    return i;
}

/**
 * \brief Funkce, která spustí vyhledávací algoritmus zadaný ukazatelem na funkci. Funkce bude spuštěna TEST_RUNSkrát.
 *        Bude počítán čas na procesoru při každém spuštění. Výsledkem měření je pak průměr těchto měření a rozptyl měření.
 *        Ostatní parametry funkce jsou předány vyhledávacímu algoritmu.
 * \param searcher Ukazatel na spuštěnou funkci (vyhledávací algoritmus).
 * \param searcher_label Popisek vyhledávacího algoritmu, který bude vypisován do konzolového výstupu aplikace.
 * \param comms Data, ve kterých bude prováděno vyhledávání.
 * \param comms_count Počet prvků pole comms.
 * \param ids Pole vyhledávaných identifikátorů.
 * \param ids_count Počet prvků pole ids.
 * \param out_quantities Pole, do kterého bude vyhledávací algoritmus ukládat nalezené hodnoty.
 */
void run_searcher(const searcher_function searcher, const char *searcher_label, const struct commodity comms[],
                    size_t comms_count, const comm_id_array_type ids[], const size_t ids_count, int out_quantities[]) {
    double times[TEST_RUNS], average = .0, variance = .0;
    clock_t tic, toc;
    size_t i;
    int ret_code;

    for (i = 0; i < TEST_RUNS; ++i) {
        memset(out_quantities, 0, sizeof(int) * ids_count);     /* Před každým spuštěním vynuluji pole výsledků. */

        tic = clock();
        ret_code = searcher(comms, comms_count, ids, ids_count, out_quantities);
        toc = clock();

        if (ret_code == EXIT_SUCCESS) {
            times[i] = (double)(toc - tic) / CLOCKS_PER_SEC;
            average += times[i];
        }
        else {
            printf("%s skoncilo chybou %d!", searcher_label, ret_code);
            return;
        }
    }
    average /= (double)TEST_RUNS;

    for (i = 0; i < TEST_RUNS; ++i) {
        variance += pow(times[i] - average, 2);
    }
    variance /= (double)TEST_RUNS;

    printf("%s trvalo %f sekund s rozptylem %f sekund.\n", searcher_label, average, variance);
}

/**
 * \brief Funkce porovnává dvě zadaná pole stejné dimenze.
 * \param lin_results Výsledky lineárního vyhledávání.
 * \param bin_results Výsledky vyhledávání pomocí BVS.
 * \param count Počet výsledků v polích lin_results a bin_results.
 * \return int 1, pokud pole obsahují stejné prvky, jinak 0.
 */
int validate_results(const int lin_results[], const int bin_results[], const size_t count) {
    size_t i;

    for (i = 0; i < count; ++i) {
        if (lin_results[i] != bin_results[i] || lin_results[i] == 0) {
            return 0;
        }
    }

    return 1;
}

/**
 * \brief Hlavní přístupový bod aplikace, která porovnává rychlost lineárního vyhledávání a vyhledávání pomocí BVS.
 * \return int Funkce při úspěchu vrací EXIT_SUCCESS, jinak EXIT_FAILURE. Chyby ve vyhledávacích algoritmech jsou ignorovány.
 */
int main() {
    /* Data načítám raději dynamicky, aby nedošlo k přetečení zásobníku. */
    comm_id_array_type *identifiers;
    struct commodity *commodities;
    size_t comm_count, ids_count;

    /* Výsledky alokuji staticky na zásobník (~370 kB už je to dost a bylo by bezpečnější sáhnout po dynamické alokaci). */
    int lin_results[FIND_COUNT], bin_results[FIND_COUNT];

    ids_count = load_identifiers(IDS_FILE, &identifiers, FIND_COUNT);
    if (ids_count == 0) {
        printf("Chyba pri nacitani vyhledavanych identifikatoru!\n");
        return EXIT_FAILURE;
    }

    comm_count = load_commodities(INPUT_FILE, &commodities, COMMODITIES_COUNT);
    if (comm_count == 0) {
        free(identifiers);
        printf("Chyba pri nacitani prohledavanych dat!\n");
        return EXIT_FAILURE;
    }

    /*
        Zde si muzete zkusit vypsat nactene komodity a identifikatory.

        printf("Nactene komodity:\n");
        for (i = 0; i < comm_count; ++i)
            printf("%s, %s, %d, %d, %d\n", commodities[i].id, commodities[i].name, commodities[i].quantity, commodities[i].price, commodities[i].merchant);

        printf("Nacteno %d komodit.\n", comm_count);

        printf("Hledane identifikatory:\n");
        for (i = 0; i < ids_count; ++i)
            printf("%s\n", identifiers[i]);

        printf("Nacteno %d identifikatoru.\n", comm_count);
    */

    printf("Nacteno %ld komodit.\nSpoustim vyhledavani %ld klicu...\n", comm_count, ids_count);

    run_searcher(linear_search, "Linearni vyhledavani", commodities, comm_count, identifiers, ids_count, lin_results);
    run_searcher(binary_search, "Binarni vyhledavani", commodities, comm_count, identifiers, ids_count, bin_results);

    printf("Vysledky vyhledavani jsou %s.\n", validate_results(lin_results, bin_results, FIND_COUNT)
                                                    ? "STEJNE! GRATULUJI!" 
                                                    : "RUZNE! NEKDE MATE CHYBU.");

    free(identifiers);
    free(commodities);
    return EXIT_SUCCESS;
}
