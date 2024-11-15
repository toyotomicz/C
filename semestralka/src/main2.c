#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "parser.h"

/* error codes */
#define SUCCESS 0
#define ERR_MISSING_ARGS 1
#define ERR_INVALID_FUNC 2
#define ERR_FILE_CREATION 3
#define ERR_INVALID_LIMITS 4


char* process_function_string(const char *input) {
    size_t len = strlen(input);
    char *result;

    /* Remove quotes if present */
    if (len >= 1 && input[0] == '"' && input[len-1] == '"') {
        result = malloc(len - 1); /* -2 for quotes, +1 for null terminator */
        if (!result) return NULL;
        strncpy(result, input + 1, len - 2);
        result[len - 2] = '\0';
    } else {
        result = strdup(input);
    }

    return result;
}

int main(int argc, char *argv[]) {
    char *func_str = process_function_string(argv[1]);

    double x = 2; // Example input value

    /* Evaluate and print the function result */
    double result = evaluate_function(func_str, x);
    printf("%s\n", func_str);
    printf("The result of evaluating the function at x = %.2f is: %.2f\n", x, result);

    free(func_str);
    return SUCCESS;
}