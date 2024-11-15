#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

/* Struktura pro uchovávání parseru, obsahuje ukazatel na aktuální pozici výrazu a hodnotu proměnné x */
typedef struct {
    const char *expr; /* Aktuální pozice ve vstupním řetězci */
    double x;         /* Hodnota proměnné x */
} Parser;

/* Deklarace funkcí pro parsování jednotlivých částí matematického výrazu */
double parseExpression(Parser *p);
double parseTerm(Parser *p);
double parseFactor(Parser *p);
double parseNumber(Parser *p);
double parseFunction(Parser *p);
void skipWhitespace(Parser *p);
void match(Parser *p, char expected);

/* Funkce pro vyhodnocení matematického výrazu s proměnnou x */
double evaluateExpression(const char *expr, double x) {
    /* Inicializace parseru s výrazem a hodnotou proměnné x */
    Parser parser = { expr, x };
    /* Spuštění parsování výrazu */
    return parseExpression(&parser);
}

/* Funkce pro parsování výrazu (zpracovává sčítání a odčítání) */
double parseExpression(Parser *p) {
    /* Načtení prvního termu */
    double result = parseTerm(p);

    /* Zpracování všech operátorů + a - v aktuálním výrazu */
    while (*p->expr == '+' || *p->expr == '-') {
        if (*p->expr == '+') {
            match(p, '+');           /* Posun na další znak po '+' */
            result += parseTerm(p);  /* Přičtení dalšího termu */
        } else if (*p->expr == '-') {
            match(p, '-');           /* Posun na další znak po '-' */
            result -= parseTerm(p);  /* Odečtení dalšího termu */
        }
    }
    return result; /* Vrácení výsledku výrazu */
}

/* Funkce pro parsování termu (zpracovává násobení a dělení) */
double parseTerm(Parser *p) {
    /* Načtení prvního faktoru */
    double result = parseFactor(p);

    /* Zpracování všech operátorů * a / v aktuálním výrazu */
    while (*p->expr == '*' || *p->expr == '/') {
        if (*p->expr == '*') {
            match(p, '*');           /* Posun na další znak po '*' */
            result *= parseFactor(p);/* Násobení dalším faktorem */
        } else if (*p->expr == '/') {
            match(p, '/');           /* Posun na další znak po '/' */
            double divisor = parseFactor(p);
            if (divisor == 0) {      /* Kontrola dělení nulou */
                printf("Chyba: Dělení nulou\n");
                exit(1);
            }
            result /= divisor;       /* Dělení dalším faktorem */
        }
    }
    return result; /* Vrácení výsledku termu */
}

/* Funkce pro parsování faktoru (zpracovává umocnění a unární mínus) */
double parseFactor(Parser *p) {
    skipWhitespace(p); /* Přeskočení bílých znaků na začátku faktoru */

    double result;
    if (*p->expr == '-') {
        /* Pokud je první znak '-', zpracuje se jako unární mínus */
        match(p, '-');
        result = -parseFactor(p);
    } else if (isalpha(*p->expr) && *p->expr != 'x') {
        /* Pokud začíná identifikátor, pokusí se parsovat matematickou funkci */
        result = parseFunction(p);
    } else {
        /* Jinak se očekává číslo nebo závorka */
        result = parseNumber(p);
    }

    /* Zpracování operátoru umocnění '^' */
    if (*p->expr == '^') {
        match(p, '^');
        result = pow(result, parseFactor(p)); /* Výpočet mocniny */
    }
    return result; /* Vrácení výsledku faktoru */
}

/* Funkce pro parsování matematických funkcí (např. sin, cos, log) */
double parseFunction(Parser *p) {
    char funcName[10] = {0}; /* Dočasné pole pro název funkce */
    int i = 0;

    /* Načtení názvu funkce */
    while (isalpha(p->expr[i]) && i < 9) {
        funcName[i] = p->expr[i];
        i++;
    }
    p->expr += i; /* Posun pozice ve výrazu za název funkce */

    skipWhitespace(p); /* Přeskočení bílých znaků */
    match(p, '(');     /* Očekává se '(' */
    double arg = parseExpression(p); /* Zpracování argumentu funkce */
    match(p, ')');     /* Očekává se ')' */

    /* Porovnání názvu funkce a volání odpovídající implementace */
    if (strcmp(funcName, "sin") == 0) return sin(arg);
    if (strcmp(funcName, "cos") == 0) return cos(arg);
    if (strcmp(funcName, "log") == 0) {
        if (arg <= 0) {
            printf("Chyba: Logaritmus není definován pro hodnoty <= 0\n");
            exit(1);
        }
        return log10(arg);
    }
    printf("Neznámá funkce: %s\n", funcName); /* Chyba při neznámé funkci */
    exit(1);
}

/* Funkce pro parsování čísel a proměnné x */
double parseNumber(Parser *p) {
    skipWhitespace(p); /* Přeskočení bílých znaků */

    double result;
    if (*p->expr == '(') {
        /* Zpracování výrazu v závorkách */
        match(p, '(');
        result = parseExpression(p);
        match(p, ')');
    } else if (*p->expr == 'x') {
        /* Zpracování proměnné x */
        match(p, 'x');
        result = p->x;
    } else if (isdigit(*p->expr) || *p->expr == '.' || *p->expr == '+' || *p->expr == '-') {
        /* Zpracování číselné hodnoty */
        result = strtod(p->expr, (char **)&p->expr);
    } else {
        printf("Neplatný symbol: %c\n", *p->expr); /* Chyba při neplatném symbolu */
        exit(1);
    }
    skipWhitespace(p); /* Přeskočení bílých znaků po čísle */
    return result;
}

/* Funkce pro přeskočení bílých znaků */
void skipWhitespace(Parser *p) {
    while (isspace(*p->expr)) { /* Dokud je aktuální znak bílý znak */
        p->expr++;              /* Posun na další znak */
    }
}

/* Funkce pro kontrolu očekávaného znaku a posun na další */
void match(Parser *p, char expected) {
    if (*p->expr == expected) {
        p->expr++; /* Posun na další znak */
    } else {
        /* Chyba při neočekávaném znaku */
        printf("Očekáváno '%c', nalezeno '%c'\n", expected, *p->expr);
        exit(1);
    }
}

int main() {
    /* Pole testovacích výrazů */
    const char *tests[] = {
        "cos(cos(cos(cos(x))))" /* Příklad výrazu obsahujícího proměnnou x a trigonometrickou funkci */
    };

    /* Smyčka pro hodnoty x od -10 do 10 s krokem 0.1 */
    for (double x = -10.0; x <= 10.0; x += 0.1) {
        for (size_t i = 0; i < sizeof(tests) / sizeof(tests[0]); i++) {
            /* Vyhodnocení výrazu pro aktuální hodnotu x */
            double result = evaluateExpression(tests[i], x);
            printf("Výsledek výrazu '%s' pro x = %.2f je %.10f\n", tests[i], x, result);
        }
    }

    return 0; /* Ukončení programu */
}
