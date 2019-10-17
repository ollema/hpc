#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <math.h>
#include <time.h>

// arguments
int threads = -1;
int number_of_coords = 0;
int block_size;

int *coords_1;
int *coords_2;

int *distances;

int raw_coord_to_int(const char *p)
{
    int x = 0;
    char negative = 0;

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

void compute_distance(int i, int j, int k, int l)
{
    // Calculate distances
    int diff_1 = coords_1[3 * l] - coords_2[3 * k];
    int diff_2 = coords_1[3 * l + 1] - coords_2[3 * k + 1];
    int diff_3 = coords_1[3 * l + 2] - coords_2[3 * k + 2];

    float temp = sqrt(diff_1 * diff_1 + diff_2 * diff_2 + diff_3 * diff_3);
    int distance = (int) ((temp * 0.1) + 0.5);

    distances[distance] += 1;
}

int main(int argc, char **argv)
{
    // ########################################################################
    // parse arguments
    // ########################################################################
    struct timespec initial_time, final_time;
    int delta_time_s;
    long double delta_time_ns;
    long double delta_time;

    timespec_get(&initial_time, TIME_UTC);

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
    char ch;
    FILE *input_file;

    input_file = fopen("/home/hpc2019/a3_grading/test_data/cell_e4", "r");
    // input_file = fopen("/home/hpcuser004/assignment_3/test_input", "r");

    while (!feof(input_file))
    {
        ch = fgetc(input_file);
        if (ch == '\n')
        {
            number_of_coords++;
        }
    }

    // TODO: increase
    block_size = 10000;

    char *raw_coords_1 = malloc(sizeof raw_coords_1 * block_size * 24);
    char *raw_coords_2 = malloc(sizeof raw_coords_2 * block_size * 24);
    coords_1 = malloc(sizeof coords_1 * block_size * 3);
    coords_2 = malloc(sizeof coords_1 * block_size * 3);

    int i = 0, j = 0;
    int k = 0, l = 0;
    int coord;

    distances = calloc(34642, sizeof distances);

    for (i = 0; i < number_of_coords; i += block_size)
    {
        for (j = i; j < number_of_coords; j += block_size)
        {
            fseek(input_file, i * 24, SEEK_SET);
            fread(raw_coords_1, sizeof(char), 24 * block_size, input_file);

            fseek(input_file, j * 24, SEEK_SET);
            fread(raw_coords_2, sizeof(char), 24 * block_size, input_file);

            for (coord = 0; coord < block_size * 3; coord += 3)
            {
                coords_1[coord] = raw_coord_to_int(raw_coords_1 + 8 * coord);
                coords_1[coord + 1] = raw_coord_to_int(raw_coords_1 + 8 + 8 * coord);
                coords_1[coord + 2] = raw_coord_to_int(raw_coords_1 + 16 + 8 * coord);

                coords_2[coord] = raw_coord_to_int(raw_coords_2 + 8 * coord);
                coords_2[coord + 1] = raw_coord_to_int(raw_coords_2 + 8 + 8 * coord);
                coords_2[coord + 2] = raw_coord_to_int(raw_coords_2 + 16 + 8 * coord);
            }

            timespec_get(&final_time, TIME_UTC);

            delta_time_s = final_time.tv_sec - initial_time.tv_sec;
            delta_time_ns = final_time.tv_nsec - initial_time.tv_nsec;

            delta_time = delta_time_s + delta_time_ns / 1000000000;
            printf("Pre time elapsed: \t\t\t%.9Lfs\n", delta_time);

            for (k = 0; k < block_size; k++)
            {
                for (l = 0; l < block_size; l++)
                {
                    int global_position_row = i + k;
                    int global_position_column = j + l;
                    if (global_position_row < global_position_column)
                    {
                        compute_distance(i, j, k, l);
                    }
                }
            }

            timespec_get(&final_time, TIME_UTC);

            delta_time_s = final_time.tv_sec - initial_time.tv_sec;
            delta_time_ns = final_time.tv_nsec - initial_time.tv_nsec;

            delta_time = delta_time_s + delta_time_ns / 1000000000;
            printf("After time elapsed: \t\t\t%.9Lfs\n", delta_time);
        }
    }

    int distance;
    int number;
    int rest;

    for (int i = 0; i < 34642; i++)
    {
        distance = distances[i];
        if (distance != 0)
        {
            number = i / 100;
            rest = i % 100;
            // printf("%02d.%02d %d\n", number, rest, distance);
        }
    }
}
