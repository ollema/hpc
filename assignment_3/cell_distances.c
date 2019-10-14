#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

// arguments
int threads = -1;

int coord_to_int(const char *p)
{
    int x = 0;
    char negative = 0;

    // handle sign
    if (*p == '-')
    {
        negative = 1;
    }
    p++;

    x += (*p - '0') * 10000;
    p++;
    x += (*p - '0') * 1000;
    p += 2;
    x += (*p - '0') * 100;
    p++;
    x += (*p - '0') * 10;
    p++;
    x += (*p - '0') * 1;
    p++;

    if (negative)
    {
        x = -x;
    }
    return x;
}

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

    int block_size = 5;
    int number_of_coords = 0;
    char ch;
    FILE *input_file;

    input_file = fopen("/home/hpc2019/a3_grading/test_data/cell_50", "r");

    while (!feof(input_file))
    {
        ch = fgetc(input_file);
        if (ch == '\n')
        {
            number_of_coords++;
        }
    }

    char *raw_coords_1 = malloc(sizeof raw_coords_1 * block_size * 24);
    char *raw_coords_2 = malloc(sizeof raw_coords_2 * block_size * 24);

    int *coords_entries_1 = malloc(sizeof coords_entries_1 * block_size * 3);
    int **coords_1 = malloc(sizeof coords_1 * block_size);
    for (size_t i = 0, j = 0; i < block_size; ++i, j += 3)
        coords_1[i] = coords_entries_1 + j;

    int *coords_entries_2 = malloc(sizeof coords_entries_2 * block_size * 3);
    int **coords_2 = malloc(sizeof coords_2 * block_size);
    for (size_t i = 0, j = 0; i < block_size; ++i, j += 3)
        coords_2[i] = coords_entries_2 + j;

    int i = 0, j = 0;
    int k = 0, l = 0;

    for (i = 0; i < number_of_coords; i += block_size)
    {
        for (j = i; j < number_of_coords; j += block_size)
        {
            fseek(input_file, i * 24, SEEK_SET);
            fread(raw_coords_1, sizeof(char), 24 * block_size, input_file);

            fseek(input_file, j * 24, SEEK_SET);
            fread(raw_coords_2, sizeof(char), 24 * block_size, input_file);

            for (k = 0; k < block_size; k++)
            {
                for (l = 0; l < block_size; l++)
                {
                    // compute_distance(k, l, i, j);
                }

            }


        }
    }
}
