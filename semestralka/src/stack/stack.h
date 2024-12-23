/* ____________________________________________________________________________
    Mathematical Expression Parser
    Version 1.0
    Header stack.h
    
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
____________________________________________________________________________ */

#ifndef STACK_H
#define STACK_H

#include <stddef.h>  /* Definitions for size_t and NULL */
#include <stdlib.h>  /* General utilities and memory management */
#include <string.h>  /* String manipulation functions */

/* 
    Stack Control Structure
    Contains all necessary information to manage the stack
*/
struct stack {
    size_t capacity;   /* Maximum number of items the stack can hold */
    size_t item_size;  /* Size of each item in bytes */
    size_t sp;         /* Stack pointer (index of the top item) */
    void *items;       /* Array of items */
};

/* ____________________________________________________________________________
    Function: stack_alloc
    
    Allocates and initializes a new stack instance with specified capacity
    and item size. Handles all necessary memory allocation.
    
    Parameters:
    - capacity:  Maximum number of items the stack can hold
    - item_size: Size of each item in bytes
    
    Returns:
    - Success: Pointer to initialized stack
    - Failure: NULL
    
    Memory Management:
    - Caller is responsible for deallocation via stack_dealloc
____________________________________________________________________________ */
struct stack *stack_alloc(const size_t capacity, const size_t item_size);

/* ____________________________________________________________________________
    Function: stack_init
    
    Initializes an existing stack structure with specified parameters.
    Assumes the stack structure itself is already allocated.
    
    Parameters:
    - s:         Pointer to pre-allocated stack structure
    - capacity:  Maximum number of items the stack can hold
    - item_size: Size of each item in bytes
    
    Returns:
    - Success: 1
    - Failure: 0
    
    Memory Management:
    - Caller must ensure stack_deinit is called when done
____________________________________________________________________________ */
int stack_init(struct stack *s, const size_t capacity, const size_t item_size);

/* ____________________________________________________________________________
    Function: stack_deinit
    
    Releases all resources associated with a stack instance except
    the stack structure itself.
    
    Parameters:
    - s: Pointer to stack to deinitialize
    
    Memory Management:
    - Frees internal buffer
    - Does not free stack structure
    - Safe to call with NULL pointer
____________________________________________________________________________ */
void stack_deinit(struct stack *s);

/* ____________________________________________________________________________
    Function: stack_dealloc
    
    Completely deallocates a stack and all its resources, including
    the stack structure itself.
    
    Parameters:
    - s: Pointer to stack pointer to deallocate
    
    Memory Management:
    - Frees all associated memory
    - Sets pointer to NULL
    - Safe to call with NULL pointer
____________________________________________________________________________ */
void stack_dealloc(struct stack **s);

/* ____________________________________________________________________________
    Function: stack_push
    
    Pushes a new item onto the top of the stack.
    
    Parameters:
    - s:    Pointer to stack
    - item: Pointer to item to push
    
    Returns:
    - Success: 1
    - Failure: 0 (stack is full)
    
    Memory Safety:
    - Ensures item size matches initialization
    - Prevents buffer overflow
____________________________________________________________________________ */
int stack_push(struct stack *s, const void *item);

/* ____________________________________________________________________________
    Function: stack_pop
    
    Removes and optionally returns the top item from the stack.
    
    Parameters:
    - s:    Pointer to stack
    - item: Pointer to storage for popped item (can be NULL)
    
    Returns:
    - Success: 1
    - Failure: 0 (stack is empty)
    
    Memory Safety:
    - Safe with NULL item pointer
    - Maintains stack bounds
____________________________________________________________________________ */
int stack_pop(struct stack *s, void *item);

/* ____________________________________________________________________________
    Function: stack_head
    
    Retrieves the top item from the stack without removing it.
    
    Parameters:
    - s:    Pointer to stack
    - item: Pointer to storage for head item
    
    Returns:
    - Success: 1
    - Failure: 0 (stack is empty)
    
    Memory Safety:
    - Requires valid item pointer
    - Maintains stack integrity
____________________________________________________________________________ */
int stack_head(const struct stack *s, void *item);

/* ____________________________________________________________________________
    Function: stack_item_count
    
    Returns the current number of items in the stack.
    
    Parameters:
    - s: Pointer to stack
    
    Returns:
    - Current number of items
    - 0 if stack is NULL
    
    Thread Safety:
    - Atomic read operation
    - Safe for concurrent access
____________________________________________________________________________ */
size_t stack_item_count(const struct stack *s);

#endif /* STACK_H */