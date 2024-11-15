/**
 * \file person.h
 * \author František Pártl (fpartl@ntis.zcu.cz)
 * \brief Hlavičkový soubor "přepravky" pro osobu.
 * \date 2021-11-25
 */

#ifndef PERSON_H_
#define PERSON_H_

/** \brief Struktura popisující osobu, její věk a velikost bot. */
struct person {
    char *name;     /**< Jméno osoby (dynamicky alokováno). */
    int age;        /**< Věk osoby. */
    int shoe_size;  /**< Velikost bot osoby. */
};

/**
 * \brief Funkce vytvoří novou instanci struktury `person`. Nová instance bude dynamicky alokována.
 * \param name Jméno osoby.
 * \param age Věk osoby.
 * \param shoe_size Velikost bot osoby.
 * \return person* Ukazatel na nově dynamicky alokovanou instanci struktury `person`.
 */
struct person *person_allocate(const char *name, const int age, const int shoe_size);

/**
 * \brief Funkce provede inicializaci instance struktury `person`, která je dána ukazatelem `p`. Velikost
 *        dynamicky alokovaného segmentu určenému pro jméno osoby bude stanovena na základě délky
 *        zadaného jména (ukazatel `name`).
 * 
 * \param p Ukazatel na inicializovanou instanci struktury `person`.
 * \param name Ukazatel na řetězec znaků obsahující jméno osoby.
 * \param age Věk osoby.
 * \param shoe_size Velikost bot osoby.
 * \return int 1, pokud inicializace proběhla v pořádku, jinak 0.
 */
int person_init(struct person *p, const char *name, const int age, const int shoe_size);

/**
 * \brief Funkce pro deinicializaci (uvolnění vnitřních členů) instance struktury `person`.
 * \param poor Uvolňována osoba.
 */
void person_deinit(struct person *poor);

/**
 * \brief Funkce pro uvolnění osoby a zneplatnění ukazatele na ní.
 * \param poor Uvolňována osoba.
 */
void person_deallocate(struct person **poor);

/**
 * \brief Funkce vytiskne osobu do konzole.
 * \param p Tištěná osoba.
 */
void person_print(const struct person *p);

#endif
