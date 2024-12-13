#include "parser.h"

// List of known functions
const char *KNOWN_FUNCTIONS[] = {
    "abs", "exp", "ln", "log", 
    "sin", "cos", "tan", 
    "asin", "acos", "atan", 
    "sinh", "cosh", "tanh"
};
const int NUM_KNOWN_FUNCTIONS = sizeof(KNOWN_FUNCTIONS) / sizeof(KNOWN_FUNCTIONS[0]);

/* Funkce pro vyhodnocení matematického výrazu */
EvaluationResult evaluate_expression(const char *expr, double x) {
    EvaluationResult result = {0, 1};  // Default to defined
    
    // Try to catch potential undefined points
    errno = 0;
    
    Parser parser = { expr, x };
    
    // Use a try-catch like approach with errno and specific checks
    result.value = parse_expression(&parser);
    
    // Check for specific undefined conditions
    if (errno == ERANGE) {
        result.is_defined = 0;
        errno = 0;
    }
    
    // Additional specific checks for undefined points
    if (isnan(result.value) || isinf(result.value)) {
        result.is_defined = 0;
    }
    
    return result;
}

/* Funkce pro parsování výrazu */
double parse_expression(Parser *p) {
    double result = parse_term(p);
    skip_whitespace(p);
    while (*p->expr == '+' || *p->expr == '-') {
        char op = *p->expr;
        match(p, op);
        skip_whitespace(p);
        if (op == '+') {
            result += parse_term(p);
        } else {
            result -= parse_term(p);
        }
        skip_whitespace(p);
    }
    return result;
}

/* Funkce pro parsování termu */
double parse_term(Parser *p) {
    double result = parse_factor(p);
    skip_whitespace(p);
    while (*p->expr == '*' || *p->expr == '/') {
        char op = *p->expr;
        match(p, op);
        skip_whitespace(p);
        if (op == '*') {
            result *= parse_factor(p);
        } else {
            double divisor = parse_factor(p);
            if (divisor == 0) {
                fprintf(stderr, "Error: Division by zero\n");
                exit(2);
            }
            result /= divisor;
        }
        skip_whitespace(p);
    }
    return result;
}

/* Funkce pro parsování faktoru */
double parse_factor(Parser *p) {
    skip_whitespace(p);
    double result;

    if (*p->expr == '(') {
        match(p, '(');
        skip_whitespace(p);
        result = parse_expression(p);
        skip_whitespace(p);
        match(p, ')');
    } else if (*p->expr == '-') {
        match(p, '-');
        skip_whitespace(p);
        result = -parse_factor(p);
    } else if (isalpha(*p->expr)) {
        if (*p->expr == 'x') {
            match(p, 'x');
            result = p->x;
        } else {
            result = parse_function(p);
        }
    } else {
        result = parse_number(p);
    }

    skip_whitespace(p);
    if (*p->expr == '^') {
        match(p, '^');
        skip_whitespace(p);
        double exponent = parse_factor(p);
        result = pow(result, exponent);
    }

    return result;
}

/* Funkce pro parsování matematických funkcí */
double parse_function(Parser *p) {
    char funcName[10] = {0};
    int i = 0;

    // Extract the function name
    while (isalpha(p->expr[i]) && i < 9) {
        funcName[i] = p->expr[i];
        i++;
    }
    p->expr += i;

    skip_whitespace(p);
    match(p, '(');
    skip_whitespace(p);
    double arg = parse_expression(p);
    skip_whitespace(p);
    match(p, ')');

    // Trigonometric functions
    if (strcmp(funcName, "sin") == 0) return sin(arg);
    if (strcmp(funcName, "cos") == 0) return cos(arg);
    if (strcmp(funcName, "tan") == 0) {
        if (cos(arg) == 0) {
            errno = ERANGE;
            return NAN;  // Undefined tangent
        }
        return tan(arg);
    }

    // Inverse trigonometric functions
    if (strcmp(funcName, "asin") == 0) {
        if (arg < -1 || arg > 1) {
            errno = ERANGE;
            return NAN;  // Undefined arcsine
        }
        return asin(arg);
    }
    if (strcmp(funcName, "acos") == 0) {
        if (arg < -1 || arg > 1) {
            errno = ERANGE;
            return NAN;  // Undefined arccosine
        }
        return acos(arg);
    }
    if (strcmp(funcName, "atan") == 0) return atan(arg);

    // Logarithmic functions
    if (strcmp(funcName, "ln") == 0) {
        if (arg <= 0) {
            errno = ERANGE;
            return NAN;  // Undefined natural logarithm
        }
        return log(arg);
    }
    if (strcmp(funcName, "log") == 0) {
        if (arg <= 0) {
            errno = ERANGE;
            return NAN;  // Undefined base-10 logarithm
        }
        return log10(arg);
    }

    // Exponential functions
    if (strcmp(funcName, "exp") == 0) return exp(arg);

    // Hyperbolic functions
    if (strcmp(funcName, "sinh") == 0) return sinh(arg);
    if (strcmp(funcName, "cosh") == 0) return cosh(arg);
    if (strcmp(funcName, "tanh") == 0) return tanh(arg);

    // Absolute value
    if (strcmp(funcName, "abs") == 0) return fabs(arg);

    fprintf(stderr, "Error: Unknown function: %s\n", funcName);
    exit(2);
}


