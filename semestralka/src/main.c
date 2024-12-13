#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "parser.h"
#include "postscript.h"

// Funkce pro odstranění nadbytečných mezer
void trim_spaces(char *dest, const char *src) {
    while (*src) {
        if (*src != ' ' && *src != '\t') {
            *dest++ = *src;
        }
        src++;
    }
    *dest = '\0';
}

// Funkce pro spojení všech argumentů od indexu 1
void combine_args(char *buffer, int argc, char *argv[]) {
    buffer[0] = '\0';
    for (int i = 1; i < argc - 1; i++) { // Kombinace do předposledního argumentu
        strcat(buffer, argv[i]);
        strcat(buffer, " ");
    }
    strcat(buffer, argv[argc - 1]); // Přidání posledního argumentu
}

int main(int argc, char *argv[]) {
    // Kontrola minimálního počtu parametrů
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <function> <output_file> [xmin:xmax:ymin:ymax]\n", argv[0]);
        fprintf(stderr, "Example: %s \"sin(x^2)\" output.ps\n", argv[0]);
        fprintf(stderr, "Example with limits: %s \"sin(x^2)\" output.ps -10:10:-1:1\n", argv[0]);
        return 1;
    }

    // Získání funkce ze vstupu
    char function_buffer[1024];
    combine_args(function_buffer, argc - 1, argv);
    char function_trimmed[1024];
    trim_spaces(function_trimmed, function_buffer);

    const char *function = function_trimmed;
    const char *output_file = argv[argc - 1];

    // Výchozí rozsahy
    double xmin = -10, xmax = 10, ymin = -10, ymax = 10;

    // Kontrola a parsování volitelného rozsahu
    if (argc >= 4 && sscanf(argv[argc - 1], "%lf:%lf:%lf:%lf", &xmin, &xmax, &ymin, &ymax) == 4) {
        argc--; // Poslední parametr byl rozsah
    }
    if (xmin >= xmax || ymin >= ymax) {
        fprintf(stderr, "Error: Invalid range format or logical range error. Use xmin:xmax:ymin:ymax and ensure xmin < xmax, ymin < ymax.\n");
        return 4;
    }

    // Validace funkce
    if (!validate_expression(function)) {
        fprintf(stderr, "Error: Invalid mathematical expression.\n");
        return 2;
    }

    // Kontrola, zda lze vytvořit výstupní soubor
    FILE *file = fopen(output_file, "w");
    if (!file) {
        fprintf(stderr, "Error: Cannot create or write to output file '%s'.\n", output_file);
        return 3;
    }
    fclose(file);

    // Generování bodů pro graf
    int num_points = 512;
    double *points = malloc(num_points * sizeof(double));
    if (!points) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        return 5;
    }

    double step = (xmax - xmin) / (num_points - 1);
    bool has_undefined_values = false;

    for (int i = 0; i < num_points; i++) {
        double x = xmin + i * step;
        EvaluationResult eval = evaluate_expression(function, x);
        if (eval.is_defined) {
            points[i] = eval.value;
        } else {
            has_undefined_values = true;
            points[i] = NAN;
        }
    }

    if (has_undefined_values) {
        fprintf(stderr, "Warning: The function contains undefined values in the given range.\n");
    }

    // Nastavení parametrů grafu
    GraphParams params = {
        .min_x = xmin,
        .max_x = xmax,
        .min_y = ymin,
        .max_y = ymax,
        .width = 512,
        .height = 512,
        .x_divisions = 10,
        .y_divisions = 10,
        .points = points,
        .num_points = num_points
    };

    // Volání funkce pro generování PostScript grafu
    int result = generate_postscript_graph(&params, output_file);
    if (result != 0) {
        fprintf(stderr, "Error: Failed to generate PostScript graph. Code: %d\n", result);
        free(points);
        return 6;
    }

    // Uvolnění paměti
    free(points);
    return 0;
}
