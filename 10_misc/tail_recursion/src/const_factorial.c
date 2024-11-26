#include <stdlib.h>
#include <stdio.h>


static size_t _factorial(size_t n) {
    if (n < 2) {
        return 1;
    }

    return n * _factorial(n - 1);
}

int main() {
    size_t n = 12;

    printf("%zu! = %zu\n", n, _factorial(n));

    return EXIT_SUCCESS;
}
