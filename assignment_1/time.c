#include <stdio.h>
#include <time.h>

int main(void)
{
    struct timespec initial_time, final_time;
    int delta_time_s;
    long double delta_time_ns;
    long double delta_time;
    long double time_per_iter;
    size_t sum = 0;

    timespec_get(&initial_time, TIME_UTC);
    for (int i = 1; i <= 1000000000; i++)
    {
        sum += i;
    }
    timespec_get(&final_time, TIME_UTC);

    printf("Result: \t\t\t%zd\n", sum);
    delta_time_s = final_time.tv_sec - initial_time.tv_sec;
    delta_time_ns = final_time.tv_nsec - initial_time.tv_nsec;

    delta_time = delta_time_s + delta_time_ns / 1000000000;
    printf("Time elapsed: \t\t\t%.9Lfs\n", delta_time);

    time_per_iter = delta_time;
    printf("Average time per iteration: \t%.9Lfns\n\n", time_per_iter);
}
