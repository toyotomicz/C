#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "parser.h"

#define MAX_STACK_SIZE 100

typedef struct {
    double items[MAX_STACK_SIZE];
    int top;
} Stack;

void push(Stack *stack, double value) {
    stack->items[++stack->top] = value;
}

double pop(Stack *stack) {
    return stack->items[stack->top--];
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

int precedence(char op) {
    switch (op) {
        case '+': case '-': return 1;
        case '*': case '/': return 2;
        case '^': return 3;
        default: return 0;
    }
}

int is_function(const char *token) {
    const char *functions[] = {"sin", "cos", "tan", "exp", "log", "sqrt", "abs", "asin", "acos", "atan", "sinh", "cosh", "tanh"};
    for (int i = 0; i < sizeof(functions) / sizeof(functions[0]); i++) {
        if (strcmp(token, functions[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Convert infix expression to postfix
int infix_to_postfix(const char *infix, char postfix[][MAX_FUNC_LEN]) {
    Stack op_stack = {.top = -1};
    int pos = 0;
    char token[MAX_FUNC_LEN];
    const char *p = infix;

    while (*p) {
        if (isspace(*p)) {
            p++;
            continue;
        }
        if (isdigit(*p) || (*p == 'x') || (*p == 'X')) {
            int len = 0;
            while (isdigit(*p) || *p == '.' || *p == 'x' || *p == 'X') {
                token[len++] = *p++;
            }
            token[len] = '\0';
            strcpy(postfix[pos++], token);
            continue;
        }
        if (isalpha(*p)) {
            int len = 0;
            while (isalpha(*p)) {
                token[len++] = *p++;
            }
            token[len] = '\0';
            if (is_function(token)) {
                strcpy(postfix[pos++], token);
            } else {
                return 0;  // Unsupported function
            }
            continue;
        }
        if (*p == '(') {
            push(&op_stack, *p++);
        } else if (*p == ')') {
            while (op_stack.top != -1 && (char)op_stack.items[op_stack.top] != '(') {
                snprintf(postfix[pos++], MAX_FUNC_LEN, "%c", (char)pop(&op_stack));
            }
            pop(&op_stack);
            p++;
        } else if (is_operator(*p)) {
            while (op_stack.top != -1 && precedence((char)op_stack.items[op_stack.top]) >= precedence(*p)) {
                snprintf(postfix[pos++], MAX_FUNC_LEN, "%c", (char)pop(&op_stack));
            }
            push(&op_stack, *p++);
        } else {
            return 0;  // Invalid character
        }
    }
    while (op_stack.top != -1) {
        snprintf(postfix[pos++], MAX_FUNC_LEN, "%c", (char)pop(&op_stack));
    }
    return pos;
}

// Evaluate a postfix expression with a given x value
double evaluate_postfix(char postfix[][MAX_FUNC_LEN], int size, double x_value) {
    Stack stack = {.top = -1};
    for (int i = 0; i < size; i++) {
        char *token = postfix[i];
        if (strcmp(token, "x") == 0) {
            push(&stack, x_value);
        } else if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            push(&stack, atof(token));
        } else if (is_function(token)) {
            double a = pop(&stack);
            if (strcmp(token, "sin") == 0) push(&stack, sin(a));
            else if (strcmp(token, "cos") == 0) push(&stack, cos(a));
            else if (strcmp(token, "tan") == 0) push(&stack, tan(a));
            else if (strcmp(token, "exp") == 0) push(&stack, exp(a));
            else if (strcmp(token, "log") == 0) push(&stack, log(a));
            else if (strcmp(token, "sqrt") == 0) push(&stack, sqrt(a));
            else if (strcmp(token, "abs") == 0) push(&stack, fabs(a));
        } else if (is_operator(token[0])) {
            double b = pop(&stack);
            double a = pop(&stack);
            switch (token[0]) {
                case '+': push(&stack, a + b); break;
                case '-': push(&stack, a - b); break;
                case '*': push(&stack, a * b); break;
                case '/': push(&stack, a / b); break;
                case '^': push(&stack, pow(a, b)); break;
            }
        }
    }
    return pop(&stack);
}

// Full function to parse, convert and evaluate an expression at x
double evaluate_function(const char *func_str, double x) {
    char postfix[MAX_STACK_SIZE][MAX_FUNC_LEN];
    int postfix_size = infix_to_postfix(func_str, postfix);

    if (postfix_size == 0) {
        fprintf(stderr, "Error: Invalid function format.\n");
        exit(ERR_INVALID_FUNC);
    }
    return evaluate_postfix(postfix, postfix_size, x);
}
