#include "stack.h"
#include <stdlib.h>
#include <string.h>

/**
 * Allocates and initializes a new stack.
 *
 * @param capacity The maximum number of items the stack can hold.
 * @param item_size The size of each item in bytes.
 *
 * @return A pointer to the newly allocated stack on success, NULL on failure.
 */
struct stack *stack_alloc(const size_t capacity, const size_t item_size) {
    struct stack *new_stack = malloc(sizeof(struct stack));
    if (!new_stack) {
        return NULL;
    }

    if (!stack_init(new_stack, capacity, item_size)) {
        free(new_stack);
        return NULL;
    }

    return new_stack;
}

/**
 * Initializes an existing stack.
 *
 * @param s A pointer to the stack to initialize.
 * @param capacity The maximum number of items the stack can hold.
 * @param item_size The size of each item in bytes.
 *
 * @return 1 on success, 0 on failure.
 */
int stack_init(struct stack *s, const size_t capacity, const size_t item_size) {
    if (capacity == 0 || item_size == 0) {
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

/**
 * Deinitializes a stack.
 *
 * @param s A pointer to the stack to deinitialize.
 */
void stack_deinit(struct stack *s) {
    if (!s) {
        return;
    }

    free(s->items);
    s->capacity = 0;
    s->item_size = 0;
    s->sp = 0;
}

/**
 * Deallocates a stack.
 *
 * @param s A pointer to the pointer to the stack to deallocate.
 */
void stack_dealloc(struct stack **s) {
    if (!s || !*s) {
        return;
    }

    stack_deinit(*s);
    free(*s);
    *s = NULL;
}

/**
 * Pushes an item onto the stack.
 *
 * @param s A pointer to the stack.
 * @param item A pointer to the item to push.
 *
 * @return 1 on success, 0 on failure.
 */
int stack_push(struct stack *s, const void *item) {
    if (s->sp == s->capacity) {
        return 0; /* Stack is full */
    }

    memcpy((char *)s->items + (s->sp * s->item_size), item, s->item_size);
    s->sp++;

    return 1;
}

/**
 * Pops an item from the stack.
 *
 * @param s A pointer to the stack.
 * @param item A pointer to where the popped item will be stored.
 *
 * @return 1 on success, 0 on failure.
 */
int stack_pop(struct stack *s, void *item) {
    if (s->sp == 0) {
        return 0; /* Stack is empty */
    }

    if (item) {
        memcpy(item, (char *)s->items + ((s->sp - 1) * s->item_size), s->item_size);
    }

    s->sp--;
    return 1;
}

/**
 * Gets the top item from the stack without removing it.
 *
 * @param s A pointer to the stack.
 * @param item A pointer to where the top item will be stored.
 *
 * @return 1 on success, 0 on failure.
 */
int stack_head(const struct stack *s, void *item) {
    if (s->sp == 0) {
        return 0; /* Stack is empty */
    }

    memcpy(item, (char *)s->items + ((s->sp - 1) * s->item_size), s->item_size);
    return 1;
}

/**
 * Gets the number of items in the stack.
 *
 * @param s A pointer to the stack.
 *
 * @return The number of items in the stack.
 */
size_t stack_item_count(const struct stack *s) {
    if (!s) {
        return 0;
    }

    return s->sp;
}