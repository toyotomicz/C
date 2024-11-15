#ifndef STACK_H
#define STACK_H

#include <stddef.h>

/**
 * Represents a stack data structure.
 */
struct stack {
    size_t capacity; /* Maximum number of items the stack can hold */
    size_t item_size; /* Size of each item in bytes */
    size_t sp; /* Stack pointer (index of the top item) */
    void *items; /* Array of items */
};

/**
 * Allocates and initializes a new stack.
 *
 * @param capacity The maximum number of items the stack can hold.
 * @param item_size The size of each item in bytes.
 *
 * @return A pointer to the newly allocated stack on success, NULL on failure.
 */
struct stack *stack_alloc(const size_t capacity, const size_t item_size);

/**
 * Initializes an existing stack.
 *
 * @param s A pointer to the stack to initialize.
 * @param capacity The maximum number of items the stack can hold.
 * @param item_size The size of each item in bytes.
 *
 * @return 1 on success, 0 on failure.
 */
int stack_init(struct stack *s, const size_t capacity, const size_t item_size);

/**
 * Deinitializes a stack.
 *
 * @param s A pointer to the stack to deinitialize.
 */
void stack_deinit(struct stack *s);

/**
 * Deallocates a stack.
 *
 * @param s A pointer to the pointer to the stack to deallocate.
 */
void stack_dealloc(struct stack **s);

/**
 * Pushes an item onto the stack.
 *
 * @param s A pointer to the stack.
 * @param item A pointer to the item to push.
 *
 * @return 1 on success, 0 on failure.
 */
int stack_push(struct stack *s, const void *item);

/**
 * Pops an item from the stack.
 *
 * @param s A pointer to the stack.
 * @param item A pointer to where the popped item will be stored.
 *
 * @return 1 on success, 0 on failure.
 */
int stack_pop(struct stack *s, void *item);

/**
 * Gets the top item from the stack without removing it.
 *
 * @param s A pointer to the stack.
 * @param item A pointer to where the top item will be stored.
 *
 * @return 1 on success, 0 on failure.
 */
int stack_head(const struct stack *s, void *item);

/**
 * Gets the number of items in the stack.
 *
 * @param s A pointer to the stack.
 *
 * @return The number of items in the stack.
 */
size_t stack_item_count(const struct stack *s);

#endif /* STACK_H */