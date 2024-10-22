/**
 * \file spirals.c
 * \brief Hlavní soubor, který obsahuje funkci main.
 *
 * Úlohou tohoto programu je generování spirály dle zadaných parametrů.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stddef.h>
#include <assert.h>

#include "matrix/matrix.h"

/** Definice Ludolfova čísla. */
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

/** Úhel, o který se spirál otočí v každé iteraci. */
#define THETA M_PI/32

/** Počet bodů spirály. */
#define SPIRAL_POINT_COUNT 10000

/**
 * Jak moc se bude zmenšovat vzdálenost mezi počátkem soustavy souřadné a jednotlivými body spirály.
 * $CONVERGENCE_RATE \\in (0, 1)$
 */
#define CONVERGENCE_RATE 0.999

/** 
 * Současně není určeno k hrabání! 
 * Pokud chcete tvořit spirály i ve třech dimenzích, tak je třeba upravit funkci generující rotační matici, 
 * tj. funkce matrix *create_rotation_matrix(size_t dimension, double angle).
 */
#define SPIRAL_DIMENSION 2

/** Soubor, do kterého bude vypsána výsledná matice. */
#define OUTPUT_FILE "spiral.m"

/**
 * \brief print_spiral Funkce provede zápis jednotlivých bodů spirály do výstupního matlabovského skriptu, aby
 *                     bylo možné je vizualizovat. Pozor! Funkce očekává, že všechny vstupní body mají stejnou
 *                     dimenzi.
 * \param points Pole bodů spirály.
 * \param count Počet bodů spirály.
 */
void print_spiral(struct matrix **points, const size_t count) {
    FILE *out_file;
    size_t dim, i;

    if (!points || count == 0) {
        return;
    }

    out_file = fopen(OUTPUT_FILE, "w");
    if (!out_file) {
        printf("Soubor %s nejde vytvorit.", OUTPUT_FILE);
        return;
    }

    fprintf(out_file, "plot(");

    for (dim = 0; dim < (*points)->rows; ++dim) {
        if (dim > 0) {
            fprintf(out_file, ", ");
        }

        fprintf(out_file, "[");

        for (i = 0; i < count; ++i) {
            if (i > 0)
                fprintf(out_file, ", ");

            fprintf(out_file, "%f", matrix_get(points[i], dim, 0));
        }

        fprintf(out_file, "]");
    }

    fprintf(out_file, ");");

    /* Na co jsme zapomněli? */
}

/**
 * \brief create_rotation_matrix Funkce inicializuje rotační matici v zadané dimenzi problému.
 * \param dimension Dimenze prostoru v níž je rotace definována.
 * \param angle O jaký úhel se má objekt rotovat kolem počátku prostoru.
 * \return 1, pokud se inicializace podaří, jinak 0.
 */
int init_rotation_matrix(struct matrix *rotation_matrix, const size_t dimension, const double angle) {
    /* Váš domácí úkol. */
    assert(dimension == 2);

    if (!rotation_matrix || dimension == 0) {
        return 0;
    }

    if (!matrix_init(rotation_matrix, dimension, dimension, 0)) {
        return 0;
    }

    matrix_set(rotation_matrix, 0, 0, cos(angle));
    matrix_set(rotation_matrix, 0, 1, -sin(angle));
    matrix_set(rotation_matrix, 1, 0, sin(angle));
    matrix_set(rotation_matrix, 1, 1, cos(angle));

    return 1;
}

/**
 * \brief main Hlavní přístupový bod aplikace. Provádí výpočet bodů spirály, které následně
 *             pomocí funkce print_spiral zapíše do matlabovského skriptu.
 * \return EXIT_SUCCESS při úspěchu, jinak EXIT_FAILURE.
 */
int main() {
    struct matrix rotation_matrix, **spiral_points;
    size_t i;

    /* Vytvoření rotační matice. */
    if (!init_rotation_matrix(&rotation_matrix, SPIRAL_DIMENSION, THETA)) {
        return EXIT_FAILURE;
    }

    /* stejné jako `spiral_points = malloc(SPIRAL_POINT_COUNT * sizeof(struct matrix *));` */
    spiral_points = malloc(SPIRAL_POINT_COUNT * sizeof(*spiral_points));
    if (!spiral_points) {
        matrix_deinit(&rotation_matrix);
        return EXIT_FAILURE;
    }

    /* Vytváření bodů matice. Vidíte v následujícím kódu nějakou chybu? */
    spiral_points[0] = matrix_allocate(SPIRAL_DIMENSION, 1, 5000.0); /* Počátek spirály. */
    for (i = 1; i < SPIRAL_POINT_COUNT; ++i) {
        /* Rotace předchozího bodu spirály. */
        spiral_points[i] = matrix_mul(&rotation_matrix, spiral_points[i - 1]);

        /* Zmenšení poloměru (jinak bych bod rotoval po kružnici). */
        matrix_scale(spiral_points[i], CONVERGENCE_RATE);
    }

    print_spiral(spiral_points, SPIRAL_POINT_COUNT);

    /* Na co jsme zapomněli? */

    return EXIT_SUCCESS;
}
