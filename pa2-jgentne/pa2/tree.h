#include <stdio.h>

typedef struct tree_node {
    struct tree_node * l;
    struct tree_node * r;
    int k;
    int balance;

} Node;

Node * build(FILE * inptr, int * frm);
Node * createNode(int k);
Node * insertNode(Node * head, int k, int * frm);
Node * deleteNode(Node * node, int k);
Node * Predecessor(Node * node, int * nkey);
Node * rotation(Node * node, int k);
Node * leftR(Node * node);
Node * rightR(Node * node);

void preOrder(Node * node, FILE * outptr);
void destroy(Node * node);
void evaluate(FILE * inptr, int * frm, int * binsearch, int * balanceTree);
