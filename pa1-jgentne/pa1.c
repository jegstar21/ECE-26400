#include <stdio.h>
#include <stdlib.h>
#include "delay.h"

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        fprintf(stderr, "Error: not enough input\n");
        return EXIT_FAILURE;
    }

    FILE * ptr = fopen(argv[1], "r");
    if (ptr == NULL)
    {
        fprintf(stderr, "Error: Input file doesn't exist\n");
        return EXIT_FAILURE;
    }
    double rd, read;
    
    Node * tree = load_tree(ptr, &rd, &read);
    fclose(ptr);

    FILE * preorderptr = fopen(argv[2], "w");
    if (preorderptr == NULL)
    {
        fprintf(stderr, "Output file 1 doesn't exist or could not be created\n");
        return EXIT_FAILURE;
    }

    if (tree != NULL) preorder(preorderptr, tree);
    fclose(preorderptr);


    FILE * delayptr = fopen(argv[3], "w");
    if (delayptr == NULL)
    {
        fprintf(stderr, "Output file 2 doesn't exist or could not be created\n");
        return EXIT_FAILURE;
    }

    if (tree != NULL) preorder_delay2(delayptr, tree, read, rd);
    fclose(delayptr);

    if (tree != NULL) destroy_tree(tree);
    return EXIT_SUCCESS;

	return 0;
}
