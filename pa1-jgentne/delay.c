#include "delay.h"

//loads data from the files and splits into corresponding trees
Node * load_tree(FILE * fptr, double * rd, double * read)
{
    double i;
    if (!feof(fptr)) fscanf(fptr, "%le %le %le\n", read, rd, &i);

    double length1, length2, capacitor;
    int firstCharacter;
    Node * leaf;
    ListNode * head = NULL;

    firstCharacter = fgetc(fptr);

    while (firstCharacter != -1)
    {
        if (firstCharacter == 40)
        {
            fscanf(fptr, "%le %le)\n", &length1, &length2);
            head = combine(head, length1, length2, i);
            if (head == NULL) return NULL;
        }
        else
        {
            int nextCharacter = fgetc(fptr);
            int d = firstCharacter - 48;
            while (nextCharacter != 40)
            {
                d = d * 10 + (nextCharacter - 48);
                nextCharacter = fgetc(fptr);
            }
            fscanf(fptr, "%le)\n", &capacitor);
            leaf = create_leaf(d, capacitor);
            if (leaf == NULL) return NULL;
            ListNode * len = create(leaf);
            if (len == NULL) return NULL;
            head = insert(head, len);
        }
        firstCharacter = fgetc(fptr);
    }

    if (head == NULL) return NULL;
    leaf = head->node;
    free(head);
    return leaf;
}

//Function creates a leaf node
Node * create_leaf(int d, double capacitor)
{
    Node * t = malloc(sizeof(Node));
    if (t == NULL) return NULL;
    t->name = d;
    t->sinkCapacitor = capacitor;
    t->capTotal = capacitor;
    t->capLeft = 0;
    t->capRight = 0;
    t->left = NULL;
    t->right = NULL;
    return t;
}

//free memory and destroy tree
void destroy_tree(Node * tree)
{
    if (tree == NULL) return;
    destroy_tree(tree->left);
    destroy_tree(tree->right);
    free(tree);
}

//create list node for tree from postorder
ListNode * create(Node * t)
{
    ListNode * len = malloc(sizeof(ListNode));
    if (len == NULL)
    {
        return NULL;
    }
    len->node = t;
    len->next = NULL;
    return len;
}

//Function places Listnode to the front of the list
ListNode * insert(ListNode * head, ListNode * insert)
{
    insert->next = head;
    return insert;
}

//Function combines first two nodes from linked list and calculates total capacitance
ListNode * combine(ListNode * head, double leftLength, double rightLength, double i)
{
    ListNode * right = head;
    ListNode * left = head->next;
    head = head->next->next;

    Node * t = create_leaf(-1, 0);
    if (t == NULL) return NULL;
    t->left = left->node;
    t->right = right->node;
    free(right);
    free(left);

    t->left_length = leftLength;
    t->right_length = rightLength;

    t->left->capTotal += leftLength * i / 2;
    t->right->capTotal += rightLength * i / 2;
    t->capTotal = leftLength * i / 2 + rightLength * i / 2;

    t->capLeft = t->left->capLeft + t->left->capRight + t->left->capTotal;
    t->capRight = t->right->capLeft + t->right->capRight + t->right->capTotal;

    ListNode * len = create(t);
    if (len == NULL) return NULL;
    head = insert(head, len);
    return head;
}

//Function is used to print out the first file from input
void preorder(FILE * output, Node * tree)
{
    if (tree->left == NULL)
    {
        fprintf(output, "%d(%le)\n", tree->name, tree->sinkCapacitor);
        return;
    }

    fprintf(output, "(%le %le)\n", tree->left_length, tree->right_length);
    preorder(output, tree->left);
    preorder(output, tree->right);
}

typedef struct capacitor_node
{
    struct capacitor_node * next;
    double capacitor;
} Cap;

void push(Cap ** head, double capacitor)
{
    Cap * hd = *head;
    Cap * i = malloc(sizeof(Cap));
    i->capacitor = capacitor;
    i->next = hd;
    *head = i;
}

double pop(Cap ** head)
{
    Cap * p = *head;
    Cap * hd = p->next;
    double capacitor = p->capacitor;
    free(p);
    *head = hd;
    return capacitor;
}

int delayCalcAssist(int name, Node * tree, double rd, double res, double * sum, int * found, double * div, Cap ** capact)
{
    if (tree == NULL)
    {
        *found = 0;
        return 0;
    }

    if (tree->name == name)
    {
        *sum += tree->capTotal * res * res;
        *div = res;
        *found = 1;
        return 1;
    }

    int f, fl;
    double sendLeft = res + rd * tree->left_length;
    double sendRight = res + rd * tree->right_length;
    int numLeft = delayCalcAssist(name, tree->left, rd, sendLeft, sum, &f, div, capact);
    int numRight = delayCalcAssist(name, tree->right, rd, sendRight, sum, &fl, div, capact);

    if (f || fl)
    {
        int numb;
        if (f) numb = numRight;
        else numb = numLeft;
        double res2 = res * res;

        *sum += tree->capTotal * res2;

        while (numb > 0)
        {
            *sum += pop(capact) * res2;
            numb--;
        }
        *found = 1;
    }
    else
    {
        push(capact, tree->capTotal);
        *found = 0;
    }
    return numLeft + numRight + 1;
}

double delayCalc(int name, Node * tree, double read, double rd)
{
    Cap * capactHead;
    double sum = 0;
    double div;
    int isLeaf;

    delayCalcAssist(name, tree, rd, read, &sum, &isLeaf, &div, &capactHead);

    return sum / div;
}

void preorder_delayAssist(FILE * output, Node * tree, Node * head, double read, double rd)
{
    if (tree->left == NULL)
    {
        double delay = delayCalc(tree->name, head, read, rd);
        fwrite(&tree->name, sizeof(int), 1, output);
        fwrite(&delay, sizeof(double), 1, output);
        return;
    }

    preorder_delayAssist(output, tree->left, head, read, rd);
    preorder_delayAssist(output, tree->right, head, read, rd);
    return;
}

void preorder_delay(FILE * output, Node * tree, double read, double rd)
{
    preorder_delayAssist(output, tree, tree, read, rd);
    return;
}


//Calculate total time delay with O(n) complexity
void preorder_delay2Assist(FILE * output, Node * tree, double rd, double sum, double res)
{
    if (tree->left == NULL)
    {
        sum += tree->capTotal * res * res;
        double delay = sum / res;
        fwrite(&tree->name, sizeof(int), 1, output);
        fwrite(&delay, sizeof(double), 1, output);
        return;
    }
    double sendLeft = res + rd * tree->left_length;
    double sendRight = res + rd * tree->right_length;
    double sendSumLeft = ((tree->capRight + tree->capTotal) * res * res) + sum;
    double sendSumRight = ((tree->capLeft + tree->capTotal) * res * res) + sum;
    preorder_delay2Assist(output, tree->left, rd, sendSumLeft, sendLeft);
    preorder_delay2Assist(output, tree->right, rd, sendSumRight, sendRight);
}

void preorder_delay2(FILE * output, Node * tree, double read, double rd)
{
    preorder_delay2Assist(output, tree, rd, 0, read);
}