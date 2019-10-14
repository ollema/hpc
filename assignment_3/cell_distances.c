#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

// arguments
int threads = -1;

int main(int argc, char **argv)
{
    // ########################################################################
    // parse arguments
    // ########################################################################
    extern char *optarg;
    extern int optind;
    int c, err = 0;

    static char usage[] = "\nusage: %s -t threads\n";

    while ((c = getopt(argc, argv, "t:")) != -1)
    {
        switch (c)
        {
        case 't':
            threads = atoi(optarg);
            break;
        case '?':
            err = 1;
            break;
        }
    }

    if (threads == -1)
    {
        printf("error: -t is required!\n");
        fprintf(stderr, usage, argv[0]);
        exit(1);
    }
    if (err)
    {
        fprintf(stderr, usage, argv[0]);
        exit(1);
    }

    if (threads < 1)
    {
        printf("error: t should be an positive integer!\n");
        fprintf(stderr, usage, argv[0]);
        exit(1);
    }
    // argument parsing done!

    // ########################################################################
    // computation part below
    // ########################################################################
}
