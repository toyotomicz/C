#ifndef BST_H
#define BST_H

struct bst_node {
    int key;
    struct bst_node *left;
    struct bst_node *right;
};

/*prida klic key do stromu tree*/
int bst_add(struct bst_node **root, int key);

/*uvolni strom root z pameti*/
void bst_free(struct bst_node **root);

/*vytiskne strom root*/
void bst_print(const struct bst_node *root);

/*vrati pocet uzlu stromu*/
int bst_size(const struct bst_node *root);

/*vrati vyvazenost stromu, tj. velikost praveho - velikost leveho podstromu*/
int bst_dsize(const struct bst_node *root);


#endif