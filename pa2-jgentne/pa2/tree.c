#include <stdlib.h>
#include <stdio.h>
#include "tree.h"

void preOrder2(Node * node)
{
    char value = 0;

    if (node == NULL) return;
    if(node->l != NULL) value |= 0x2;
    if(node->r != NULL) value |= 0x1;

    fprintf(stdout, "%d %d %d\n", node->k, value, node->balance);

    preOrder2(node->l);
    preOrder2(node->r);
}

Node * build(FILE * inptr, int * frm)
{
    Node * head = NULL;
    int a = 0;

    while(!feof(inptr)) {
        int k;
        char instruction;
        int x1 = fread(&k, sizeof(int), 1, inptr);
        int x2 = fread(&instruction, sizeof(char), 1, inptr);

        if (x1 != x2) {
            *frm = 0;
            return head;
        }

        if (!feof(inptr)) {
            if (instruction == 'i') {
                head = insertNode(head, k, frm);
                if (*frm == 0)
                    return head;
            }
            else if (instruction == 'd') {
                head = deleteNode(head, k);
            }
            else {
                *frm = 0;
                return head;
            }
        }
        a++;
    }

    return head;
}

void preOrder(Node * node, FILE * outptr)
{
    if (node == NULL) return;

    fwrite(&(node->k), sizeof(int), 1, outptr);

    char value = 0;
    if(node->l != NULL) value |= 0x2;
    if(node->r != NULL) value |= 0x1;

    fwrite(&value, sizeof(char), 1, outptr);

    preOrder(node->l, outptr);
    preOrder(node->r, outptr);
}

Node * createNode(int k)
{
    Node * t = malloc(sizeof(Node));

    if (t == NULL) {
        return NULL;
    }

    t->k = k;
    t->l = NULL;
    t->r = NULL;

    return t;
}

Node * insertNode(Node * head, int k, int * frm)
{
    Node * y = head;
    Node * current = head;
    Node * py = NULL;
    Node * pcurrent = NULL;
    Node * v;

    while (current != NULL) {

        if (k <= current->k){
            v = current->l;
        }
        else{
            v = current->r;
        }

        if (v != NULL && v->balance != 0) {
            py = current;
            y = v;
        }

        pcurrent = current;
        current = v;
    }

    v = createNode(k);

    if (v == NULL) {
        *frm = 0;
        return head;
    }

    v->balance = 0;

    if (pcurrent == NULL) {
        head = v;
        return head;
    }

    else {
        if (k <= pcurrent->k) {
            pcurrent->l = v;
        } 
        else {
            pcurrent->r = v;
        }
    }

    current = y;

    while (current != v)
    {
        if (k <= current->k) {
            current->balance += 1;
            current = current->l;
        }
        else {
            current->balance -= 1;
            current = current->r;
        }
    }

    if ((y->balance < 2) && (y->balance > -2)) {
        return head;
    }

    Node * ny;
    ny = rotation(y, k);

    if (py == NULL) {
        head = ny;
    }

    else if (py->l == y) {
        py->l = ny;
    }

    else {
        py->r = ny;
    }

    return head;
}

Node * deleteNodeHelp(Node * node, int k)
{

    if (node->k == k)
    {

        if (node->l == NULL && node->r == NULL) {
            free(node);
            return NULL;
        }

        else if (node->l == NULL) {
            Node * node2 = node->r;
            free(node);
            return node2;
        }

        else {
            int key;
            int pbalance = node->l->balance;
            node->l = Predecessor(node->l, &key);
            node->k = key;
            if (node->l == NULL) {
                node->balance -= 1;
            }
            else {
                if (node->l->balance == 2 || node->l->balance == -2) {
                    node->l = rotation(node->l, k);
                }
                if (pbalance != node->l->balance && node->l->balance == 0) {
                    node->balance -= 1;
                }
            }
            return node;
        }
    }

    if (k < node->k) {
        if (node->l == NULL){
            return node;
        }

        int pbalance = node->l->balance;
        node->l = deleteNodeHelp(node->l, k);

        if (node->l == NULL) {
            node->balance -= 1;
        }
        else {
            if (node->l->balance == 2 || node->l->balance == -2) {
                node->l = rotation(node->l, k);
            }
            if (pbalance != node->l->balance && node->l->balance == 0) {
                node->balance -= 1;
            }
        }
        return node;
    }
    else {
        if (node->r == NULL)
            return node;

        int pbalance = node->r->balance;
        node->r = deleteNodeHelp(node->r, k);

        if (node->r == NULL) {
            node->balance += 1;
        }
        else {
            if (node->r->balance == 2 || node->r->balance == -2) {
                node->r = rotation(node->r, k);
            }
            if (pbalance != node->r->balance && node->r->balance == 0) {
                node->balance += 1;
            }
        }
        return node;
    }
}

Node * deleteNode(Node * head, int k)
{
    head = deleteNodeHelp(head, k);
    
    if (head->balance == 2 || head->balance == -2) {
        head = rotation(head, k);
    }
    return head;
}

