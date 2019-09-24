#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void row_sums(double *sums, const double **matrix, size_t nrs, size_t ncs)
{
    for (size_t ix = 0; ix < nrs; ++ix)
    {
        for (size_t jx = 0; jx < ncs; ++jx)
            sums[ix] += matrix[ix][jx];
    }
}

void col_sums(double *sums, const double **matrix, size_t nrs, size_t ncs)
{
    for (size_t ix = 0; ix < nrs; ++ix)
    {
        for (size_t jx = 0; jx < ncs; ++jx)
            sums[jx] += matrix[ix][jx];
    }
}

int main()
{
    struct timespec initial_time, final_time;
    int delta_time_s;
    long double delta_time_ns;
    long double row_delta_time, col_delta_time;
    int nrs = 1000, ncs = 1000;
    double **matrix;
    double rsums[nrs], csums[ncs];

    matrix = (double **)malloc(nrs * sizeof(double *));
    for (int i = 0; i < nrs; i++)
        matrix[i] = (double *)malloc(ncs * sizeof(double));

    srand(0);

    for (int i = 0; i < nrs; i++)
    {
        for (int j = 0; j < ncs; j++)
        {
            matrix[i][j] = rand() % 10;
        }
    }

    const double **real_matrix = (const double**)matrix;

    timespec_get(&initial_time, TIME_UTC);
    row_sums(rsums, real_matrix, nrs, ncs);
    timespec_get(&final_time, TIME_UTC);

    delta_time_s = final_time.tv_sec - initial_time.tv_sec;
    delta_time_ns = final_time.tv_nsec - initial_time.tv_nsec;

    row_delta_time = delta_time_s + delta_time_ns / 1000000000;
    printf("Row sums time elapsed: \t\t\t%.9Lfs\n", row_delta_time);
    printf("First five row sums: \t\t\t%.0f, %.0f, %.0f, %.0f, %.0f\n", rsums[0], rsums[1], rsums[2], rsums[3], rsums[4]);


    timespec_get(&initial_time, TIME_UTC);
    col_sums(csums, real_matrix, nrs, ncs);
    timespec_get(&final_time, TIME_UTC);

    delta_time_s = final_time.tv_sec - initial_time.tv_sec;
    delta_time_ns = final_time.tv_nsec - initial_time.tv_nsec;

    col_delta_time = delta_time_s + delta_time_ns / 1000000000;
    printf("Col sums time elapsed: \t\t\t%.9Lfs\n", col_delta_time);
    printf("First five col sums: \t\t\t%.0f, %.0f, %.0f, %.0f, %.0f\n", csums[0], csums[1], csums[2], csums[3], csums[4]);
    printf("Total time elapsed: \t\t\t%.9Lfs\n\n", row_delta_time + col_delta_time);

    return 0;
}
