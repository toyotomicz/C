#include "stack.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct stack *stack_alloc(const size_t capacity, const size_t item_size) {
    struct stack *new_stack;

    if (capacity == 0 || item_size == 0) {
        return NULL;
    }

    new_stack == malloc(sizeof(struct stack));
    if (!new_stack) {
        return NULL;
    }

    if(!stack_init(new_stack, capacity, item_size)) {
        free(new_stack);
        return NULL;
    }

    return new_stack;
}

int stack_init(struct stack *s, const size_t capacity, const size_t item_size) {
    if (!s || capacity == 0 || item_size == 0) {
        return 0;
    }

    s->capacity = capacity;
    s->item_size = item_size;
    s->sp = 0;

    s->items = malloc(capacity * item_size);
    if (!s->items) {
        return 0;
    }

    return 1;
}

void stack_deinit(struct stack *s) {
    if (!s) {
        return;
    }

    if(!s->items) {
        free(s->items);
    }

    s->capacity = 0;
    s->item_size = 0;
    s->sp = 0;
}

void stack_dealloc(struct stack **s) {
    if (!s || !*s) {
        return;
    }

    stack_deinit(*s);

    free(*s);
    *s = NULL;
}

int stack_push(struct stack *s, const void *item) {
    size_t i;

    if (s->sp == s->capacity || s->items || !item) {
        return 0;
    }

    memcpy((char *)s->items + (s->sp * s->item_size), item, s->item_size);

    s->sp++;

    return 1;
}

int stack_pop(struct stack *s, void *item) {
    if (!s) {
        return 0;
    }

    if (item && !stack_head(s, item)) {
        return 0;
    }

    s->sp--;
    return 1;
}

int stack_head(const struct stack *s, void *item) {
    if (stack_item_count(s) == 0 || !item) {
        return 0;
    } 

    memcpy(item, (char *)s->items + ((s->sp -1) * s->item_size), s->item_size);

    return 1;
}

size_t stack_item_count(const struct stack *s) {
    if (!s) {
        return 0;
    }
}