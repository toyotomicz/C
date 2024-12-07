#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "parser.h"
#include "postscript.h"

int main(int argc, char *argv[]) {
    // Kontrola minimálního počtu parametrů
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <function> <output_file> [xmin:xmax:ymin:ymax]\n", argv[0]);
        fprintf(stderr, "Example: %s \"sin(x^2)\" output.ps\n", argv[0]);
        fprintf(stderr, "Example with limits: %s \"sin(x^2)\" output.ps -10:10:-1:1\n", argv[0]);
        return 1;
    }

    // Parametry
    const char *function = argv[1];
    const char *output_file = argv[2];

    // Výchozí rozsahy
    double xmin = -10, xmax = 10, ymin = -10, ymax = 10;

    // Kontrola a parsování volitelného rozsahu
    if (argc == 4) {
        if (sscanf(argv[3], "%lf:%lf:%lf:%lf", &xmin, &xmax, &ymin, &ymax) != 4 || xmin >= xmax || ymin >= ymax) {
            fprintf(stderr, "Error: Invalid range format or logical range error. Use xmin:xmax:ymin:ymax and ensure xmin < xmax, ymin < ymax.\n");
            return 4;
        }
    }

    // Validace funkce
    if (!validateExpression(function)) {
        fprintf(stderr, "Error: Invalid mathematical expression.\n");
        return 2;
    }

    // Kontrola, zda lze vytvořit výstupní soubor
    FILE *file = fopen(output_file, "w");
    if (!file) {
        fprintf(stderr, "Error: Cannot create or write to output file '%s'.\n", output_file);
        return 3;
    }
    fclose(file); // Soubor zavíráme, aby byl použit později v PostScriptové funkci

    // Generování bodů pro graf
    int num_points = 512; // Počet bodů
    double *points = malloc(num_points * sizeof(double));
    if (!points) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        return 5;
    }

    double step = (xmax - xmin) / (num_points - 1);
    bool has_undefined_values = false; // Indikátor, zda funkce obsahuje nedefinované body
    for (int i = 0; i < num_points; i++) {
        double x = xmin + i * step;
        EvaluationResult eval = evaluateExpression(function, x);
        
        if (eval.is_defined) {
            points[i] = eval.value;
        } else {
            has_undefined_values = true;
            points[i] = NAN; // Nastavení na NAN pro nedefinované hodnoty
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
        .width = 512,  // Šířka grafu v jednotkách
        .height = 512, // Výška grafu v jednotkách
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
