/**
 * \file config.h
 * \author František Pártl (fpartl@ntis.zcu.cz)
 * \brief Hlavičkový soubor obsahující konfigurace naší aplikace. Jaké má toto řešení nevýhody?
 * \date 2022-11-10
 */

#ifndef CONFIG_H_
#define CONFIG_H_

/** \brief Používaný znak k oddělování hodnotu v CSV souboru. */
#define CSV_DELIMETER ";"

/** \brief Souboru s načítanými osobami. */
#define PERSON_FILE "../data/persons.csv"

/** \brief Soubor s načítanými celočíselnými hodnotami. */
#define INTEGER_FILE "../data/integers.txt"

/** \brief Velikost bufferu pro načítání jednotlivých řádek. */
#define CHAR_BUFFER_SIZE 256

/** \brief Pokud je makro definováno, vektory budou po naplnění vypsány. */
#define PRINT_VECTORS

#endif
