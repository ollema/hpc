#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <math.h>

// arguments
int iterations = -1;
float diff_const = -1;


int main(int argc, char **argv)
{
    // ########################################################################
    // parse arguments
    // ########################################################################
    extern char *optarg;
    extern int optind;
    int c, err = 0;

    static char usage[] = "\nusage: %s -n iterations -d diff_const\n";

    while ((c = getopt(argc, argv, "n:d:")) != -1)
    {
        switch (c)
        {
        case 'n':
            iterations = atoi(optarg);
            break;
        case 'd':
            diff_const = atof(optarg);
            break;
        case '?':
            err = 1;
            break;
        }
    }

    if (iterations == -1)
    {
        printf("error: -n is required!\n");
        fprintf(stderr, usage, argv[0]);
        exit(1);
    }
    if (diff_const == -1)
    {
        printf("error: -d is required!\n");
        fprintf(stderr, usage, argv[0]);
        exit(1);
    }
    if (err)
    {
        fprintf(stderr, usage, argv[0]);
        exit(1);
    }

    if (iterations < 1)
    {
        printf("error: n should be an positive integer!\n");
        fprintf(stderr, usage, argv[0]);
        exit(1);
    }
    if (diff_const < 0)
    {
        printf("error: d should be an positive float!\n");
        fprintf(stderr, usage, argv[0]);
        exit(1);
    }
    // argument parsing done!

    // ########################################################################
    // computation part below
    // ########################################################################

    printf("iterations: %d\ndiff_const: %f\n\n", iterations, diff_const);
}
