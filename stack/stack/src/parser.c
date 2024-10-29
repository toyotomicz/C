/**
 * @file parser.c
 * @author František Pártl (fpartl@ntis.zcu.cz)
 * @brief Zdrojový kód jednoduchého syntaktického analyzátoru aritmetických výrazů zapsaných v tzv. Reverzní polské
 * notaci (RPN), který využívá implementovaný zásobník.
 * @version 1.O
 * @date 2020-09-08
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "stack/stack.h"
#include "operators.h"

/** @brief Maximální velikost zásobníku a maximální délka vstupu. */
#define MAX_INPUT_LEN 256

/**
 * @brief Funkce slouží k vyhodnocení výrazu zapsaného v reverzní polské notaci (RPN).
 * @param input Zpracovávaný výraz.
 * @param result Ukazatel na paměť, kam bude zkopírován výsledek výrazu.
 * @return int 1, pokud zpracování proběhlo v pořádku, jinak 0.
 */
int evaluate_rpn_expression_1(const char *input, calc_num_type *result) {
    struct stack *s;
    size_t i, input_length;
    calc_num_type a, b, temp;
    calc_handler_type handler;

    if (!input || !result) {
        return 0;
    }
    
    input_length = strlen(input);
    if (input_length == 0) {
        return 0;
    }

    s = stack_alloc(input_length, sizeof(calc_num_type));
    if (!s) {
        return 0;
    }

    for (i = 0; i < input_length; ++i) {
        if (input[i] >= '0' && input[i] <= '9') {
            temp = input[i] - '0';
            stack_push(s, &temp);
        }
        else {  /* Když to není operand, tak to musí být operátor. */
            handler = get_operator_handler(input[i]);
            if (!handler) {
                stack_dealloc(&s);         /* Při každém průšvihu musím uvolnit zdroje. Co když někde zapomenu?! */
                return 0;
            }

            if (!stack_pop(s, &b) || !stack_pop(s, &a)) {
                stack_dealloc(&s);         /* Kód se začíná opakovat! */
                return 0;
            }

            temp = handler(a, b);
            stack_push(s, &temp);
        }
    }

    if (stack_item_count(s) != 1) {
        stack_dealloc(&s);                 /* Kód se opět opakuje! */
        return 0;
    }

    stack_pop(s, result);
    stack_dealloc(&s);
    return 1;
}

/**
 * @brief Funkce slouží k vyhodnocení výrazu zapsaného v reverzní polské notaci (RPN).
 * @param input Zpracovávaný výraz.
 * @param result Ukazatel na paměť, kam bude zkopírován výsledek výrazu.
 * @return int 1, pokud zpracování proběhlo v pořádku, jinak 0.
 */
int evaluate_rpn_expression_2(const char *input, calc_num_type *result) {
    size_t i, input_length;
    struct stack *s = NULL;
    calc_num_type a, b, temp;
    calc_handler_type handler;

    /** Velice zajímavým řešením je tvorba preprocesorového makra! Znakem '\' jsou uvozeny konce řádků makra. Pokud bude
     * expression == 0, tak uvolní alokovaný zásobník a ukončí funkci s návratovou hodnotou 0. Expression bude při
     * vyšívání preprocesoru nahrazen konkrétním výrazem, takže zde jsou závorky nezbytné!
     */
    #define EXIT_IF_NOT(expression)     \
        if (!(expression)) {            \
            stack_dealloc(&s);          \
            return 0;                   \
        }

    /* Makro je možné použít i u kontroly argumentů. */
    EXIT_IF_NOT(input && result);

    input_length = strlen(input);
    EXIT_IF_NOT(input_length != 0);

    /* Výraz můžu dle svého gusta dát přímo jako argument funkce, ale pozor na přehlednost. */
    s = stack_alloc(input_length, sizeof(calc_num_type));
    EXIT_IF_NOT(s);

    for (i = 0; i < input_length; ++i) {
        if (input[i] >= '0' && input[i] <= '9') {
            temp = input[i] - '0';
            stack_push(s, &temp);
        }
        else {  /* Asi nejčistší řešení, nemyslíte? */
            EXIT_IF_NOT(handler = get_operator_handler(input[i]));
            EXIT_IF_NOT(stack_pop(s, &b) && stack_pop(s, &a));

            temp = handler(a, b);
            stack_push(s, &temp);   /* Přidání kontroly návratové hodnoty funkce stack_push by při použití makra bylo celkem bezbolestné. */
        }
    }

    EXIT_IF_NOT(stack_item_count(s) == 1);

    stack_pop(s, result);
    stack_dealloc(&s);
    return 1;

    #undef EXIT_IF_NOT  /* Zneplatním makro, aby jej nebylo možné použít v dalších funkcích. */
}

/**
 * @brief Funkce slouží k vyhodnocení výrazu zapsaného v reverzní polské notaci (RPN).
 * @param input Zpracovávaný výraz.
 * @param result Ukazatel na paměť, kam bude zkopírován výsledek výrazu.
 * @return int 1, pokud zpracování proběhlo v pořádku, jinak 0.
 */
int evaluate_rpn_expression_3(const char *input, calc_num_type *result) {
    size_t i, input_length;
    calc_num_type a, b, temp;
    calc_handler_type handler;

    /**
     * Co když někdo bude chtít použít staticky alokovanou instanci struktury `stack`?
     * Není problém, implementace správně odděluje alokaci (`stack_alloc`) a inicializaci (`stack_init`).
     */
    struct stack s = DEFAULT_STACK;

    if (!input || !result) {
        return 0;
    }

    input_length = strlen(input);
    if (input_length == 0) {
        return 0;
    }

    if (!stack_init(&s, input_length, sizeof(calc_num_type))) {
        return 0;
    }

    for (i = 0; i < input_length; ++i) {
        if (input[i] >= '0' && input[i] <= '9') {
            temp = input[i] - '0';
            stack_push(&s, &temp);
        }
        else {
            handler = get_operator_handler(input[i]);
            if (!handler) {
                /* Toto je jedno z mála přijatelných použití příkazu goto. Z 99 % je ale jeho použití známkou špatného návrhu programu. */
                goto exit_on_error;
            }

            if (!stack_pop(&s, &b) || !stack_pop(&s, &a)) {
                /* Vyřešili jsme opakující se kód... alespoň částečně (zdroje se nám vždy správně uvolní). */
                goto exit_on_error;
            }

            temp = handler(a, b);
            stack_push(&s, &temp);
        }
    }

    if (stack_item_count(&s) == 1) {
        stack_pop(&s, result);
        stack_deinit(&s);
        return 1;
    }

  exit_on_error:     /* Vlastně takový catch blok. */
    stack_deinit(&s);
    return 0;
}

/**
 * @brief Hlavní přístupový bod aplikace pro zpracování postfixových výrazů.
 * @return int Funkce vždy vrací hodnotu `EXIT_SUCCESS`.
 */
int main() {
    char input[MAX_INPUT_LEN];
    calc_num_type result;

    printf("Enter \"quit\" to exit this amazing calculator.\n\n");

    for (;;) {
        printf("> ");
        fgets(input, MAX_INPUT_LEN, stdin);
        input[strcspn(input, "\r\n")] = '\000';

        if (strcmp(input, "quit") == 0) {
            break;
        }

        if (evaluate_rpn_expression_2(input, &result)) {
            printf("%d\n", result);
        }
        else printf("syntax error\n");
    }

    printf("You are leaving an awesome calculator. Be back soon!\n");
    return EXIT_SUCCESS;
}
