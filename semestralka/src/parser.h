#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <errno.h>

/* Struktura pro uchovávání parseru */
typedef struct {
    const char *expr;
    double x;
} Parser;

/* Deklarace funkcí */
double evaluateExpression(const char *expr, double x);
double parseExpression(Parser *p);
double parseTerm(Parser *p);
double parseFactor(Parser *p);
double parseNumber(Parser *p);
double parseFunction(Parser *p);
void skipWhitespace(Parser *p);
void match(Parser *p, char expected);
int isValidNumber(const char *expr);

#endif /* PARSER_H */
