#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

typedef struct Node {
    char op;
    double value;
    struct Node *left, *right;
} Node;

Node* createNode(char op, double value) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->op = op;
    node->value = value;
    node->left = node->right = NULL;
    return node;
}

Node* createTree(char* postfix[], int *index) {
    if (*index < 0) return NULL;

    char* token = postfix[(*index)--];
    if (isdigit(token[0]) || token[0] == '.' || (token[0] == '-' && isdigit(token[1]))) {
        return createNode(0, atof(token));
    }

    Node* node = createNode(token[0], 0);
    node->right = createTree(postfix, index);
    if (token[0] != '-') { // Unární mínus nepotřebuje levý operand
        node->left = createTree(postfix, index);
    }
    return node;
}

double evaluate(Node* root) {
    if (root == NULL) return 0;
    if (root->op == 0) return root->value;

    double left = evaluate(root->left);
    double right = evaluate(root->right);

    switch (root->op) {
        case '+': return left + right;
        case '-': return (root->left) ? left - right : -right; // Rozlišení unárního mínusu
        case '*': return left * right;
        case '/': return left / right;
        case '^': return pow(left, right);
    }
    return 0;
}

int precedence(char op) {
    switch (op) {
        case '+': case '-': return 1;
        case '*': case '/': return 2;
        case '^': return 3; // Nejvyšší priorita pro umocnění
        default: return 0;
    }
}

int isRightAssociative(char op) {
    return op == '^';
}

void shuntingYard(const char* expression, char* output[]) {
    char stack[100][100];
    int top = -1, outIndex = 0;
    int i = 0;

    while (expression[i]) {
        if (isspace(expression[i])) {
            i++;
            continue;
        }

        if (isdigit(expression[i]) || expression[i] == '.' ||
            (expression[i] == '-' && (i == 0 || expression[i - 1] == '(' || strchr("+-*/^", expression[i - 1])))) {
            char number[100];
            int numIndex = 0;
            if (expression[i] == '-') number[numIndex++] = expression[i++];

            while (isdigit(expression[i]) || expression[i] == '.' || expression[i] == 'E' || expression[i] == 'e' ||
                   expression[i] == '-' || expression[i] == '+') {
                number[numIndex++] = expression[i++];
                if (expression[i - 1] == 'E' || expression[i - 1] == 'e') {
                    if (expression[i] == '-' || expression[i] == '+') {
                        number[numIndex++] = expression[i++];
                    }
                }
            }
            number[numIndex] = '\0';
            output[outIndex++] = strdup(number);
            continue;
        }

        char token = expression[i];
        if (token == '(') {
            strcpy(stack[++top], "(");
        } else if (token == ')') {
            while (top >= 0 && stack[top][0] != '(') {
                output[outIndex++] = strdup(stack[top--]);
            }
            top--;
        } else if (strchr("+-*/^", token)) {
            while (top >= 0 && stack[top][0] != '(') {
                int precedenceTop = precedence(stack[top][0]);
                int precedenceToken = precedence(token);
                if ((precedenceToken < precedenceTop) || 
                    (precedenceToken == precedenceTop && !isRightAssociative(token))) {
                    output[outIndex++] = strdup(stack[top--]);
                } else {
                    break;
                }
            }
            stack[++top][0] = token;
            stack[top][1] = '\0';
        }
        i++;
    }

    while (top >= 0) {
        output[outIndex++] = strdup(stack[top--]);
    }
    output[outIndex] = NULL;
}

void freeTree(Node* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

void freePostfix(char* postfix[]) {
    int i = 0;
    while (postfix[i]) {
        free(postfix[i]);
        i++;
    }
}

int main() {
    char expression[100];
    printf("Zadejte výraz: ");
    fgets(expression, sizeof(expression), stdin);
    expression[strcspn(expression, "\n")] = '\0';

    char* postfix[100];
    shuntingYard(expression, postfix);

    printf("Postfix: ");
    for (int i = 0; postfix[i] != NULL; i++) {
        printf("%s ", postfix[i]);
    }
    printf("\n");

    int index = 0;
    while (postfix[index] != NULL) index++;
    index--;
    Node* root = createTree(postfix, &index);

    double result = evaluate(root);
    printf("Výsledek: %.2f\n", result);

    freeTree(root);
    freePostfix(postfix);
    return 0;
}
