/*
    Mathematical Expression Parser
    Version 1.0
    Header parser.h

    Comprehensive implementation of a mathematical expression parser 
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

#ifndef PARSER_H
#define PARSER_H

#include <math.h>    /* Mathematical functions and constants */
#include <stdlib.h>  /* General utilities and memory management */
#include <stdio.h>   /* Input/output operations */
#include <string.h>  /* String manipulation functions */
#include <ctype.h>   /* Character type functions */
#include <errno.h>   /* Error number definitions */
#include <stdio.h>

/*
  Array of supported mathematical function names.
  Contains strings representing all valid function names that can be used
  in expressions. Includes:
  - Trigonometric functions (sin, cos, tan)
  - Inverse trigonometric functions (asin, acos, atan)
  - Hyperbolic functions (sinh, cosh, tanh)
  - Logarithmic functions (ln, log)
  - Other functions (abs, exp)
*/
extern const char *KNOWN_FUNCTIONS[];

/*
  Number of supported mathematical functions in KNOWN_FUNCTIONS array.
  Used for bounds checking and function validation during parsing.
  Updated automatically when new functions are added to KNOWN_FUNCTIONS.
 */
extern const int NUM_KNOWN_FUNCTIONS;

/*
  Parser context structure
  Maintains the current state of the parsing process including the expression
  being parsed and any variables needed during evaluation.
  
  Members:
  expr - Pointer to the current position in the expression string
         Updated as parsing progresses through the expression
  x    - Value of the variable 'x' for expression evaluation
         Used when 'x' is encountered in the expression
  
  Usage:
  - Created and initialized by evaluate_expression()
  - Passed to parsing functions to maintain state
  - expr pointer is modified during parsing
*/
typedef struct {
    const char *expr;  /* Current position in expression string */
    double x;         /* Value of variable x for evaluation */
} Parser;

/*
  Structure containing the result of expression evaluation
  Encapsulates both the computed value and its mathematical validity.
  
  Members:
  value      - Computed numerical result of the expression
               Undefined if is_defined is 0
  is_defined - Flag indicating whether the result is mathematically defined
               0 for undefined (e.g., division by zero)
               1 for defined
  
  Usage:
  - Returned by evaluate_expression()
  - Check is_defined before using value
*/
typedef struct {
    double value;     /* Numerical result */
    int is_defined;   /* Definition status */
} EvaluationResult;

/*
  Primary function for evaluating mathematical expressions
  Entry point for the expression parser that handles the complete evaluation
  process including validation and undefined point detection.
  
  Parameters:
  expr - Null-terminated string containing the mathematical expression
         Must be a valid mathematical expression using supported operators
         and functions
  x    - Value to substitute for the variable 'x' in the expression
  
  Returns:
  EvaluationResult containing:
  - Computed value of the expression
  - Flag indicating if the result is mathematically defined
  
  Error Handling:
  - Returns {0, 0} for undefined results
  - Sets errno for various error conditions
  
  Thread Safety:
  - Function is reentrant and thread-safe
  - Each call uses its own Parser instance
 */
EvaluationResult evaluate_expression(const char *expr, double x);

/*
  Parses addition and subtraction operations in the expression
  Implements the highest level of operator precedence parsing,
  handling the lowest precedence operators (+ and -).
  
  Grammar Rule:
  expression = term {("+"|"-") term}
  
  Parameters:
  p - Pointer to Parser structure containing parsing context
      expr member must point to valid expression string
  
  Returns:
  double - Computed value of the expression
  
  Error Handling:
  - Exits with status 2 on syntax errors
  - Sets errno for mathematical errors
  
  Notes:
  - Left-associative evaluation
  - Calls parse_term() for higher precedence operations
*/
double parse_expression(Parser *p);

/*
  Parses multiplication and division operations
  Implements the second level of operator precedence parsing,
  handling the higher precedence operators (* and /).
  
  Grammar Rule:
  term = factor {("*"|"/") factor}
  
  Parameters:
  p - Pointer to Parser structure containing parsing context
      expr member must point to valid term
  
  Returns:
  double - Computed value of the term
  
  Error Handling:
  - Exits with status 2 on syntax errors
  - Sets errno for division by zero
  
  Notes:
  - Left-associative evaluation
  - Calls parse_factor() for higher precedence operations
  - Special handling for division by zero
*/
double parse_term(Parser *p);

