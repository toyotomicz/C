#include "parser.h"

/* Funkce pro vyhodnocení matematického výrazu */
double evaluateExpression(const char *expr, double x) {
    Parser parser = { expr, x };
    return parseExpression(&parser);
}

/* Funkce pro parsování výrazu */
double parseExpression(Parser *p) {
    double result = parseTerm(p);
    skipWhitespace(p);
    while (*p->expr == '+' || *p->expr == '-') {
        char op = *p->expr;
        match(p, op);
        skipWhitespace(p);
        if (op == '+') {
            result += parseTerm(p);
        } else {
            result -= parseTerm(p);
        }
        skipWhitespace(p);
    }
    return result;
}

/* Funkce pro parsování termu */
double parseTerm(Parser *p) {
    double result = parseFactor(p);
    skipWhitespace(p);
    while (*p->expr == '*' || *p->expr == '/') {
        char op = *p->expr;
        match(p, op);
        skipWhitespace(p);
        if (op == '*') {
            result *= parseFactor(p);
        } else {
            double divisor = parseFactor(p);
            if (divisor == 0) {
                fprintf(stderr, "Error: Division by zero\n");
                exit(2);
            }
            result /= divisor;
        }
        skipWhitespace(p);
    }
    return result;
}

/* Funkce pro parsování faktoru */
double parseFactor(Parser *p) {
    skipWhitespace(p);
    double result;

    if (*p->expr == '(') {
        match(p, '(');
        skipWhitespace(p);
        result = parseExpression(p);
        skipWhitespace(p);
        match(p, ')');
    } else if (*p->expr == '-') {
        match(p, '-');
        skipWhitespace(p);
        result = -parseFactor(p);
    } else if (isalpha(*p->expr)) {
        if (*p->expr == 'x') {
            match(p, 'x');
            result = p->x;
        } else {
            result = parseFunction(p);
        }
    } else {
        result = parseNumber(p);
    }

    skipWhitespace(p);
    if (*p->expr == '^') {
        match(p, '^');
        skipWhitespace(p);
        double exponent = parseFactor(p);
        result = pow(result, exponent);
    }

    return result;
}

/* Funkce pro parsování matematických funkcí */
double parseFunction(Parser *p) {
    char funcName[10] = {0};
    int i = 0;
    
    // Načtení jména funkce
    while (isalpha(p->expr[i]) && i < 9) {
        funcName[i] = p->expr[i];
        i++;
    }
    p->expr += i;
    
    skipWhitespace(p);
    match(p, '(');
    skipWhitespace(p);
    double arg = parseExpression(p);
    skipWhitespace(p);
    match(p, ')');

    // Goniometrické funkce
    if (strcmp(funcName, "sin") == 0) return sin(arg);
    if (strcmp(funcName, "cos") == 0) return cos(arg);
    if (strcmp(funcName, "tan") == 0) {
        if (cos(arg) == 0) {
            fprintf(stderr, "Error: Tangent undefined at cos(x) = 0\n");
            exit(2);
        }
        return tan(arg);
    }

    // Cyklometrické funkce
    if (strcmp(funcName, "asin") == 0) {
        if (arg < -1 || arg > 1) {
            fprintf(stderr, "Error: Arcsine defined only for values in [-1,1]\n");
            exit(2);
        }
        return asin(arg);
    }
    if (strcmp(funcName, "acos") == 0) {
        if (arg < -1 || arg > 1) {
            fprintf(stderr, "Error: Arccosine defined only for values in [-1,1]\n");
            exit(2);
        }
        return acos(arg);
    }
    if (strcmp(funcName, "atan") == 0) return atan(arg);

    // Hyperbolometrické funkce
    if (strcmp(funcName, "sinh") == 0) return sinh(arg);
    if (strcmp(funcName, "cosh") == 0) return cosh(arg);
    if (strcmp(funcName, "tanh") == 0) return tanh(arg);

    // Logaritmické funkce
    if (strcmp(funcName, "ln") == 0) {
        if (arg <= 0) {
            fprintf(stderr, "Error: Natural logarithm undefined for values <= 0\n");
            exit(2);
        }
        return log(arg);
    }
    if (strcmp(funcName, "log") == 0) {
        if (arg <= 0) {
            fprintf(stderr, "Error: Logarithm undefined for values <= 0\n");
            exit(2);
        }
        return log10(arg);
    }

    // Exponenciální funkce
    if (strcmp(funcName, "exp") == 0) return exp(arg);

    // Absolutní hodnota
    if (strcmp(funcName, "abs") == 0) return fabs(arg);

    fprintf(stderr, "Error: Unknown function: %s\n", funcName);
    exit(2);
}

