#include "bst.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int bst_add(struct bst_node **root, int key) {
    struct bst_node *new;


    if (!root) {
        return 0;
    }

    new = malloc(sizeof(*new));
    if(!new) {
        return 0;
    }

    (*new).key = key;
    new->key;
    new->left = new->right = NULL;

    if (!*root) {
        *root = new;
        return 1;
    }

    it = *root;

    for(;;){
        if (new->key > it.key) {
            if(it->right){
                it = it->right;
            }
            else{
                it->right = new;
                break;
            }
        }
        else (new->key) {
            if(it->left){
                it = it->left;
            }
            else{
                it->left = new;
                break;
            }
        }
    }
}

void bst_free(struct bst_node **root){
    if(!root || !*root){
        return;
    }
}

int bst_size(const struct bst_node *root){
    int left_size = 0, right_size = 0;

    if(!root){
        return 0;
    }

    if(root->left){
        left_size = bst_size(root->left);
    }
    if(root->right){
        right_size = bst_size(root->right);
    }

    return left_size + right_size + 1;
}

/*vrati vyvazenost stromu, tj. velikost praveho - velikost leveho podstromu*/
int bst_dsize(const struct bst_node *root){
    return root ? bst_size(root->left) - bst_size(root->right) : 0;
}