/*
  Parses basic factors and exponentiation
  Implements the highest level of operator precedence parsing,
  handling numbers, variables, parenthesized expressions, and
  exponentiation operations.
  
  Grammar Rule:
  factor = number | "x" | function "(" expression ")" | 
           "(" expression ")" | "-" factor | factor "^" factor
  
  Parameters:
  p - Pointer to Parser structure containing parsing context
      expr member must point to valid factor
  
  Returns:
  double - Computed value of the factor
  
  Error Handling:
  - Exits with status 2 on syntax errors
  - Sets errno for domain errors
  
  Notes:
  - Right-associative for exponentiation
  - Handles unary minus
  - Calls parse_number() for numeric literals
  - Calls parse_function() for function calls
*/
double parse_factor(Parser *p);

/*
  Parses and evaluates mathematical function calls
  Handles the parsing and computation of all supported mathematical
  functions listed in KNOWN_FUNCTIONS.
  
  Parameters:
  p - Pointer to Parser structure containing parsing context
      expr member must point to function name
  
  Returns:
  double - Computed value of the function
  
  Error Handling:
  - Exits with status 2 for unknown functions
  - Sets errno for domain errors
  - Sets errno for undefined results
  
  Supported Functions:
  - Trigonometric: sin, cos, tan
  - Inverse trigonometric: asin, acos, atan
  - Hyperbolic: sinh, cosh, tanh
  - Logarithmic: ln, log
  - Other: abs, exp
  
  Notes:
  - Validates function names against KNOWN_FUNCTIONS
  - Handles domain restrictions (e.g., asin range [-1,1])
  - Detects undefined points (e.g., tan(π/2))
*/
double parse_function(Parser *p);

/*
  Parses numeric literals in the expression
  Handles the conversion of string representations of numbers
  into double values, including scientific notation.
  
  Parameters:
  p - Pointer to Parser structure containing parsing context
      expr member must point to numeric literal
  
  Returns:
  double - Parsed numeric value
  
  Supported Formats:
  - Integer values: "123"
  - Decimal values: "123.456"
  - Scientific notation: "1.23e-4"
  - Leading decimal point: ".123"
  
  Error Handling:
  - Exits with status 2 for invalid number format
  - Sets errno for out of range values
  
  Notes:
  - Handles both positive and negative numbers
  - Validates number format
  - Detects overflow/underflow conditions
*/
double parse_number(Parser *p);

/*
  Utility function to skip whitespace characters in the expression
  Advances the expression pointer past any whitespace characters until
  a non-whitespace character is encountered or the end of string is reached.
  
  Parameters:
  p - Pointer to Parser structure containing the current parsing context
      The expr member will be updated to point to the next non-whitespace char
  
  Whitespace Characters:
  - Space
  - Tab
  - Newline
  - Carriage return
  - Form feed
  - Vertical tab
  
  Notes:
  - Uses isspace() for whitespace detection
  - Modifies the Parser's expr pointer directly
  - Safe with no whitespace (no-op in this case)
  - Safe at end of string
*/
void skip_whitespace(Parser *p);

/*
  Utility function to match and consume expected characters
  Verifies that the current character in the expression matches the expected
  character and advances the parser position.
  
  Parameters:
  p        - Pointer to Parser structure containing the current parsing context
  expected - The character that should appear at the current position
  
  Error Handling:
  - If the current character doesn't match the expected character:
    - Prints error message to stderr with context
    - Exits with status code 2
  - If end of expression is reached unexpectedly:
    - Prints appropriate error message
    - Exits with status code 2
  
  Notes:
  - Automatically skips leading whitespace before matching
  - Updates parser position on successful match
  - Used for enforcing expression syntax
  - Critical for maintaining parsing invariants
*/
void match(Parser *p, char expected);

/*
  Comprehensive syntax validation for mathematical expressions
  Performs a complete syntax check of the mathematical expression
  before actual parsing begins.
  
  Parameters:
  expr - Null-terminated string containing the mathematical expression
         to validate
  
  Returns:
  int - 1 if the expression is syntactically valid
        0 if any syntax errors are detected
  
  Validation Checks:
  - Balanced parentheses
  - Proper operator placement and sequence
  - Valid function names and usage
  - Correct number format
  - Variable 'x' usage
  - No consecutive operators (except unary minus)
  - No missing operators between operands
  
  Error Conditions:
  - Unbalanced parentheses
  - Invalid function names
  - Malformed numbers
  - Invalid operator sequences
  - Unknown symbols
  
  Notes:
  - Should be called before attempting to parse
  - Does not evaluate the expression
  - Performance is O(n) where n is expression length
  - Returns 0 for empty expressions
  - Safe with null input (returns 0)
*/
int validate_expression(const char *expr);

#endif /* PARSER_H */