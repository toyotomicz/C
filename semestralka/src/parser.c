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

/* Funkce pro validaci matematického výrazu */
int validateExpression(const char *expr) {
    printf("Validating expression: %s\n", expr);
    int paren_depth = 0;    // Hloubka závorek
    int last_was_operator = 1;  // Příznak pro kontrolu po sobě jdoucích operátorů
    int last_was_function = 0;  // Příznak pro kontrolu funkce
    int last_was_number = 0;    // Příznak pro kontrolu čísla
    const char *known_functions[] = {
        "abs", "exp", "ln", "log", 
        "sin", "cos", "tan", 
        "asin", "acos", "atan", 
        "sinh", "cosh", "tanh"
    };
    const int num_known_functions = sizeof(known_functions) / sizeof(known_functions[0]);

    // Průchod výrazem znak po znaku
    for (; *expr != '\0'; expr++) {
        // Přeskočení bílých znaků
        if (isspace(*expr)) continue;

        // Kontrola proměnné x
        if (*expr == 'x') {
            last_was_operator = 0;
            last_was_function = 0;
            last_was_number = 0;
            continue;
        }

        // Kontrola závorek
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

        // Kontrola operátorů
        if (*expr == '+' || *expr == '-' || *expr == '*' || *expr == '/' || *expr == '^') {
            // Unární mínus je povolen
            if (*expr == '-' && ( *(expr-1) == '(' || *(expr-1) == '+' || 
                                  *(expr-1) == '-' || *(expr-1) == '*' || 
                                  *(expr-1) == '/' || *(expr-1) == '^')) {
                last_was_operator = 1;
                last_was_function = 0;
                last_was_number = 0;
                continue;
            }

            // Kontrola po sobě jdoucích operátorů (s výjimkou unárního mínus)
            if (last_was_operator) return 0;
            last_was_operator = 1;
            last_was_function = 0;
            last_was_number = 0;
            continue;
        }

        // Kontrola funkcí
        if (isalpha(*expr)) {
            char function_name[10] = {0};
            int func_len = 0;

            // Načtení názvu funkce
            while (isalpha(expr[func_len]) && func_len < 9) {
                function_name[func_len] = expr[func_len];
                func_len++;
            }

            // Kontrola, zda je funkce v seznamu povolených funkcí
            int is_valid_function = 0;
            for (int i = 0; i < num_known_functions; i++) {
                if (strcmp(function_name, known_functions[i]) == 0) {
                    is_valid_function = 1;
                    break;
                }
            }

            if (!is_valid_function) return 0;

            // Kontrola, zda následuje otevírací závorka
            expr += func_len - 1;
            if (*(expr + 1) != '(') return 0;

            last_was_operator = 0;
            last_was_function = 1;
            last_was_number = 0;
            continue;
        }

        // Kontrola čísel (včetně vědeckého zápisu)
        if (isdigit(*expr) || *expr == '.') {
            char *endptr;
            strtod(expr, &endptr);
            
            // Pokud není platné číslo, vrátí 0
            if (expr == endptr) return 0;

            // Přesun ukazatele na konec čísla
            expr = endptr - 1;

            // Kontrola opakovaných čísel bez operátoru
            if (last_was_number) return 0;
            last_was_operator = 0;
            last_was_function = 0;
            last_was_number = 1;
            continue;
        }

        // Pokud znak neodpovídá žádné povolené kategorii, vrátí 0
        return 0;
    }

    // Závěrečné kontroly
    // Všechny závorky musí být uzavřené
    if (paren_depth != 0) return 0;
    // Nesmí končit operátorem
    if (last_was_operator) return 0;

    return 1;
}