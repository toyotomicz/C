/**
 * \file qsortdemo.c
 * \author František Pártl (fpartl@ntis.zcu.cz)
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stddef.h>
#include <stdint.h>

/**
    Takhle to vychází mně:
        a) Implementace funkce swap v tomto souboru                               ? s
        b) Implementace funkce swap v assembleru                                  ? s
        c) Implementace funkce swap v tomto souboru s -O3                         ? s

    Měření byla prováděna bez výpisu a s NUMBER_COUNT 200000000.
*/

/** \brief Počet generovaných čísel, která budou následně řazena algoritmem Quicksort. */
// #define NUMBER_COUNT 10
#define NUMBER_COUNT 200000000

/** \brief Volíme zde konstantní seed generátoru náhodných čísel. Co to znamená a proč to děláme?  */
#define SRAND_SEED 42

/**
 * \brief Při deklaraci tohoto symbolu budou přeloženy a spuštěny testy správnosti řešení
 *        problému. Tohle je samozřejmě "prasárna". Testy by měly být umístěny v odděleném projektu
 *        v adresáři `tests`! Ale jenom pro ukázku... více na předmětu KIV/OKS.
 * 
 *        Sám CMake podporuje testování. Mrkněte do dokumentace. Testy jsou obyčejné prográmky, které
 *        vracejí 0 nebo 1. Můžete tak napsat testy ověřující chování programu například v Pythonu a unit 
 *        testy v C/C++ například s knihovnou GoogleTest.
 */
#define BUILD_TESTS

/**
 * \brief Naplní zadané pole náhodnými čísly.
 * \param array Plněné pole.
 * \param array_size Počet prvků generovaného pole.
 */
void fill_array(int array[], const size_t array_size) {
    size_t i;

    if (!array || array_size == 0) {
        return;
    }

    /* Inicializace generátoru náhodných čísel. */
  #ifdef SRAND_SEED
    srand(SRAND_SEED);
  #endif

    for (i = 0; i < array_size; ++i) {
        array[i] = rand();
    }
}

/**
 * \brief Vypíše array_size prvků pole array na standardní výstup programu. Pokud ukazatel
 *        na pole neplatný nebo má pole nulovou délku, funkce neprovede žádnou akci.
 * \param array Pole celých čísel, které bude tištěno.
 * \param array_size Počet tištěných prvků.
 */
void print_array(const int *array, const size_t array_size) {
    size_t i;

    if (!array || array_size == 0) {
        return;
    }

    for (i = 0; i < array_size; ++i) {
        printf("%3lu. %15d\n", i + 1, array[i]);
    }
}

/**
 * \brief Prohodí hodnoty dvou proměnných, které jsou dány zadanými ukazateli.
 * \param a Ukazatel na první proměnnou.
 * \param b Ukazatel na druhou proměnnou.
 */
// void swap(int *a, int *b) {
//     int temp;

//     temp = *a;
//     *a = *b;
//     *b = temp;
// }

/**
 * \brief Funkci můžu samozřejmě pouze deklarovat a její tělo definovat v jiném souboru.
 *        Volací konvence __fastcall znamená, že parametry nebudou předávány prostřednictvím
 *        zásobníku, ale pomocí registrů ECX/RCX/RDI a EDX/RDX/RSI.
 */
void swap(int *, int *);
/* void __fastcall swap(int *, int *); */
/* void swap(int *, int *) __attribute__((fastcall)); */

/**
 * \brief Rekurzivní implementace řadícího algoritmu Quicksort.
 * \param array Pole řazených prvků. 
 * \param first Index první prvku 
 * \param last Index posledního prvku.
 */
void quicksort(int *array, const size_t first, const size_t last){
    size_t fi = first, li = last;
    const size_t pivot = first;

    if (!array || first >= last) {
        return;
    }

    while (fi < li) {
        while (array[fi] <= array[pivot] && fi < last) {
            fi++;
        }
        while (array[li] > array[pivot] && li > 0) {
            li--;
        }

        if (fi < li) {
            swap(&array[fi], &array[li]);
        }
    }

    swap(&array[pivot], &array[li]);
    if (li > 0) {
        quicksort(array, first, li - 1);
    }
    if (li < SIZE_MAX) {
        quicksort(array, li + 1, last);
    }
}

/**
 * \brief Funkce pro zkontrolování správnosti výsledku řazení. Taková funkce by neměla být zde, ale byla by umístěna
 *        v samostatném spustitelném souboru v adresáři test. Vzpomínáte na standardní adresářovou strukturu v C/C++
 *        projektech? V adresáři `tests` by pak byl soubor `CMakeLists.txt` s definicí připravených testů. Testem může
 *        být jakýkoliv příkaz, který vrací 0 nebo !0. Tzn. že testy ověřující chování Vaší aplikace můžete mít napsané
 *        třeba v Pythonu.
 * \param array Pole, u kterého bude ověřena vzestupnost jeho prvků.
 * \param array_size Počet prvků pole.
 * \return int 1, pokud test dopadl dobře, tj. prvky pole jsou vzestupně seřazeny, jinak 0.
 */
int test_ascendence(const int *array, const size_t array_size) {
    size_t i;

    if (!array) {
      return 0;
    }

    /* "odrolujeme" proměnnou `i` na konec pole... */
    for (i = 1; i < array_size && array[i] >= array[i - 1]; ++i);
    return i == array_size;
}

/**
 * \brief Hlavní přístupový bod aplikace, která vygeneruje `NUMBER_COUNT` čísel, které 
 *        seřadí algoritmem QuickSort.
 * \return int Vrací `EXIT_SUCCESS` při úspěchu, jinak `EXIT_FAILURE`.
 */
int main() {
    int *numbers;
    clock_t tic, toc;

    numbers = malloc(NUMBER_COUNT * sizeof(int));
    if (!numbers) {
        printf("Tolik mista nam operacni system nechce dat...\n");
        return EXIT_FAILURE;
    }

    fill_array(numbers, NUMBER_COUNT);

    /* Spustím algoritmus Quicksort a měřím rozdíl v počtu tiků procesorových hodin. */
    tic = clock();
    quicksort(numbers, 0, NUMBER_COUNT - 1);
    toc = clock();
    printf("Quicksort razeni trvalo: %0.3f sekund.\n", (double)(toc - tic) / CLOCKS_PER_SEC);

  /**
      Tohle je prasárna! Viz definice makra `BUILD_TESTS`.
      Ale buďme realisté... tenhle náš malý projekt je sám o sobě takový test.
  */
  #ifdef BUILD_TESTS 
    printf("\nVysledek testu: %s!\n", test_ascendence(numbers, NUMBER_COUNT) ? "OK" : "CHYBA");
  #endif

    return EXIT_SUCCESS;
}
