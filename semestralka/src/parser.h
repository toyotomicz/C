#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAX_FUNC_LEN 50
#define ERR_INVALID_FUNC 1
#define MAX_STACK_SIZE 100

/* Checks if a character is a valid operator */
int is_operator(char c);

/* Returns the precedence of an operator */
int precedence(char op);

/* Checks if a token is a recognized function */
int is_function(const char *token);

/* Converts an infix expression to postfix notation */
int infix_to_postfix(const char *infix, char postfix[][MAX_FUNC_LEN]);

/* Applies an operator on two operands */
double apply_operator(double a, double b, char op);

/* Applies a function on a single operand */
double apply_function(const char *func, double val);

/* Evaluates a postfix expression */
double evaluate_postfix(char postfix[][MAX_FUNC_LEN], int size, double x_value);

/* Evaluates a mathematical function string at a given x value */
double evaluate_function(const char *func_str, double x);

#endif /* PARSER_H */
