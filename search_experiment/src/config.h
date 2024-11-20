/**
 * \file config.h
 * \author František Pártl (fpartl@ntis.zcu.cz)
 * \brief Soubor obsahuje konfigurační konstanty nutné při experimentu se způsoby vyhledávání.
 * \version 1.0
 * \date 2020-09-29
 */

#ifndef CONFIG
#define CONFIG

/**
 * \brief Soubor s nabídkou trhu.
 */
#define INPUT_FILE "./data/offer.csv"

/**
 * \brief Abychom se v tomto cvičení vyhnuli dynamické alokaci, tak jsme se podívali do souboru INPUT_FILE a zjistili
 *        kolik vlastně obsahuje řádek. Ve funkci load_commodities je ochrana, která zabraňuje čtení dalších řádků.
 */
#define COMMODITIES_COUNT 23219

/**
 * \brief Maximální počet znaků na jedné řádce souboru INPUT_FILE.
 */
#define MAX_LINE_LENGTH 512

/**
 * \brief Používaný znak k oddělování hodnotu v CSV souboru.
 */
#define CSV_DELIMETER ";"

/**
 * \brief Umístění souboru s vyhledávanými položkami.
 */
#define IDS_FILE "./data/find.txt"

/**
 * \brief Počet položek, které budou vyhledávány v datech. Opět jsme si pomohli a podívali jsme do souboru FIND_FILE.
 */
#define FIND_COUNT 46438

/**
 * \brief Konstanta říká, kolikrát bude vyhledávání spuštěno. Výsledek bude pak průměrem těchto měření doplněný o střední
 *        hodnotu.
 */
#define TEST_RUNS 1

#endif
