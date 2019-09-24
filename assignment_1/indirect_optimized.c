#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    struct timespec initial_time, final_time;
    int delta_time_s;
    long double delta_time_ns;
    long double delta_time;
    int n = 1000000, a = 2;
    double *x = (double *)malloc(n * sizeof(double));
    double *y = (double *)malloc(n * sizeof(double));

    srand(0);

    for (long i = 0; i < n; i++)
    {
        x[i] = rand() % 10;
    }

    timespec_get(&initial_time, TIME_UTC);
    for(int i=0; i < n; i++) {
        y[i] += a * x[i];
    }
    timespec_get(&final_time, TIME_UTC);

    delta_time_s = final_time.tv_sec - initial_time.tv_sec;
    delta_time_ns = final_time.tv_nsec - initial_time.tv_nsec;

    delta_time = delta_time_s + delta_time_ns / 1000000000;
    printf("First five ys: \t\t\t%.0f, %.0f, %.0f, %.0f, %.0f\n", y[0], y[1], y[2], y[3], y[4]);
    printf("Total time elapsed: \t\t%.9Lfs\n\n", delta_time);

    return 0;
}