/* Funkce pro parsování čísel */
double parseNumber(Parser *p) {
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
void skipWhitespace(Parser *p) {
    while (isspace(*p->expr)) {
        p->expr++;
    }
}

/* Funkce pro kontrolu očekávaného znaku */
void match(Parser *p, char expected) {
    skipWhitespace(p);
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

int validateExpression(const char *expr) {
    int parenCount = 0;
    const char *p = expr;
    int hasValidChar = 0;
    int hasX = 0;
    
    while (*p) {
        if (*p == '(') {
            parenCount++;
            hasValidChar = 1;
        } else if (*p == ')') {
            parenCount--;
            if (parenCount < 0) {
                fprintf(stderr, "Error: Unmatched closing parenthesis\n");
                return 0;
            }
            hasValidChar = 1;
        } else if (*p == 'x') {
            hasX = 1;
            hasValidChar = 1;
        } else if (isdigit(*p) || *p == '.') {
            hasValidChar = 1;
        } else if (*p == 'E' || *p == 'e') {
            if (p[1] == '+' || p[1] == '-') {
                if (!isdigit(p[2])) {
                    fprintf(stderr, "Error: Invalid scientific notation format\n");
                    return 0;
                }
                p += 2;
            } else if (!isdigit(p[1])) {
                fprintf(stderr, "Error: Invalid scientific notation format\n");
                return 0;
            }
            hasValidChar = 1;
        } else if (isalpha(*p)) {
            // Kontrola známých funkcí
            const char *funcStart = p;
            while (isalpha(*p)) p++;
            size_t funcLen = p - funcStart;
            p--; // Vrátíme se o jeden znak zpět, protože cyklus while inkrementuje p
            
            if (funcLen > 1 && *funcStart != 'x') {  // Ignorujeme 'x' jako proměnnou
                if (strncmp(funcStart, "sin", funcLen) != 0 &&
                    strncmp(funcStart, "cos", funcLen) != 0 &&
                    strncmp(funcStart, "tan", funcLen) != 0 &&
                    strncmp(funcStart, "asin", funcLen) != 0 &&
                    strncmp(funcStart, "acos", funcLen) != 0 &&
                    strncmp(funcStart, "atan", funcLen) != 0 &&
                    strncmp(funcStart, "sinh", funcLen) != 0 &&
                    strncmp(funcStart, "cosh", funcLen) != 0 &&
                    strncmp(funcStart, "tanh", funcLen) != 0 &&
                    strncmp(funcStart, "log", funcLen) != 0 &&
                    strncmp(funcStart, "ln", funcLen) != 0 &&
                    strncmp(funcStart, "exp", funcLen) != 0 &&
                    strncmp(funcStart, "abs", funcLen) != 0) {
                    fprintf(stderr, "Error: Unknown function '%.*s'\n", (int)funcLen, funcStart);
                    return 0;
                }
            }
        } else if (!strchr("+-*/^() ", *p)) {
            fprintf(stderr, "Error: Invalid character '%c'\n", *p);
            return 0;
        }
        p++;
    }
    
    if (parenCount > 0) {
        fprintf(stderr, "Error: Unmatched opening parenthesis\n");
        return 0;
    }
    if (!hasValidChar || !hasX) {
        fprintf(stderr, "Error: Expression must contain variable 'x' and valid mathematical operations\n");
        return 0;
    }
    
    return 1;
}

int main(int argc, char *argv[]) {
    // Kontrola počtu parametrů
    if (argc < 2) {
        fprintf(stderr, "Error: Program requires exactly one parameter - the mathematical expression.\n");
        fprintf(stderr, "Usage: %s \"expression\"\n", argv[0]);
        return 1;  // Chybné parametry na příkazové řádce
    }

    const char *expr = argv[1];

    // Kontrola syntaxe výrazu
    if (!validateExpression(expr)) {
        fprintf(stderr, "Error: Invalid mathematical expression. Expression must contain:\n");
        fprintf(stderr, "- Exactly one variable 'x'\n");
        fprintf(stderr, "- Valid operators (+, -, *, /, ^)\n");
        fprintf(stderr, "- Valid numbers (including scientific notation)\n");
        fprintf(stderr, "- Valid mathematical functions (sin, cos, tan, etc.)\n");
        return 2;  // Neakceptovatelný zápis matematické funkce
    }
    
    // Pro korektní výraz vypsání výsledků bez hlavičky
    for (int x = -5; x <= 5; x++) {
        double result = evaluateExpression(expr, x);
        printf("%2d\t%.10g\n", x, result);
    }

    return 0;  // Korektní ukončení
}