/* Funkce pro parsování čísel */
double parse_number(Parser *p) {
    errno = 0;

    if (*p->expr == '.') {
        char *tempExpr = (char *)malloc(strlen(p->expr) + 2);
        if (tempExpr == NULL) {
            fprintf(stderr, "Error: Memory allocation failed.\n");
            exit(2);  // Neakceptovatelný zápis - chyba při zpracování
        }
        strcpy(tempExpr, "0");
        strcat(tempExpr, p->expr);
        char *endPtr;
        double result = strtod(tempExpr, &endPtr);
        
        if (tempExpr == endPtr) {
            free(tempExpr);
            fprintf(stderr, "Error: Invalid number format.\n");
            exit(2);
        }
        
        if (errno == ERANGE) {
            free(tempExpr);
            fprintf(stderr, "Error: Number out of range.\n");
            exit(2);
        }
        
        p->expr += (endPtr - tempExpr - 1);
        free(tempExpr);
        return result;
    }

    char *endPtr;
    double result = strtod(p->expr, &endPtr);
    
    if (p->expr == endPtr) {
        fprintf(stderr, "Error: Invalid number format.\n");
        exit(2);
    }

    const char *original = p->expr;
    p->expr = endPtr;
    
    if (errno == ERANGE) {
        fprintf(stderr, "Error: Number out of range.\n");
        exit(2);
    }

    int hasE = 0;
    int hasDot = 0;
    while (original < endPtr) {
        if (*original == 'e' || *original == 'E') {
            if (hasE) {
                fprintf(stderr, "Error: Invalid number format - multiple exponential notations.\n");
                exit(2);
            }
            hasE = 1;
        } else if (*original == '.') {
            if (hasDot) {
                fprintf(stderr, "Error: Invalid number format - multiple decimal points.\n");
                exit(2);
            }
            hasDot = 1;
        }
        original++;
    }

    return result;
}

/* Funkce pro přeskočení bílých znaků */
void skip_whitespace(Parser *p) {
    while (isspace(*p->expr)) {
        p->expr++;
    }
}

/* Funkce pro kontrolu očekávaného znaku */
void match(Parser *p, char expected) {
    skip_whitespace(p);
    if (*p->expr == expected) {
        p->expr++;
    } else {
        if (*p->expr == '\0') {
            fprintf(stderr, "Error: Expected '%c', found end of expression\n", expected);
        } else {
            fprintf(stderr, "Error: Expected '%c', found '%c'\n", expected, *p->expr);
        }
        exit(2);
    }
}

// Check if a character is an operator
int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

// Check if a given function name is valid
int is_valid_function(const char *func_name) {
    for (int i = 0; i < NUM_KNOWN_FUNCTIONS; i++) {
        if (strcmp(func_name, KNOWN_FUNCTIONS[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Check if a minus sign is a unary minus
int is_unary_minus(const char *expr, char current_char) {
    return current_char == '-' && (
        *(expr - 1) == '(' || *(expr - 1) == '+' || 
        *(expr - 1) == '-' || *(expr - 1) == '*' || 
        *(expr - 1) == '/' || *(expr - 1) == '^'
    );
}

// Validate a number (including scientific notation)
int validate_number(const char *expr, const char **end) {
    char *endptr;
    strtod(expr, &endptr);
    
    // If not a valid number, return 0
    if (expr == endptr) return 0;
    
    *end = endptr;
    return 1;
}

// Extract a function name
int extract_function_name(const char *expr, char *function_name) {
    int func_len = 0;
    
    // Read function name
    while (isalpha(expr[func_len]) && func_len < 9) {
        function_name[func_len] = expr[func_len];
        func_len++;
    }
    function_name[func_len] = '\0';
    
    return func_len;
}

// Main validation function
int validate_expression(const char *expr) {
    int paren_depth = 0;
    int last_was_operator = 1;
    int last_was_function = 0;
    int last_was_number = 0;

    // Iterate through the expression
    for (; *expr != '\0'; expr++) {
        // Skip whitespace
        if (isspace(*expr)) continue;

        // Handle variable x
        if (*expr == 'x') {
            last_was_operator = 0;
            last_was_function = 0;
            last_was_number = 0;
            continue;
        }

        // Handle parentheses
        if (*expr == '(') {
            paren_depth++;
            if (!last_was_operator && !last_was_function) return 0;
            last_was_operator = 1;
            last_was_function = 0;
            last_was_number = 0;
            continue;
        }

        if (*expr == ')') {
            paren_depth--;
            if (paren_depth < 0) return 0;
            last_was_operator = 0;
            last_was_function = 0;
            last_was_number = 1;
            continue;
        }

        // Handle operators
        if (is_operator(*expr)) {
            // Check for unary minus
            if (is_unary_minus(expr, *expr)) {
                last_was_operator = 1;
                last_was_function = 0;
                last_was_number = 0;
                continue;
            }

            // Check for consecutive operators
            if (last_was_operator) return 0;
            last_was_operator = 1;
            last_was_function = 0;
            last_was_number = 0;
            continue;
        }

        // Handle functions
        if (isalpha(*expr)) {
            char function_name[10] = {0};
            int func_len = extract_function_name(expr, function_name);

            // Check if it's a valid function
            if (!is_valid_function(function_name)) return 0;

            // Check if followed by an opening parenthesis
            expr += func_len - 1;
            if (*(expr + 1) != '(') return 0;

            last_was_operator = 0;
            last_was_function = 1;
            last_was_number = 0;
            continue;
        }

        // Handle numbers
        if (isdigit(*expr) || *expr == '.') {
            const char *number_end;
            if (!validate_number(expr, &number_end)) return 0;

            // Move pointer to end of number
            expr = number_end - 1;

            // Check for consecutive numbers
            if (last_was_number) return 0;
            last_was_operator = 0;
            last_was_function = 0;
            last_was_number = 1;
            continue;
        }

        // If character doesn't match any allowed category
        return 0;
    }

    // Final checks
    // All parentheses must be closed
    if (paren_depth != 0) return 0;
    // Cannot end with an operator
    if (last_was_operator) return 0;

    return 1;
}