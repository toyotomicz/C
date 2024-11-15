/**
 * \file loader_examples.h
 * @author František Pártl (fpartl@ntis.zcu.cz)
 * \brief Hlavičkový soubor knihovny pro testování kontejneru `struct vector`.
 * @version 1.0
 * \date 2022-11-10
 */

#ifndef LOADER_EXAMPLES_H_
#define LOADER_EXAMPLES_H_

/** \brief Ukázka načítání instancí typu `int`, tj. celých čísel. */
void int_loader_example();

/**
 * \brief Načítání osob ze souboru `PERSON_FILE`. Každá osoba je uložena do dynamicky alokované instance struktury
 *        `person`. Do vektoru `persons` jsou pak ukládány ukazatele na ně. Je zde tedy provedeno velké množství drahé
 *        dynamické alokace, která v případě malých struktur, jako je třeba `person`, není vůbec nutná. Porovnejte tuto
 *        funkci s funkcí `static_person_loader_example`.
 */
void dynamic_person_loader_example();

/**
 * \brief Načítání osob ze souboru `PERSON_FILE`. Oproti funkci `dynamic_person_loader_example` tato funkce nevytváří
 *        nové a nové dynamicky alokované instance struktury `person`, ale místo toho cyklicky přepisuje jednu automaticky
 *        alokovanou na zásobníku funkce (proměnná `temp_person`). Tu pak kopíruje přímo do vektoru `persons`. Buffer vektoru je
 *        tak větší, ale zbavili jsme se tak drahé alokace na haldě nemalého množství dereferencí ukazatelů. Zkuste zvětšovat
 *        soubor `PERSON_FILE`. Měli byste sledovat nezanedbatelný rozdíl v časech.
 */
void static_person_loader_example();

#endif
