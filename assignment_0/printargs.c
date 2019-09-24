#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

int main(int argc, char **argv)
{
    int optind = 0;
    int a = -1, b = -1;

    while ((optind = getopt(argc, argv, "a:b:")) != -1)
    {
        switch (optind)
        {
        case 'a':
            a = atoi(optarg);
            break;
        case 'b':
            b = atoi(optarg);
            break;
        default:
            return 1;
        }
    }

    if (a == -1)
    {
        printf("-a is required!\n");
        return 1;
    }
    if (b == -1)
    {
        printf("-b is required!\n");
        return 1;
    }

    printf("A is %d and B is %d\n", a, b);

    return 0;
}
