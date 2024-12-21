/*
    Mathematical Expression Parser
    Version 1.0
    Module main.c

    This module composes all the parts of the analyzer together and
    contains the executable entry point function. It is also responsible
    for the user interaction.

    Dialect: ANSI C
    Compiler: Any ANSI C-compatible compiler

    Copyright (c) Jiří Joska, 2024
    Provided "AS IS" with NO WARRANTY OF ANY KIND
*/

#include <math.h>    /* Mathematical functions and constants */
#include <stdlib.h>  /* General utilities and memory management */
#include <stdio.h>   /* Input/output operations */
#include <string.h>  /* String manipulation functions */
#include <stdbool.h> /* Boolean type and constants (true, false) */
#include <ctype.h>   /* Character type functions */
#include <errno.h>   /* Error number definitions */
#include "parser.h"  /* Mathematical expression parser functions */
#include "postscript.h" /* PostScript graph generation utilities */

/* Function prototypes */
void trim_spaces(char *dest, const char *src);
void combine_args(char *buffer, int argc, char *argv[]);

/* ____________________________________________________________________________
 
    MAIN PROGRAM
   ____________________________________________________________________________
*/

int main(int argc, char *argv[]) {
    if (argc < 3) {
        /* Validate minimum number of arguments */
        fprintf(stderr, "Usage: %s <function> <output_file> [xmin:xmax:ymin:ymax]\n", argv[0]);
        fprintf(stderr, "Example: %s \"sin(x^2)\" output.ps\n", argv[0]);
        fprintf(stderr, "Example with limits: %s \"sin(x^2)\" output.ps -10:10:-1:1\n", argv[0]);
        return 1; /* Exit code for insufficient arguments */
    }

    /* Retrieve and process the mathematical function */
    char function_buffer[1024];
    combine_args(function_buffer, argc - 1, argv);
    char function_trimmed[1024];
    trim_spaces(function_trimmed, function_buffer);

    const char *function = function_trimmed;
    const char *output_file = argv[argc - 1];

    /* Set default graph range */
    double xmin = -10, xmax = 10, ymin = -10, ymax = 10;

    /* Parse optional range argument */
    if (argc >= 4 && sscanf(argv[argc - 1], "%lf:%lf:%lf:%lf", &xmin, &xmax, &ymin, &ymax) == 4) {
        argc--; /* Adjust argc to account for range argument */
    }
    if (xmin >= xmax || ymin >= ymax) {
        fprintf(stderr, "Error: Invalid range format or logical range error. Use xmin:xmax:ymin:ymax and ensure xmin < xmax, ymin < ymax.\n");
        return 4; /* Exit code for invalid range */
    }

    /* Validate the provided mathematical expression */
    if (!validate_expression(function)) {
        fprintf(stderr, "Error: Invalid mathematical expression.\n");
        return 2; /* Exit code for invalid expression */
    }

    /* Ensure the output file is writable */
    FILE *file = fopen(output_file, "w");
    if (!file) {
        fprintf(stderr, "Error: Cannot create or write to output file '%s'.\n", output_file);
        return 3; /* Exit code for file I/O error */
    }
    fclose(file);

    /* Allocate memory for graph points */
    int num_points = 512;
    double *points = malloc(num_points * sizeof(double));
    if (!points) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        return 5; /* Exit code for memory allocation failure */
    }

    /* Generate graph points by evaluating the function */
    double step = (xmax - xmin) / (num_points - 1);
    bool has_undefined_values = false;

    for (int i = 0; i < num_points; i++) {
        double x = xmin + i * step;
        EvaluationResult eval = evaluate_expression(function, x);
        if (eval.is_defined) {
            points[i] = eval.value;
        } else {
            has_undefined_values = true;
            points[i] = NAN; /* Undefined values are set to NaN */
        }
    }

    if (has_undefined_values) {
        fprintf(stderr, "Warning: The function contains undefined values in the given range.\n");
    }

    /* Configure graph parameters */
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

    /* Generate PostScript graph */
    int result = generate_postscript_graph(&params, output_file);
    if (result != 0) {
        fprintf(stderr, "Error: Failed to generate PostScript graph. Code: %d\n", result);
        free(points);
        return 6; /* Exit code for graph generation failure */
    }

    /* Free allocated memory and exit */
    free(points);
    return 0;
}

/* ____________________________________________________________________________
    trim_spaces

    Removes all whitespace characters (spaces and tabs) from the source string
    and stores the result in the destination buffer.

    Parameters:
        dest - Destination buffer for the trimmed string
        src  - Source string to trim
   ____________________________________________________________________________
*/
void trim_spaces(char *dest, const char *src) {
    while (*src) {
        if (*src != ' ' && *src != '\t') {
            *dest++ = *src;
        }
        src++;
    }
    *dest = '\0'; /* Null-terminate the destination string */
}

/* ____________________________________________________________________________
    combine_args

    Combines all arguments from index 1 to (argc - 2) into a single string buffer, 
    separating them with spaces.

    Parameters:
        buffer - Destination buffer for the combined arguments
        argc   - Number of command-line arguments
        argv   - Array of command-line arguments
   ____________________________________________________________________________
*/
void combine_args(char *buffer, int argc, char *argv[]) {
    buffer[0] = '\0'; /* Initialize buffer as an empty string */
    for (int i = 1; i < argc - 1; i++) { /* Combine all arguments except the last */
        strcat(buffer, argv[i]);
        strcat(buffer, " ");
    }
    strcat(buffer, argv[argc - 1]); /* Add the last argument */
}