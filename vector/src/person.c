#include "person.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct person *person_allocate(const char *name, const int age, const int shoe_size) {
    struct person *new;

    if (!name || strlen(name) == 0) {
        return NULL;
    }

    new = malloc(sizeof(struct person));
    if (!new) {
        return NULL;
    }

    if (!person_init(new, name, age, shoe_size)) {
        return NULL;
    }

    return new;
}

int person_init(struct person *p, const char *name, const int age, const int shoe_size) {
    if (!p || !name || strlen(name) == 0) {
        return 0;
    }

    p->name = malloc(strlen(name) + 1);
    if (!p->name) {
        return 0;
    }
    strcpy(p->name, name);

    p->age = age;
    p->shoe_size = shoe_size;

    return 1;
}

void person_deinit(struct person *poor) {
    if (!poor) {
        return;
    }

    free(poor->name); /* Opět otázka k zamyšlení: vynulovali jsme tímto pamět?! */
    poor->name = NULL;

    poor->age = 0;
    poor->shoe_size = 0;
}

void person_deallocate(struct person **poor) {
    if (!poor || !*poor) {
        return;
    }

    person_deinit(*poor);

    free(*poor);
    *poor = NULL;
}

void person_print(const struct person *p) {
    if (p) {
        printf("(\"%s\", %d, %d)\n", p->name, p->age, p->shoe_size);
    }
    else {
        printf("(undefined)\n");
    }
}
