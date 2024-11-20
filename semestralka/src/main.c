#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "parser.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s \"expression\"\n", argv[0]);
        printf("Example: %s \"x^2\"\n", argv[0]);
        return 1;
    }

    const char *expr = argv[1];  // Vezmeme výraz z příkazové řádky
    printf("Evaluating expression: %s\n", expr);

    // Vyhodnocení výrazu pro x od -1 do 1 s krokem 0.5
    for (double x = 0.0; x <= 1.0; x += 0.1) {
        printf("f(%.1f) = %.3f\n", x, evaluateExpression(expr, x));
    }

    return 0;
}