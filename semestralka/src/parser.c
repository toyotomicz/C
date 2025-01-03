/*
    Mathematical Expression Parser
    Version 1.0
    Module parser.c

    Implementation of a mathematical expression parser 
    capable of processing complex expressions with various functions, 
    operators, and a single variable (x).

    Key Features:
    - Support for mathematical operations 
    - Mathematical function parsing
    - Single variable (x) support
    - Error handling and validation
    - Undefined point detection

    Expression Grammar:
    expression = term {("+"|"-") term}
    term       = factor {("*"|"/") factor}
    factor     = number | "x" | function "(" expression ")" | 
                 "(" expression ")" | "-" factor | factor "^" factor

    Dialect: ANSI C
    Compiler: Any ANSI C-compatible compiler

    Copyright (c) Jiří Joska, 2024
    Provided "AS IS" with NO WARRANTY OF ANY KIND
*/

#include "parser.h"

/* List of known mathematical functions */
const char *KNOWN_FUNCTIONS[] = {
    "abs", "exp", "ln", "log", 
    "sin", "cos", "tan", 
    "asin", "acos", "atan", 
    "sinh", "cosh", "tanh"
};
const int NUM_KNOWN_FUNCTIONS = sizeof(KNOWN_FUNCTIONS) / sizeof(KNOWN_FUNCTIONS[0]);


/* ____________________________________________________________________________
    EvaluationResult evaluate_expression(const char *expr, double x)
    
    Primary function for parsing and evaluating a mathematical expression.
    
    Parameters:
    - expr: Null-terminated string containing the mathematical expression
    - x:    Value to substitute for the variable 'x'
    
    Returns:
    EvaluationResult containing the computed value and its defined status.
    
    Responsibilities:
    - Validate input expression
    - Parse and compute expression value
    - Handle mathematical undefined scenarios
   ____________________________________________________________________________
*/
EvaluationResult evaluate_expression(const char *expr, double x) {
   /* Initialize result struct - default to "defined" state */
   EvaluationResult result = {0, 1};
   
   /* Validate input expression pointer and length */
   if (!expr || strlen(expr) >= MAX_EXPR_LEN) {
       result.is_defined = 0;
       return result;
   }
   
   /* Create safe copy of expression with length bounds */
   char validated_expr[MAX_EXPR_LEN];
   strncpy(validated_expr, expr, MAX_EXPR_LEN - 1);
   validated_expr[MAX_EXPR_LEN - 1] = '\0';
   
   /* Set up parser with validated expression */
   Parser parser = { validated_expr, x };
   result.value = parse_expression(&parser);
   
   /* Check for overflow/underflow and infinity conditions */
   if (errno == ERANGE || 
       result.value == HUGE_VAL || 
       result.value == -HUGE_VAL) {
       result.is_defined = 0;
       errno = 0;
   }
   
   return result;
}

/* ____________________________________________________________________________
    double parse_expression(Parser *p)
    
    Recursive descent parser for addition and subtraction operations.
    
    Parsing Strategy:
    - Parse initial term
    - Iteratively handle subsequent addition/subtraction operations
   ____________________________________________________________________________
*/
double parse_expression(Parser *p) {
    double result = parse_term(p);  /* Parse the first term */
    skip_whitespace(p);            /* Skip any whitespace after the term */

    /* Process addition and subtraction operators. */
    while (*p->expr == '+' || *p->expr == '-') {
        char op = *p->expr;         /* Get the current operator ('+' or '-') */
        match(p, op);              /* Consume the operator and advance the parser */
        skip_whitespace(p);        /* Skip any whitespace. */

        if (op == '+') {
            result += parse_term(p); /* Add the next term */
        } else {
            result -= parse_term(p); /* Subtract the next term */
        }

        skip_whitespace(p);        /* Skip any whitespace after the operation */
    }

    return result; /* Return the evaluated result of the expression */
}

/* ____________________________________________________________________________
    double parse_term(Parser *p)
    
    Recursive descent parser for multiplication and division operations.
    
    Parsing Strategy:
    - Parse initial factor
    - Iteratively handle subsequent multiplication/division operations
   ____________________________________________________________________________
*/
double parse_term(Parser *p) {
    double result = parse_factor(p); /* Parse the first factor */
    skip_whitespace(p);             /* Skip any whitespace after the factor */

    /* Process multiplication and division operators */
    while (*p->expr == '*' || *p->expr == '/') {
        char op = *p->expr;         /* Get the current operator ('*' or '/') */
        match(p, op);              /* Consume the operator and advance the parser */
        skip_whitespace(p);        /* Skip any whitespace. */

        if (op == '*') {
            result *= parse_factor(p); /* Multiply by the next factor */
        } else {
            double divisor = parse_factor(p); /* Parse the divisor */

            if (divisor == 0) {
                fprintf(stderr, "Error: Division by zero\n");
                exit(2); /* Terminate the program on division by zero */
            }

            result /= divisor; /* Divide by the divisor */
        }

        skip_whitespace(p);        /* Skip any whitespace after the operation */
    }

    return result; /* Return the evaluated result of the term */
}

