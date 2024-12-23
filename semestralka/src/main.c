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
int parse_command_args(int argc, char *argv[], 
                      char *function, char **output_file,
                      double *xmin, double *xmax, 
                      double *ymin, double *ymax);

/* ____________________________________________________________________________
 
    MAIN PROGRAM
   ____________________________________________________________________________
*/

int main(int argc, char *argv[]) {
    char function[1024];
    char *output_file;
    double xmin, xmax, ymin, ymax;

    /* Parse command line arguments */
    int parse_result = parse_command_args(argc, argv, function, &output_file,
                                        &xmin, &xmax, &ymin, &ymax);
    if (parse_result != 0) {
        return parse_result;
    }

    /* Validate the provided mathematical expression */
    if (!validate_expression(function)) {
        fprintf(stderr, "Error: Invalid mathematical expression.\n");
        return 2;
    }

    /* Ensure the output file is writable */
    FILE *file = fopen(output_file, "w");
    if (!file) {
        fprintf(stderr, "Error: Cannot create or write to output file '%s'.\n", output_file);
        return 3;
    }
    fclose(file);

    /* Allocate memory for graph points */
    int num_points = 512;
    double *points = malloc(num_points * sizeof(double));
    if (!points) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        return 5;
    }

    /* Generate graph points by evaluating the function */
    double step = (xmax - xmin) / (num_points - 1);
    bool has_undefined_values = false;

    int i;
    for (i = 0; i < num_points; i++) {
        double x = xmin + i * step;
        EvaluationResult eval = evaluate_expression(function, x);
        if (eval.is_defined) {
            points[i] = eval.value;
        } else {
            has_undefined_values = true;
            points[i] = 0.f / 0.f; /* NaN for undefined values, 0.f / 0.f is a trick in ANSI C to get NaN value */
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
        return 6;
    }

    /* Free allocated memory and exit */
    free(points);
    return 0;
}

/* ____________________________________________________________________________
    parse_command_args

    Parses command line arguments, handling both quoted and unquoted mathematical
    functions. If the function is unquoted, it must not contain whitespace.
    If it is quoted, whitespace is allowed.

    Parameters:
        argc        - Number of command-line arguments
        argv        - Array of command-line arguments
        function    - Buffer to store the parsed function
        output_file - Buffer to store the output file path
        xmin, xmax, ymin, ymax - Pointers to store the range values
        
    Returns:
        0 on success, error code on failure
   ____________________________________________________________________________
*/
int parse_command_args(int argc, char *argv[], 
                      char *function, char **output_file,
                      double *xmin, double *xmax, 
                      double *ymin, double *ymax) {
    /* Verify minimum required arguments (program name, function, output file) */
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <function> <output_file> [xmin:xmax:ymin:ymax]\n", argv[0]);
        fprintf(stderr, "Example: %s \"sin(x^2)\" output.ps\n", argv[0]);
        fprintf(stderr, "Example with limits: %s \"sin(x^2)\" output.ps -10:10:-1:1\n", argv[0]);
        fprintf(stderr, "Note: Quotes are optional if function contains no spaces\n");
        return 1;
    }

    /* Set default plotting range values */
    *xmin = -10;
    *xmax = 10;
    *ymin = -10;
    *ymax = 10;

    /* Get first argument which should contain the function or start of function */
    const char *first_arg = argv[1];

    /* Check if the function is quoted (starts with ") */
    if (first_arg[0] == '"') {
        /* 
            Handle quoted function case
            Need to combine arguments until we find closing quote
            This allows functions with spaces like "sin ( x )"
        */
        char buffer[1024] = "";             /* Buffer to store combined function */
        int arg_index = 1;                  /* Current argument being processed */
        bool found_closing_quote = false;   /* Flag for tracking if we found closing quote */

        /* Process arguments until we find closing quote or run out of args */
        while (arg_index < argc - 1) { /* -1 ensures space for output_file */
            const char *current_arg = argv[arg_index];
            size_t arg_len = strlen(current_arg);

            /* Check if current argument contains closing quote */
            if (current_arg[arg_len - 1] == '"') {
                /* 
                    Found closing quote
                    If this is first argument (arg_index == 1), need to skip opening quote
                    Always need to skip closing quote
                */
                strncat(buffer, current_arg + (arg_index == 1 ? 1 : 0), 
                       arg_len - (arg_index == 1 ? 2 : 1));
                found_closing_quote = true;
                arg_index++;
                break;
            } else if (arg_index == 1) {
                /* First argument - skip opening quote and add space */
                strcat(buffer, current_arg + 1);
                strcat(buffer, " ");
            } else {
                /* Middle argument - add whole argument and space */
                strcat(buffer, current_arg);
                strcat(buffer, " ");
            }
            arg_index++;
        }

        /* Verify we found closing quote */
        if (!found_closing_quote) {
            fprintf(stderr, "Error: Missing closing quote in function argument\n");
            return 1;
        }

        /* Store processed function and output file */
        strcpy(function, buffer);
        *output_file = argv[arg_index];

        /* Check for optional range argument after output file */
        if (arg_index + 1 < argc) {
            /* Try to parse range in format xmin:xmax:ymin:ymax */
            if (sscanf(argv[arg_index + 1], "%lf:%lf:%lf:%lf", 
                      xmin, xmax, ymin, ymax) != 4) {
                fprintf(stderr, "Error: Invalid range format. Use xmin:xmax:ymin:ymax\n");
                return 1;
            }
        }
    } else {
        /* 
            Handle unquoted function case
            Function must be a single argument without spaces
        */
        strcpy(function, first_arg);
        *output_file = argv[2];

        /* Check for optional range argument */
        if (argc > 3) {
            /* Try to parse range in format xmin:xmax:ymin:ymax */
            if (sscanf(argv[3], "%lf:%lf:%lf:%lf", 
                      xmin, xmax, ymin, ymax) != 4) {
                fprintf(stderr, "Error: Invalid range format. Use xmin:xmax:ymin:ymax\n");
                return 1;
            }
        }
    }

    /* Validate that min values are less than max values */
    if (*xmin >= *xmax || *ymin >= *ymax) {
        fprintf(stderr, "Error: Invalid range values. Ensure xmin < xmax and ymin < ymax\n");
        return 1;
    }

    return 0; /* Success */
}