Node * Predecessor(Node * node, int * key)
{
    if (node->r == NULL && node->l == NULL) {
        *key = node->k;
        free(node);
        return NULL;
    }
    else if (node->r == NULL) {
        *key = node->k;
        Node * node2 = node->l;
        free(node);
        return node2;
    }
    else {
        int pbalance = node->r->balance;
        node->r = Predecessor(node->r, key);
        if (node->r == NULL) {
            node->balance += 1;
        }
        else {
            if (node->r->balance == 2 || node->r->balance == -2) {
                node->r = rotation(node->r, *key);
            }
            if (pbalance != node->r->balance && node->r->balance == 0) {
                node->balance += 1;
            }
        }
        return node;
    }
}

Node * rotation(Node * node, int k)
{
    Node * current;
    Node * child;

    if (k < node->k){
        child = node->l;
    }
    else{
        child = node->r;
    }

    Node * new = 0;

    if ((node->balance == 2) && (node->l->balance == 1)) {
        child = node->l;
        current = child;
        new = rightR(node);
        node->balance = 0;
        child->balance = 0;
    }

    if ((node->balance == -2) && (node->r->balance == -1)) {
        child = node->r;
        current = child;
        new = leftR(node);
        node->balance = 0;
        child->balance = 0;
    }

    if ((node->balance == 2) && (node->l->balance == -1)) {
        child = node->l;
        current = child->r;
        node->l = leftR(child);
        new = rightR(node);

        if (current->balance == 0){
            node->balance = 0;
            child->balance = 0;
        }
        else {
            if (current->balance == 1) {
                node->balance = -1;
                child->balance = 0;
            }
            else {
                node->balance = 0;
                child->balance = 1;
            }
            current->balance = 0;
        }
    }

    if ((node->balance == -2) && (node->r->balance == 1)) {
        child = node->r;
        current = child->l;
        node->r = rightR(child);
        new = leftR(node);

        if (current->balance == 0) {
            node->balance = 0;
            child->balance = 0;
        }
        else {
            if (current->balance == -1) {
                node->balance = 1;
                child->balance = 0;
            }
            else {
                node->balance = 0;
                child->balance = -1;
            }
            current->balance = 0;
        }
    }

    if (node->balance == 2 && node->l->balance == 0) {
        child = node->l;
        current = child;
        new = rightR(node);
        node->balance = 1;
        child->balance = -1;
    }

    if (node->balance == -2 && node->r->balance == 0) {
        child = node->r;
        current = child;
        new = leftR(node);
        node->balance = -1;
        child->balance = 1;
    }

    return new;
}

Node * leftR(Node * node)
{
    Node * new = node->r;
    Node * newNodeL = node->r->l;

    new->l = node;
    node->r = newNodeL;

    return new;
}


Node * rightR(Node * node)
{
    Node * new = node->l;
    Node * newNodeR = node->l->r;

    new->r = node;
    node->l = newNodeR;

    return new;
}

void destroy(Node * node)
{
    if (node == NULL){
        return;
    }

    destroy(node->l);
    destroy(node->r);
    free(node);
}

int evaluateTreeHelp(Node * node, int * binsearch, int * balanceTree)
{
    if (node == NULL){
        return -1;
    }

    if (node->r != NULL && node->r->k < node->k){
        *binsearch = 0;
    }

    if (node->l != NULL && node->l->k > node->k){
        *binsearch = 0;
    }

    int lHead = evaluateTreeHelp(node->l, binsearch, balanceTree);
    int rHead = evaluateTreeHelp(node->r, binsearch, balanceTree);
    node->balance = lHead - rHead;

    if (node->balance > 2 || node->balance < -2){
        *balanceTree = 0;
    }

    if (lHead > rHead){
        return lHead + 1;
    }
    else{
        return rHead + 1;
    }
}

Node * evaluateTree(FILE * inptr, int * frm)
{
    if (*frm == 0)
        return NULL;

    if (!feof(inptr))
    {
        int k;
        char value;
        int x1 = fread(&k, sizeof(int), 1, inptr);
        int x2 = fread(&value, sizeof(char), 1, inptr);

        if (feof(inptr)) return NULL;
        if (x1 != x2)
        {
            *frm = 0;
            return NULL;
        }
        if (value < 0 || value > 3)
        {
            *frm = 0;
            return NULL;
        }

        Node * t = createNode(k);

        if ((value & 0x2) == 2)
            t->l = evaluateTree(inptr, frm);
        if ((value & 0x1) == 1)
            t->r = evaluateTree(inptr, frm);

        return t;
    }

    return NULL;
}

void evaluate(FILE * inptr, int * frm, int * binsearch, int * balanceTree)
{
    *frm = 1;
    *binsearch = 1;
    *balanceTree = 1;

    Node * t = evaluateTree(inptr, frm);

    if (*frm == 0)
        return;

    evaluateTreeHelp(t, binsearch, balanceTree);
    destroy(t);
}