/* ____________________________________________________________________________
    double parse_factor(Parser *p)
    
    Handles complex parsing of factors including:
    - Parenthesized expressions
    - Unary negation
    - Variable x
    - Functions
    - Exponential operation
   ____________________________________________________________________________
*/
double parse_factor(Parser *p) {
    skip_whitespace(p); /* Skip any leading whitespace */
    double result;

    if (*p->expr == '(') {
        /* Handle expressions within parentheses */
        match(p, '(');
        skip_whitespace(p);
        result = parse_expression(p); /* Recursively parse the inner expression */
        skip_whitespace(p);
        match(p, ')');
    } else if (*p->expr == '-') {
        /* Handle negation */
        match(p, '-');
        skip_whitespace(p);
        result = -parse_factor(p); /* Negate the result of the next factor */
    } else if (isalpha(*p->expr)) {
        /* Handle variables or functions */
        if (*p->expr == 'x') {
            match(p, 'x');
            result = p->x; /* Retrieve the value of the variable 'x' */
        } else {
            result = parse_function(p); /* Parse and evaluate the function */
        }
    } else {
        /* Parse a numeric value */
        result = parse_number(p);
    }

    skip_whitespace(p); /* Skip any whitespace */

    if (*p->expr == '^') {
        /* Handle exponentiation */
        match(p, '^');
        skip_whitespace(p);
        double exponent = parse_factor(p); /* Parse the exponent */
        result = pow(result, exponent);    /* Raise the base to the exponent */
    }

    return result; /* Return the evaluated result of the factor */
}

/* ____________________________________________________________________________
    double parse_function(Parser *p)
    
    Parses and evaluates mathematical functions with their arguments.
    
    Supports a wide range of mathematical functions including:
    - Trigonometric
    - Inverse trigonometric
    - Logarithmic
    - Exponential
    - Hyperbolic
   ____________________________________________________________________________
*/
double parse_function(Parser *p) {
    char funcName[10] = {0}; /* Buffer to store the function name */
    int i = 0;

    /* Extract the function name from the expression */
    while (isalpha(p->expr[i]) && i < 9) {
        funcName[i] = p->expr[i];
        i++;
    }
    p->expr += i; /* Advance the parser past the function name */

    skip_whitespace(p); /* Skip any whitespace */
    match(p, '(');     /* Match the opening parenthesis of the function argument */
    skip_whitespace(p);
    double arg = parse_expression(p); /* Parse the function argument */
    skip_whitespace(p);
    match(p, ')');     /* Match the closing parenthesis */

    /* Handle known mathematical functions */
    if (strcmp(funcName, "sin") == 0) return sin(arg);
    if (strcmp(funcName, "cos") == 0) return cos(arg);
    if (strcmp(funcName, "tan") == 0) {
        if (cos(arg) == 0) {
            errno = ERANGE;
            return HUGE_VAL; /* Undefined tangent */
        }
        return tan(arg);
    }

    if (strcmp(funcName, "asin") == 0) {
        if (arg < -1 || arg > 1) {
            errno = ERANGE;
            return HUGE_VAL; /* Undefined arcsine */
        }
        return asin(arg);
    }
    if (strcmp(funcName, "acos") == 0) {
        if (arg < -1 || arg > 1) {
            errno = ERANGE;
            return HUGE_VAL; /* Undefined arccosine */
        }
        return acos(arg);
    }
    if (strcmp(funcName, "atan") == 0) return atan(arg);

    if (strcmp(funcName, "ln") == 0) {
        if (arg <= 0) {
            errno = ERANGE;
            return HUGE_VAL; /* Undefined natural logarithm */
        }
        return log(arg);
    }
    if (strcmp(funcName, "log") == 0) {
        if (arg <= 0) {
            errno = ERANGE;
            return HUGE_VAL; /* Undefined base-10 logarithm */
        }
        return log10(arg);
    }

    if (strcmp(funcName, "exp") == 0) return exp(arg);

    if (strcmp(funcName, "sinh") == 0) return sinh(arg);
    if (strcmp(funcName, "cosh") == 0) return cosh(arg);
    if (strcmp(funcName, "tanh") == 0) return tanh(arg);

    if (strcmp(funcName, "abs") == 0) return fabs(arg);

    fprintf(stderr, "Error: Unknown function: %s\n", funcName);
    exit(2); /* Terminate the program for an unknown function */
}


