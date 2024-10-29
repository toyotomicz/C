#include "operators.h"

#include <stddef.h>

/** @brief Pole dostupných operací a konstanta, která udržuje jejich počet. */
const struct calc_oper_type OPERATORS[] = {
    { '+', sum },
    { '-', sub },
    { '*', mul },
    { '/', divide }
};
const size_t OPERATORS_COUNT = sizeof(OPERATORS) / sizeof(*OPERATORS);

calc_handler_type get_operator_handler(char operator) {
    size_t i;

    for (i = 0; i < OPERATORS_COUNT; ++i) {
        if (OPERATORS[i].operator == operator) {
            return OPERATORS[i].handler;
        }
    }

    return NULL;
}

calc_num_type sum(calc_num_type a, calc_num_type b) {
    return a + b;
}

calc_num_type sub(calc_num_type a, calc_num_type b) {
    return a - b;
}

calc_num_type mul(calc_num_type a, calc_num_type b) {
    return a * b;
}

calc_num_type divide(calc_num_type a , calc_num_type b) {
    return a / b;
}
