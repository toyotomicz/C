#ifndef PARSER_H
#define PARSER_H

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

// Extern declarations for the known functions array
extern const char *KNOWN_FUNCTIONS[];
extern const int NUM_KNOWN_FUNCTIONS;

// Struct for parser context
typedef struct {
    const char *expr; // Pointer to the current position in the expression
    double x;         // Value of the variable 'x'
} Parser;

typedef struct {
    double value;
    int is_defined;
} EvaluationResult;

EvaluationResult evaluate_expression(const char *expr, double x);
double parse_expression(Parser *p);
double parse_term(Parser *p);
double parse_factor(Parser *p);
double parse_function(Parser *p);
double parse_number(Parser *p);
void skip_whitespace(Parser *p);
void match(Parser *p, char expected);
int validate_expression(const char *expr);

#endif // PARSER_H
