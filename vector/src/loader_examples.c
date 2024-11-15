#include "loader_examples.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "person.h"
#include "vector.h"
#include "config.h"

/* Jen pro ukázku. */
#define TODO(message) \
    printf(__FILE__":%d This has not been implemented yet: "#message"!\n", __LINE__);


void int_loader_example() {
    char line[CHAR_BUFFER_SIZE] = { 0 };
    struct vector numbers;  /* Instance `struct vector` je definována na zásobníku! */
    FILE *file;
    int temp;

    file = fopen(INTEGER_FILE, "r");
    if (!file) {
        return;
    }

    if (!vector_init(&numbers, sizeof(int), NULL)) {
        goto exit_close_file;
    }

    while (fgets(line, CHAR_BUFFER_SIZE, file)) {
        temp = atoi(line);

        if (!vector_push_back(&numbers, &temp)) {
            goto exit_clean_all;
        }
    }

  #ifdef PRINT_VECTORS
    for (size_t i = 0; i < vector_count(&numbers); ++i) {
        printf("%d\n", *(int *)vector_at(&numbers, i));
    }
  #endif

exit_clean_all:
    vector_deinit(&numbers);

exit_close_file:
    fclose(file);
}

void dynamic_person_loader_example() {
    char line[CHAR_BUFFER_SIZE] = { 0 }, *name;
    int age, shoe_size;
    FILE *file;
    struct vector *persons;
    struct person *temp_person;

    #define EXIT_IF_NOT(expression)         \
        if (!(expression)) {                \
            vector_deallocate(&persons);    \
            fclose(file);                   \
            return;                         \
        }

    file = fopen(PERSON_FILE, "r");
    if (!file) {
        return;
    }

    /**
     * Na následujícím řádku vidíte explicitní konverzi ukazatele na funkci. Konkrétně zde převádíme
     *
     *     `void(*)(person **)` na `void(*)(void *)`.
     *
     * Teoreticky by v tom neměl být problém -- stejný návratový typ a stejný počet stejně velkých parametrů. Standard
     * jazyka tuto konverzi ovšem NIJAK NEDEFINUJE, takže je možné, že na nějaké exotické platformě tohle nebude
     * fungovat. Díky explicitní konverzi jsme kompilátoru "zavřeli ústa", takže hledání chyby na oné exotické platformě
     * by bylo přinejlepším "sebevraždyhodné". Zkuste si explicitní konverzi (`(vec_it_dealloc_type)`) odstranit,
     * kompilátor začne okamžitě chrlit chybová hlášení. Když pomineme exotické platformy a zamyslíme se nad mnohem
     * častějším jevem, vlastní blbostí, zjistíme, že explicitní konverzí můžeme "převést" i nesouvisející funkce, třeba
     *
     *     `int(*)()` na `void(*)(void *)` (použijte například funkci `int_loader_example` -> zkuste si to!)
     *
     * De facto jsme kompilátoru řekli "buď v klidu, vím co dělám", a to samozřejmě obecně není dobrý nápad.
     *
     * ! Mnohem lepším nápadem je napsat obalovací funkci (třeba `_person_deinit_wrapper` definovanou níže), která již
     * ! bude mít správný prototyp. V jejím těle pak provedeme implicitní konverzi samotného ukazatele, což je operace
     * ! definovaná standardem, tzn. jsme v pohodě a můžeme klidně spát (to děláme při každém volání funkce `malloc`).
     *
     * Zajímavostí ovšem je, že na této explicitní konverzi ukazatelů na funkce (tedy standardem zcela nedefinované
     * operaci) závisí kromě našeho skromného prográmku i nemalé množství v produkci používaných knihoven. Dokonce i
     * samotná glibc! S tou nedefinovaností to tedy nebude tak horké... ale je dobré vědět, že dle standardu je tato
     * konverze nedefinovaná. 
     *
     * ! Jediné co standard říká je, že pokud ten pointer vezmu, převedu a pak převedu zpátky na původní typ, tak se
     * ! hodnota ukazatele nezmění. Dále pozor: toto řešení bude fungovat jenom s kompilátorem jazyka C! Ukazatel na
     * ! funkci v očích C++ je úplně jiná liga.
    */
    persons = vector_allocate(sizeof(struct person *), (vec_it_dealloc_type)person_deallocate);
    EXIT_IF_NOT(persons);

    while (fgets(line, CHAR_BUFFER_SIZE, file)) {
        if (strlen(line) == 0) {
            continue;
        }

        age = atoi(strtok(line, CSV_DELIMETER));
        shoe_size = atoi(strtok(NULL, CSV_DELIMETER));
        name = strtok(NULL, CSV_DELIMETER);
        name[strcspn(name, "\r\n")] = 0;

        temp_person = person_allocate(name, age, shoe_size);
        EXIT_IF_NOT(temp_person);
        EXIT_IF_NOT(vector_push_back(persons, &temp_person));
    }

    /**
     * Uvědomte si, co přesně je uloženo ve vektoru persons (přesný datový typ).
     * Prvek z vektoru vyberte pomocí funkce vector_at a opět se důkladně zamyslete nad tím, co tato funkce vrací.
     * Instanci struktury struct person je možné vypsat pomocí funkce person_print, která je deklarována v person.h.
     */
  #ifdef PRINT_VECTORS
    TODO("Vypis vektoru persons");
  #endif

    EXIT_IF_NOT(0);
    #undef EXIT_IF_NOT
}

/**
 * \brief Účel této obalové funkce popisuje "slohová práce" výše. Ve zkratce se tímto způsobem zbavujeme standardem
 *        nedefinované konverze ukazatelů funkce a místo toho implicitně převádíme samotné ukazatele, v tomto případě
 *  
 *            `void *` na `struct person *`.
 * 
 *        Využíváním implicitních konverzí dáváme kompilátoru prostor pro kontrolu.
 * 
 * \param poor Ukazatel na deinicilizovanou instanci struktury `person`.
 */
static void _person_deinit_wrapper(void *poor) {
    person_deinit(poor);
}

void static_person_loader_example() {
    char line[CHAR_BUFFER_SIZE] = { 0 }, *name;
    int age, shoe_size;
    FILE *file;
    struct vector *persons;
    struct person temp_person;

    #define EXIT_IF_NOT(expression)         \
        if (!(expression)) {                \
            vector_deallocate(&persons);    \
            fclose(file);                   \
            return;                         \
        }

    file = fopen(PERSON_FILE, "r");
    if (!file) {
        return;
    }

    /* ! Zde není potřeba ošklivé explicitní konverze! Woo hoo! */
    persons = vector_allocate(sizeof(temp_person), _person_deinit_wrapper);
    EXIT_IF_NOT(persons)

    while (fgets(line, CHAR_BUFFER_SIZE, file)) {
        if (strlen(line) == 0) {
            continue;
        }

        age = atoi(strtok(line, CSV_DELIMETER));
        shoe_size = atoi(strtok(NULL, CSV_DELIMETER));
        name = strtok(NULL, CSV_DELIMETER);
        name[strcspn(name, "\r\n")] = 0;

        /* Inicializuji automaticky alokovanou (na zásobníku této funkce) instanci struktury struct person. */
        EXIT_IF_NOT(person_init(&temp_person, name, age, shoe_size));

        /* A zkopírujeme ji do vektoru persons. */
        EXIT_IF_NOT(vector_push_back(persons, &temp_person));
    }

    /**
     * Uvědomte si, co přesně je uloženo ve vektoru persons (přesný datový typ).
     * Prvek z vektoru vyberte pomocí funkce vector_at a opět se důkladně zamyslete nad tím, co tato funkce vrací.
     * Instanci struktury struct person je možné vypsat pomocí funkce person_print, která je deklarována v person.h.
     */
  #ifdef PRINT_VECTORS
    TODO("Vypis vektoru `persons`")
  #endif

    EXIT_IF_NOT(0);
    #undef EXIT_IF_NOT
}
