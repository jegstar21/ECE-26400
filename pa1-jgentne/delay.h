#include <stdlib.h>
#include <stdio.h>

typedef struct tree_node
{
    struct tree_node * right;
    struct tree_node * left;
    int name;
    double left_length;
    double right_length;
    double sinkCapacitor;
    double capTotal;
    double capLeft;
    double capRight;
} Node;

typedef struct list_node
{
    struct list_node * next;
    Node * node;
} ListNode;

Node * load_tree(FILE * fptr, double * rd, double * read);
Node * create_leaf(int id, double cap);
void destroy_tree(Node * tree);

ListNode * combine(ListNode * head, double leftLength, double rightLength, double c);
ListNode * create(Node * t);
ListNode * insert(ListNode * head, ListNode * insert);

double delayCalc(int name, Node * tree, double read, double rd);
void preorder(FILE * output, Node * tree);
void preorder_delay1(FILE * output, Node * tree, double read, double rd);
void preorder_delay2(FILE * output, Node * tree, double read, double rd);