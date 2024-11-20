/**
 * \file commodity.h
 * \author František Pártl (fpartl@ntis.zcu.cz)
 * \brief Soubor obsahující definici datového typu commodity.
 * \version 1.0
 * \date 2020-09-29
 */

#ifndef COMMODITY
#define COMMODITY

/**
 * \brief Maximální délka identifikátoru včetně ukončovací nuly.
 */
#define MAX_ID_LENGTH 16

/**
 * \brief Datový typ identifikátoru komodity. Pokud je Vám zde něco divného, tak je to jedině v pořádku.
 *        O objasnění tohoto zvěrstva požádejte cvičícího.
 */
typedef char comm_id_array_type[MAX_ID_LENGTH];

/**
 * \brief Maximální délka jména komodity včetně ukončovací nuly. 
 */
#define MAX_NAME_LENGTH 64

/**
 * \brief Datový typ názvu komodity. Pokud je Vám zde něco divného, tak je to jedině v pořádku.
 *        O objasnění tohoto zvěrstva požádejte cvičícího.
 */
typedef char comm_name_array_type[MAX_NAME_LENGTH];

/**
 * \brief Definice datového typu commodity, který obsahuje informace o jedné komoditě vedené na
 *        fiktivní burze. Všechny nabízené položky mají jedinečný identifikátor a jméno.
 *        U každého je dále vedena informace o dostupném množství a ceně jednotlivých položek.
 *        Posledním údajem je identifikátor obchodníka, u kterého je zboží dostupné.
 */
struct commodity {
    comm_id_array_type id;      /*!< Identifikátor zboží. */
    comm_name_array_type name;  /*!< Název zboží. */
    int quantity;               /*!< Počet dostupných položek. */
    int price;                  /*!< Cena za jednu položku. */
    int merchant;               /*!< Identifikátor obchodníka, u kterého je zboží k dispozici. */
};

#endif
