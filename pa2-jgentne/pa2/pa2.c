#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

int main(int argc, char ** argv)
{
    if (argc < 3) {
        return EXIT_FAILURE;
    }

    if (argv[1][1] == 'b') {
        if (argc < 4) {
            return EXIT_FAILURE;
        }

        FILE * in = fopen(argv[2], "r");

        if (in == NULL) {
            fprintf(stdout, "%d\n", -1);
            return EXIT_FAILURE;
        }

        int frm = 1;
        Node * t = build(in, &frm);
        fclose(in);

        FILE * out = fopen(argv[3], "w");

        if (out == NULL) {
            fprintf(stdout, "%d\n", -1);
            return EXIT_FAILURE;
        }
        preOrder(t, out);
        fprintf(stdout, "%d\n", frm);
        fclose(out);
        destroy(t);
    }

    else if (argv[1][1] == 'e') {
        int frm = -1;
        int binsearch = 0;
        int balanceTree = 0;

        FILE * in = fopen(argv[2], "r");

        if (in == NULL) {
            fprintf(stdout, "%d,%d,%d\n", frm, binsearch, balanceTree);
            return EXIT_FAILURE;
        }

        evaluate(in, &frm, &binsearch, &balanceTree);
        fprintf(stdout, "%d,%d,%d\n", frm, binsearch, balanceTree);
        fclose(in);
    }

    return EXIT_SUCCESS;
}