/* ____________________________________________________________________________
    double parse_number(Parser *p)
    
    Parses numeric values with error checking.
    
    Supports:
    - Integer values
    - Decimal values
    - Scientific notation
    - Leading decimal points
   ____________________________________________________________________________
*/
double parse_number(Parser *p) {
    /* Reset error number for fresh error checking */
    errno = 0;

    /* 
        Special handling for numbers starting with a decimal point 
        Prepends a '0' to ensure valid parsing of leading decimal numbers
     */
    if (*p->expr == '.') {
        /* Create a buffer large enough for most number representations */
        char tempBuffer[64];  
        
        /* Prepend '0' to the decimal number */
        sprintf(tempBuffer, "0%s", p->expr);
        char *endPtr;
        
        /* Attempt to parse the number */
        double result = strtod(tempBuffer, &endPtr);
        
        /* Check for parsing failure */
        if (tempBuffer == endPtr) {
            fprintf(stderr, "Error: Invalid number format.\n");
            exit(2);
        }
        
        /* Check for number being out of representable range */
        if (errno == ERANGE) {
            fprintf(stderr, "Error: Number out of range.\n");
            exit(2);
        }
        
        /* 
            Update the parser's expression pointer 
            Subtract 1 to account for the prepended '0'
         */
        p->expr += (endPtr - tempBuffer - 1);
        return result;
    }

    /* Parse number using standard strtod */
    char *endPtr;
    double result = strtod(p->expr, &endPtr);
    
    /* Check if no parsing occurred */
    if (p->expr == endPtr) {
        fprintf(stderr, "Error: Invalid number format.\n");
        exit(2);
    }

    /* Store original expression pointer for detailed validation */
    const char *original = p->expr;
    p->expr = endPtr;
    
    /* Check for number being out of representable range */
    if (errno == ERANGE) {
        fprintf(stderr, "Error: Number out of range.\n");
        exit(2);
    }

    /* 
        Perform additional syntax validation 
        Checks for:
        - Multiple exponential notations
        - Multiple decimal points
    */
    int hasE = 0;  /* Exponential notation flag */
    int hasDot = 0;  /* Decimal point flag */
    while (original < endPtr) {
        /* Check for exponential notation */
        if (*original == 'e' || *original == 'E') {
            if (hasE != 0) {
                fprintf(stderr, "Error: Invalid number format - multiple exponential notations.\n");
                exit(2);
            }
            hasE = 1;
        } 
        /* Check for decimal points */
        else if (*original == '.') {
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

/* ____________________________________________________________________________
    void skip_whitespace(Parser *p)
    
    Advances the parser's expression pointer past whitespace characters.
   ____________________________________________________________________________
*/
void skip_whitespace(Parser *p) {
    while (isspace(*p->expr)) {
        p->expr++;
    }
}

/* ____________________________________________________________________________
    void match(Parser *p, char expected)
    
    Verifies and consumes an expected character in the expression.
    
    Error Handling Strategy:
    - Skip leading whitespace
    - Check if current character matches expected character
    - If mismatch, print error and exit
    - If match, advance parser pointer
   ____________________________________________________________________________
*/
void match(Parser *p, char expected) {
    skip_whitespace(p); /* Skip any leading whitespace */

    if (*p->expr == expected) {
        p->expr++; /* Advance the parser past the matched character */
    } else {
        if (*p->expr == '\0') {
            /* Handle end-of-expression error */
            fprintf(stderr, "Error: Expected '%c', found end of expression\n", expected);
        } else {
            /* Handle mismatched character error */
            fprintf(stderr, "Error: Expected '%c', found '%c'\n", expected, *p->expr);
        }
        exit(2); /* Terminate the program on error */
    }
}

/* ____________________________________________________________________________
    int is_operator(char c)
    
    Determines if a character represents a mathematical operator.
    
    Check Strategy:
    - Compare input character against known operators
    - Return 1 if match found, 0 otherwise
   ____________________________________________________________________________
*/
int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

/* ____________________________________________________________________________
    int is_valid_function(const char *func_name)
    
    Validates whether a given string represents a known mathematical function.
    
    Validation Strategy:
    - Iterate through list of known functions
    - Compare input function name against predefined set
    - Return 1 if match found, 0 otherwise
   ____________________________________________________________________________
*/
int is_valid_function(const char *func_name) {
    int i;
    for (i = 0; i < NUM_KNOWN_FUNCTIONS; i++) {
        if (strcmp(func_name, KNOWN_FUNCTIONS[i]) == 0) {
            return 1; /* Function name is valid */
        }
    }
    return 0; /* Function name is invalid */
}

/* ____________________________________________________________________________
    int is_unary_minus(const char *expr, char current_char)
    
    Determines if a minus sign represents a unary (negative) operator.
    
    Detection Strategy:
    - Check if current character is a minus sign
    - Verify previous character suggests unary context
    - Return 1 if unary minus, 0 otherwise
   ____________________________________________________________________________
*/
int is_unary_minus(const char *expr, char current_char) {
    return current_char == '-' && (
        *(expr - 1) == '(' || *(expr - 1) == '+' || 
        *(expr - 1) == '-' || *(expr - 1) == '*' || 
        *(expr - 1) == '/' || *(expr - 1) == '^'
    );
}

/* ____________________________________________________________________________
    int validate_number(const char *expr, const char **end)
    
    Validates numeric expressions, including scientific notation.
    
    Validation Strategy:
    - Use strtod to parse numeric string
    - Check for successful number conversion
    - Update end pointer to mark number's extent
    - Return validation result
   ____________________________________________________________________________
*/
int validate_number(const char *expr, const char **end) {
    char *endptr;
    strtod(expr, &endptr);
    
    /* If not a valid number, return 0 */
    if (expr == endptr) return 0;
    
    *end = endptr;
    return 1;
}

/* ____________________________________________________________________________
    int extract_function_name(const char *expr, char *function_name)
    
    Extracts a function name from an expression.
    
    Extraction Strategy:
    - Iterate through alphabetic characters
    - Copy characters to function name buffer
    - Limit function name length
    - Null-terminate the result
    - Return extracted function name length
   ____________________________________________________________________________
*/
int extract_function_name(const char *expr, char *function_name) {
    int func_len = 0;
    
    /* Read function name */
    while (isalpha(expr[func_len]) && func_len < 9) {
        function_name[func_len] = expr[func_len];
        func_len++;
    }
    function_name[func_len] = '\0';
    
    return func_len;
}

/* ____________________________________________________________________________
    int validate_expression(const char *expr)
    
    Syntax validation for mathematical expressions.
    
    Checks:
    - Balanced parentheses
    - Correct function and number syntax
    - No consecutive operators
    - Proper positioning of operators and functions
   ____________________________________________________________________________
*/
int validate_expression(const char *expr) {
    int paren_depth = 0;
    int last_was_operator = 1;
    int last_was_function = 0;
    int last_was_number = 0;

    /* Iterate through the expression */
    for (; *expr != '\0'; expr++) {
        /* Skip whitespace */
        if (isspace(*expr)) continue;

        /* Handle variable x */
        if (*expr == 'x') {
            last_was_operator = 0;
            last_was_function = 0;
            last_was_number = 0;
            continue;
        }

        /* Handle parentheses */
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

        /* Handle operators */
        if (is_operator(*expr)) {
            /* Check for unary minus */
            if (is_unary_minus(expr, *expr)) {
                last_was_operator = 1;
                last_was_function = 0;
                last_was_number = 0;
                continue;
            }

            /* Check for consecutive operators */
            if (last_was_operator) return 0;
            last_was_operator = 1;
            last_was_function = 0;
            last_was_number = 0;
            continue;
        }

        /* Handle functions */
        if (isalpha(*expr)) {
            char function_name[10] = {0};
            int func_len = extract_function_name(expr, function_name);

            /* Check if it is a valid function */
            if (!is_valid_function(function_name)) return 0;

            /* Check if followed by an opening parenthesis */
            expr += func_len - 1;
            if (*(expr + 1) != '(') return 0;

            last_was_operator = 0;
            last_was_function = 1;
            last_was_number = 0;
            continue;
        }

        /* Handle numbers */
        if (isdigit(*expr) || *expr == '.') {
            const char *number_end;
            if (!validate_number(expr, &number_end)) return 0;

            /* Move pointer to end of number */
            expr = number_end - 1;

            /* Check for consecutive numbers */
            if (last_was_number) return 0;
            last_was_operator = 0;
            last_was_function = 0;
            last_was_number = 1;
            continue;
        }

        /* If character doesn't match any allowed category */
        return 0;
    }

    /* Final checks */
    /* All parentheses must be closed */
    if (paren_depth != 0) return 0;
    /* Cannot end with an operator */
    if (last_was_operator) return 0;

    return 1;
}