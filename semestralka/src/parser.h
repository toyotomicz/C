#ifndef PARSER_H
#define PARSER_H

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

// Struct for parser context
typedef struct {
    const char *expr; // Pointer to the current position in the expression
    double x;         // Value of the variable 'x'
} Parser;

// Function prototypes
double evaluateExpression(const char *expr, double x);
double parseExpression(Parser *p);
double parseTerm(Parser *p);
double parseFactor(Parser *p);
double parseFunction(Parser *p);
double parseNumber(Parser *p);
void skipWhitespace(Parser *p);
void match(Parser *p, char expected);
int validateExpression(const char *expr);

#endif // PARSER_H
