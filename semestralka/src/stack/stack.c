/*
____________________________________________________________________________ 
    Mathematical Expression Parser
    Version 1.0
    Module stack.c

    A type-safe, generic stack data structure implementation supporting
    arbitrary data types through void pointers and item size specification.
    
    Key Features:
    - Dynamic memory allocation
    - Type-safe operations
    - Configurable capacity
    - Complete memory management
    
    Implementation Details:
    - Uses void pointers for type genericity
    - Zero-initialization of memory
    
    Usage Requirements:
    - Caller must manage memory alignment
    - Item size must remain constant
    - Stack capacity is fixed after initialization

    Dialect: ANSI C
    Compiler: Any ANSI C-compatible compiler

    Copyright (c) Jiří Joska, 2024
    Provided "AS IS" with NO WARRANTY OF ANY KIND
____________________________________________________________________________ 
*/

#include "stack.h"

/* ____________________________________________________________________________
    Function: stack_alloc
    
    Implementation Notes:
    - Allocates both structure and internal buffer
    - Validates all parameters before allocation
    - Handles failure gracefully with cleanup
____________________________________________________________________________ */
struct stack *stack_alloc(const size_t capacity, const size_t item_size) {
    /* Allocate the stack structure */
    struct stack *new_stack = malloc(sizeof(struct stack));
    if (!new_stack) {
        return NULL;
    }
    /* Initialize the stack, clean up and return NULL on failure */
    if (stack_init(new_stack, capacity, item_size) == 0) {
        free(new_stack);
        return NULL;
    }

    return new_stack;
}

/* ____________________________________________________________________________
    Function: stack_init
    
    Implementation Notes:
    - Validates input parameters
    - Initializes all structure fields
    - Allocates internal buffer
____________________________________________________________________________ */
int stack_init(struct stack *s, const size_t capacity, const size_t item_size) {
    if (capacity == 0 || item_size == 0) {
        return 0;
    }

    s->capacity = capacity;
    s->item_size = item_size;
    s->sp = 0;
    /* Allocate the item buffer */
    s->items = malloc(capacity * item_size);
    if (!s->items) {
        return 0;
    }

    return 1;
}

/* ____________________________________________________________________________
    Function: stack_deinit
    
    Implementation Notes:
    - Safely handles NULL pointer
    - Resets all structure fields
    - Releases internal buffer
____________________________________________________________________________ */
void stack_deinit(struct stack *s) {
    if (!s) {
        return;
    }
    /* Free the item buffer and reset stack properties */
    free(s->items);
    s->capacity = 0;
    s->item_size = 0;
    s->sp = 0;
    s->items = NULL;
}

/* ____________________________________________________________________________
    Function: stack_dealloc
    
    Implementation Notes:
    - Complete cleanup of all resources
    - Safe with NULL pointers
    - Prevents memory leaks
____________________________________________________________________________ */
void stack_dealloc(struct stack **s) {
    if (!s || !*s) {
        return;
    }
    /* Clean up the stack's resources and free the structure */
    stack_deinit(*s);
    free(*s);
    *s = NULL;
}

/* ____________________________________________________________________________
    Function: stack_push
    
    Implementation Notes:
    - Bounds checking
    - Memory copy using item_size
    - Stack pointer management
____________________________________________________________________________ */
int stack_push(struct stack *s, const void *item) {
    /* Check if the stack is valid and not full */
    if (!s || !item || s->sp == s->capacity) {
        return 0; /* Failure: Invalid input or stack is full */
    }

    /* Temporary buffer to avoid aliasing */
    char temp[s->item_size];

    /* Copy the item into the temporary buffer */
    memcpy(temp, item, s->item_size);

    /* Copy the temporary buffer into the stack's storage at the correct position */
    memcpy((char *)s->items + (s->sp * s->item_size), temp, s->item_size);

    /* Increment the stack pointer (sp) to reflect the new stack state */
    s->sp++;

    return 1; /* Success */
}

/* ____________________________________________________________________________
    Function: stack_pop
    
    Implementation Notes:
    - Bounds checking
    - Optional item retrieval
    - Stack pointer management
____________________________________________________________________________ */
int stack_pop(struct stack *s, void *item) {
    if (s->sp == 0) {
        return 0; /* Stack is empty */
    }
    /* If a buffer was provided, copy the top item */
    if (item) {
        memcpy(item, (char *)s->items + ((s->sp - 1) * s->item_size), s->item_size);
    }

    s->sp--;
    return 1;
}

/* ____________________________________________________________________________
    Function: stack_head
    
    Implementation Notes:
    - Bounds checking
    - Memory copy without stack modification
    - Constant time operation
____________________________________________________________________________ */
int stack_head(const struct stack *s, void *item) {
    if (s->sp == 0) {
        return 0; /* Stack is empty */
    }
    /* Copy the top item without modifying the stack */
    memcpy(item, (char *)s->items + ((s->sp - 1) * s->item_size), s->item_size);
    return 1;
}

/* ____________________________________________________________________________
    Function: stack_item_count
    
    Implementation Notes:
    - NULL pointer safe
    - Direct stack pointer access
    - No side effects
____________________________________________________________________________ */
size_t stack_item_count(const struct stack *s) {
    if (!s) {
        return 0;
    }

    return s->sp;
}