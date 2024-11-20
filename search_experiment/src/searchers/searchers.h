/**
 * \file searchers.h
 * \author František Pártl (fpartl@ntis.zcu.cz)
 * \brief V tomto souboru jsou deklarovány vaše vyhledávací funkce.
 * \version 1.0
 * \date 2020-09-29
 */

#ifndef SEARCHERS
#define SEARCHERS

#include <stdlib.h>
#include <string.h>

#include "../commodity.h"

/**
 * \brief Definice typu ukazatele na vyhledávací funkci, která přijímá pole dat, vyhledávaných identifikátorů a výsledků.
 *        Všechny další funkce deklarované v tomto hlavičkovém souboru odpovídají tomuto prototypu.
 */
typedef int (*searcher_function)(const struct commodity comms[], const size_t comms_count,
                                 const comm_id_array_type ids[], const size_t ids_count, int quantities[]);

/**
 * \brief Funkce, která provádí lineární vyhledávání klíčů ids v poli comms. Výsledky vyhledávání jsou zapisovány na odpovídající
 *        pozice do pole quantities.
 * \param comms Datové pole s komoditami, které bude prohledáváno.
 * \param comms_count Počet prvků pole comms.
 * \param ids Pole identifikátorů, které budou postupně vyhledávány v datovém poli comms.
 * \param ids_count Počet prvků v poli ids.
 * \param quantities Pole výsledků hledání, tj. počet dostupných položek komodity (quantity.quantity).
 * \return int Funkce vrací EXIT_SUCCESS pokud byly všechny klíče nalezeny, jinak EXIT_FAILURE.
 */
int linear_search(const struct commodity comms[], const size_t comms_count,
                  const comm_id_array_type ids[], const size_t ids_count, int quantities[]);

/**
 * \brief Funkce, která provádí vyhledávání klíčů ids v poli comms pomocí algoritmu binárního vyhledávání. Výsledky vyhledávání
          jsou zapisovány na odpovídající pozice do pole quantities.
 * \param comms Datové pole s komoditami, které bude prohledáváno.
 * \param comms_count Počet prvků pole comms.
 * \param ids Pole identifikátorů, které budou postupně vyhledávány v datovém poli comms.
 * \param ids_count Počet prvků v poli ids.
 * \param quantities Pole výsledků hledání, tj. počet dostupných položek komodity (quantity.quantity).
 * \return int Funkce vrací EXIT_SUCCESS pokud byly všechny klíče nalezeny, jinak EXIT_FAILURE.
 */
int binary_search(const struct commodity comms[], const size_t comms_count,
                  const comm_id_array_type ids[], const size_t ids_count, int quantities[]);

#endif