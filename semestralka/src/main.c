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

    /* Copy and clean the function string */
    const char* input_function = argv[1];
    char temp_function[1024] = {0};
    size_t j = 0;
    
    /* Copy while removing whitespace */
    size_t i;
    for (i = 0; input_function[i] != '\0'; i++) {
        if (!isspace(input_function[i])) {
            if (j >= sizeof(temp_function) - 1) {
                fprintf(stderr, "Error: Function too long\n");
                return 1;
            }
            temp_function[j++] = input_function[i];
        }
    }
    temp_function[j] = '\0';
    
    /* Validate characters */
    for (i = 0; temp_function[i] != '\0'; i++) {
        if (!isalnum(temp_function[i]) && 
            temp_function[i] != '(' && 
            temp_function[i] != ')' && 
            temp_function[i] != '^' && 
            temp_function[i] != '*' && 
            temp_function[i] != '/' && 
            temp_function[i] != '+' && 
            temp_function[i] != '-' && 
            temp_function[i] != '.') {
            fprintf(stderr, "Error: Invalid character in function: '%c'\n", temp_function[i]);
            return 1;
        }
    }

    /* Copy cleaned function to output */
    strcpy(function, temp_function);

    /* Set output file */
    *output_file = argv[2];

    /* Parse optional range parameters if provided */
    if (argc > 3) {
        char range_copy[256];  /* Local buffer for range string */
        strncpy(range_copy, argv[3], sizeof(range_copy) - 1);
        range_copy[sizeof(range_copy) - 1] = '\0';  /* Ensure null termination */

        char *token;
        
        /* Parse xmin */
        token = strtok(range_copy, ":");
        if (!token || sscanf(token, "%lf", xmin) != 1) {
            fprintf(stderr, "Error: Invalid xmin value\n");
            return 1;
        }

        /* Parse xmax */
        token = strtok(NULL, ":");
        if (!token || sscanf(token, "%lf", xmax) != 1) {
            fprintf(stderr, "Error: Invalid xmax value\n");
            return 1;
        }

        /* Parse ymin */
        token = strtok(NULL, ":");
        if (!token || sscanf(token, "%lf", ymin) != 1) {
            fprintf(stderr, "Error: Invalid ymin value\n");
            return 1;
        }

        /* Parse ymax */
        token = strtok(NULL, ":");
        if (!token || sscanf(token, "%lf", ymax) != 1) {
            fprintf(stderr, "Error: Invalid ymax value\n");
            return 1;
        }

        /* Validate ranges */
        if (*xmax <= *xmin || *ymax <= *ymin) {
            fprintf(stderr, "Error: Invalid range (max must be greater than min)\n");
            return 1;
        }
    }

    return 0; /* Success */
}