#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

int main(int argc, char **argv)
{
    extern char *optarg;
    extern int optind;
    int c, err = 0;

    int t = -1, l = -1, d;
    static char usage[] = "\nusage: %s -t threads -l lines d degree\n";

    while ((c = getopt(argc, argv, "t:l:")) != -1)
    {
        switch (c)
        {
        case 't':
            t = atoi(optarg);
            break;
        case 'l':
            l = atoi(optarg);
            break;
        case '?':
            err = 1;
            break;
        }
    }

    if (t == -1)
    {
        printf("error: -t is required!\n");
        fprintf(stderr, usage, argv[0]);
        exit(1);
    }
    if (l == -1)
    {
        printf("error: -l is required!\n");
        fprintf(stderr, usage, argv[0]);
        exit(1);
    }
    if (err)
    {
        fprintf(stderr, usage, argv[0]);
        exit(1);
    }

    if (optind < argc)
    {
        d = atoi(argv[optind]);
        printf("t:\t%d\nl:\t%d\nd:\t%d\n", t, l, d);

        if (t < 1 || t > l) {
            printf("error: t should be an integer between 1-l, or in other words 1-%d!\n", l);
            fprintf(stderr, usage, argv[0]);
            exit(1);
        }
        if (l < 1 || l > 100000) {
            printf("error: l should be an integer between 1-100000!\n");
            fprintf(stderr, usage, argv[0]);
            exit(1);
        }
        if (d < 1 || d > 9) {
            printf("error: d should be an integer between 1-9!\n");
            fprintf(stderr, usage, argv[0]);
            exit(1);
        }
    }
    else
    {
        printf("error: d is required!\n");
        fprintf(stderr, usage, argv[0]);
        exit(1);
    }

    return 0;
}
