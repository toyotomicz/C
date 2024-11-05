#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

#define STACK_MAX 100

// Struktura pro zásobník
typedef struct {
    int data[STACK_MAX];
    int top;
} Stack;

// Funkce pro inicializaci zásobníku
void stack_init(Stack *s);

// Funkce pro kontrolu, zda je zásobník prázdný
bool stack_is_empty(const Stack *s);

// Funkce pro přidání prvku na vrchol zásobníku
bool stack_push(Stack *s, int value);

// Funkce pro odstranění prvku z vrcholu zásobníku
bool stack_pop(Stack *s, int *value);

// Funkce pro nahlédnutí na vrchol zásobníku (bez odstranění)
bool stack_peek(const Stack *s, int *value);

#endif // STACK_H
