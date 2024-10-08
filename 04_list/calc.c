/**
 * \file calc.c
 * \author FrantiĹˇek PĂˇrtl (fpartl@ntis.zcu.cz)
 * \brief ZdrojovĂ˝ kĂłd primitivnĂ­ kalkulaÄŤky, kterĂˇ se zcela spolĂ©hĂˇ na bezchybnost uĹľivatele.
 * \version 1.0
 * \date 2020-09-02
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* MaximĂˇlnĂ­ dĂ©lka vstupu od uĹľivatele. */
#define MAX_INPUT_LENGTH 64

/**
 * \brief Funkce vrĂˇtĂ­ souÄŤet zadanĂ˝ch parametrĹŻ.
 * \param a SÄŤĂ­tanec.
 * \param b SÄŤĂ­tanec.
 * \return float SouÄŤet.
 */
float sum(float a, float b) {
    return a + b;
}

/**
 * \brief Funkce vrĂˇtĂ­ rozdĂ­l zadanĂ˝ch parametrĹŻ.
 * \param a MenĹˇenec.
 * \param b MenĹˇitel.
 * \return float RozdĂ­l.
 */
float sub(float a, float b) {
    return a - b;
}

/**
 * \brief Funkce vrĂˇtĂ­ souÄŤin zadanĂ˝ch parametrĹŻ. 
 * \param a ÄŚinitel.
 * \param b ÄŚinitel.
 * \return float SouÄŤin.
 */
float mul(float a, float b) {
    return a * b;
}

/**
 * \brief Funkce vrĂˇtĂ­ podĂ­l zadanĂ˝ch parametrĹŻ.
 * \param a DÄ›lenec.
 * \param b DÄ›litel.
 * \return float PodĂ­l.
 */
float divide(float a , float b) {
    return a / b;
}

typedef (*calc_func_type)(float, float);

/**
 * \brief PĹ™Ă­stupovĂ˝ bod aplikace. RozhranĂ­m aplikace je primitivnĂ­ konzolovĂ© rozhranĂ­, kterĂ© oÄŤekĂˇvĂˇ 
 *        aritmetickĂ© vĂ˝razy v infixovĂ© formÄ› bez mezer.
 * \return int Funkce vracĂ­ vĹľdy `EXIT_SUCCESS`.
 */
int main() {
    calc_func_type handler;
    char op, input[MAX_INPUT_LENGTH];
    float a, b;

    printf("Enter \"quit\" to exit this amazing calculator.\n");
    while (1) { /* vtĂ­pek: #define forever while(1), MISRA compliant verze for(;;). */
        printf("> ");
        fgets(input, MAX_INPUT_LENGTH, stdin);      /* MusĂ­m sice udat proud, ale je moĹľnĂ© kontrolovat dĂ©lku vstupnĂ­ho Ĺ™etÄ›zce! VidĂ­te zde praktiÄŤnost tĂ©to funkce? */

        /* MoĹľnĂˇ trochu neintuitivnĂ­ porovnĂˇvĂˇnĂ­. Zkontrolujte dokumentaci. */
        if (strncmp(input, "quit\n", MAX_INPUT_LENGTH) == 0) {
            break;
        }

        /* Bez pĹ™edĂˇnĂ­ ukazatele to prostÄ› nemĹŻĹľe jĂ­t. C pĹ™ece pĹ™edĂˇvĂˇ parametry hodnotou... */
        sscanf(input, "%f%c%f", &a, &op, &b);   /* Zde neoĹˇetĹ™ujeme vstupy! Zkontrolujte dokumentaci funkce sscanf a opravte potenciĂˇlnÄ› nebezpeÄŤnĂ˝ kĂłd! */

        /* PĹ™i pouĹľitĂ­ ukazatelĹŻ na funkce je od switche jenom malĂ˝ krĹŻÄŤek k poli struktur (operĂˇtor, handler) a for cyklu v novĂ© funkci `calc_func_type get_operator_handler(char op);`. */
        /* OpakujĂ­cĂ­ kĂłd tĂ­m pĂˇdem ĂşplnÄ› zmizĂ­ a moĹľnĂ© operĂˇtory (a jejich obsluĹľnĂ© funkce) mĹŻĹľeme pĹ™idĂˇvat a odebĂ­rat jednoduchou Ăşpravou statickĂ©ho pole (konfiguraÄŤnĂ­ho souboru). */
        /* Zkuste si tento program v tomto duchu upravit. Funkce `main` pak bude dÄ›lat jenom jednu vÄ›c (Single Responsibility Principle)! */
        switch (op) {
            case '+' : handler = sum;     break;        /* Zde bĂ˝t & nemusĂ­, protoĹľe bez () se nejednĂˇ o volĂˇnĂ­ funkce, coĹľ kompilĂˇtor vĂ­. */
            case '-' : handler = sub;     break;
            case '*' : handler = mul;     break;
            case '/' : handler = divide;  break;
            default  : handler = NULL;    break;
        }

        if (handler) {
            printf("%f %c %f = %f\n", a, op, b, handler(a, b));
        }
        else printf("Unsupported operator %c!\n", op);
    }

    printf("You are leaving an awesome calculator. Be back soon!\n");
    return EXIT_SUCCESS;
}