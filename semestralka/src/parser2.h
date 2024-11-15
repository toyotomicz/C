#ifndef PARSER_H
#define PARSER_H

typedef enum {
    TOK_NUMBER,
    TOK_VARIABLE,
    TOK_FUNCTION,
    TOK_OPERATOR,
    TOK_LPAREN,
    TOK_RPAREN
} token_type_t;

typedef struct {
    token_type_t type;
    double value;
    char *name;
} token_t;

typedef struct node {
    token_t token;
    struct node *left;
    struct node *right;
} node_t;

int is_function(const char *name);
int precedence(char op);
token_t *next_token(const char **expr);
node_t *parse_expression(const char **expr);
double evaluate_node(node_t *node);

#endif // PARSER_H