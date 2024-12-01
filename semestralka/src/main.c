#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <float.h>
#include "parser.h"
#include "postscript.h"

#define MAX_STEPS 1000

// Structure to represent a result with validity
typedef struct {
    double x;
    double result;
    int is_valid;
} ResultEntry;

int main(int argc, char *argv[]) {
    // Kontrola počtu parametrů
    if (argc < 2) {
        fprintf(stderr, "Error: Program requires exactly one parameter - the mathematical expression.\n");
        fprintf(stderr, "Usage: %s \"expression\"\n", argv[0]);
        return 1;
    }

    const char *expr = argv[1];

    // Kontrola syntaxe výrazu
    if (!validateExpression(expr)) {
        fprintf(stderr, "Error: Invalid mathematical expression.\n");
        return 2;
    }
    
    int from = -5;
    int to = 5;
    int number_of_steps = 10;
    double step = (double)(to - from) / number_of_steps;

    // Alokace pole pro výsledky
    ResultEntry *results = malloc(number_of_steps + 1 * sizeof(ResultEntry));
    if (results == NULL) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        return 3;
    }

    // Výpočet a uložení výsledků
    for (int i = 0; i <= number_of_steps; i++) {
        double x = from + i * step;
        results[i].x = x;
        
        // Výpočet výsledku s kontrolou validity
        double result = evaluateExpression(expr, x);
        
        // Kontrola validity výsledku
        if (isfinite(result)) {
            results[i].result = result;
            results[i].is_valid = 1;
            printf("%2.2f\t%.10g\n", x, result);
        } else {
            results[i].result = 0.0;
            results[i].is_valid = 0;
            printf("%2.2f\tNot Valid\n", x);
        }
    }

    // Zde můžete dále pracovat s polem results

    // Uvolnění paměti
    free(results);

    return 0;
}