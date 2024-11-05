#include "stack.h"

// Inicializuje zásobník
void stack_init(Stack *s) {
    s->top = -1;
}

// Kontroluje, zda je zásobník prázdný
bool stack_is_empty(const Stack *s) {
    return s->top == -1;
}

// Přidá prvek na vrchol zásobníku
bool stack_push(Stack *s, int value) {
    if (s->top >= STACK_MAX - 1) {
        return false; // Zásobník je plný
    }
    s->data[++(s->top)] = value;
    return true;
}

// Odstraní prvek z vrcholu zásobníku
bool stack_pop(Stack *s, int *value) {
    if (stack_is_empty(s)) {
        return false; // Zásobník je prázdný
    }
    *value = s->data[(s->top)--];
    return true;
}

// Nahlédne na vrchol zásobníku (bez odstranění)
bool stack_peek(const Stack *s, int *value) {
    if (stack_is_empty(s)) {
        return false; // Zásobník je prázdný
    }
    *value = s->data[s->top];
    return true;
}
