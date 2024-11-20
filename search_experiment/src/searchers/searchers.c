#include "searchers.h"

/**
 * Klíč k úspěchu!
 *
 * ! `struct commodity comms[]` == `struct commodity *comms`
 * ! `comm_id_array_type id` == `char ids[MAX_ID_LENGTH]`
 * ! `comm_id_array_type ids[]` == `char(*ids)[MAX_ID_LENGTH]`
 * ! `comm_id_array_type ids[]` == `char(ids[])[MAX_ID_LENGTH]`
 */

/**
 * Funkce ke studiu:
 *
 * `qsort`      https://en.cppreference.com/w/c/algorithm/qsort
 * `bsearch`    https://en.cppreference.com/w/c/algorithm/bsearch
 * `strcmp`     https://en.cppreference.com/w/c/string/byte/strcmp
 */

/** INFO: pro rychlejší debugging snižte hodnotu TEST_RUNS v souboru `config.h`. */


int linear_search(const struct commodity comms[], const size_t comms_count,
                    const comm_id_array_type ids[], const size_t ids_count, int quantities[]) {
    size_t i, j;
    int found;

    if (!comms || comms_count == 0 || !ids || ids_count == 0 || !quantities) {
        return EXIT_FAILURE;
    }

    for (i = 0; i < ids_count; ++i) {
        found = 0;

        for (j = 0; j < comms_count; ++j) {
            if (strcmp(ids[i], comms[j].id) == 0) {
                quantities[i] = comms[j].quantity;
                found = 1;
                break;
            }
        }

        if (!found) {
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

int binary_search(const struct commodity comms[], const size_t comms_count, const comm_id_array_type ids[], const size_t ids_count, int quantities[]) {
    size_t i, left, right, mid;

    if (!comms || comms_count == 0 || !ids || ids_count == 0 || !quantities) {
        return EXIT_FAILURE;
    }

    for (i = 0; i < ids_count; ++i) {
        left = 0;
        right = comms_count - 1;
        
        while (left <= right) {
            mid = left + (right - left) / 2;
            int cmp = strcmp(ids[i], comms[mid].id);

            if (cmp == 0) {
                quantities[i] = comms[mid].quantity;
                break;
            } else if (cmp < 0) {
                if (mid == 0) break;
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        }

        if (left > right) {
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
