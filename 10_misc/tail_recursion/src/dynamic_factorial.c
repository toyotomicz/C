#include <stdlib.h>
#include <stdio.h>


// TODO: zkuste si smazat specifikátory `static` u následujících funkcí. Jak se zachová kompilátor?

/** \brief Stará známá rekurzivní implementace faktoriálu (jen pro akademické účely)! */
static size_t _factorial_classic(const size_t n) {
    if (n < 2) {
        return 1;
    }

    // Tento výraz mi zatím GCC optimalizovat neumí. Snad s novou verzí...
    return n * _factorial_classic(n - 1);
}

/** \brief Rekurzivní implementace faktoriálu s možnou tail call optimalizací. */
static size_t _factorial_tail_acc(const size_t acc, const size_t n) {
    if (n < 2) {
        return acc;
    }

    return _factorial_tail_acc(n * acc, n - 1);
}

/** \brief Pouze wrapper funkce pro snazší použití funkce `_factorial_tail_acc`. */
static size_t _factorial_tail(const size_t n) {
    return _factorial_tail_acc(1, n);
}

/**
 * \brief Funkce načte argument `n` z konzolového rozhraní (výchozí hodnota 0) a vypíše hodnotu výrazu `n!` na
 * standardní výstup.
 * 
 * \param argc Počet argumentů programu.
 * \param argv Argumenty programu.
 * \return int `EXIT_SUCCESS` při úspěchu nebo `EXIT_FAILURE` při neplatném vstupu.
 */
int main(int argc, char *argv[]) {
    size_t n = 0;

    // načtení `n` z argumentů programu
    if (argc >= 2) {
        if (sscanf(argv[1], "%zu", &n) != 1) {
            return EXIT_FAILURE;
        }
    }

    // Pro demonstraci zavoláme obě funkce...
    printf("%zu! = %zu\n", n, _factorial_classic(n));
    printf("%zu! = %zu\n", n, _factorial_tail(n));

    return EXIT_SUCCESS;
}
