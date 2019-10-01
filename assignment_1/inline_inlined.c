#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    struct timespec initial_time, final_time;
    int delta_time_s;
    long double delta_time_ns;
    long double delta_time;
    double as_re[30000], as_im[30000], bs_re[30000], bs_im[30000], cs_re[30000], cs_im[30000];

    srand(0);

    for (int i = 0; i < 30000; i++) {
        bs_re[i] = rand() % 100;
        bs_im[i] = rand() % 100;
        cs_re[i] = rand() % 100;
        cs_im[i] = rand() % 100;
    }

    timespec_get(&initial_time, TIME_UTC);
    for (int i = 0; i < 30000; i++) {
        as_re[i] = bs_re[i] * cs_re[i] - bs_im[i] * cs_im[i];
        as_im[i] = bs_re[i] * cs_im[i] + bs_im[i] * cs_re[i];
    }
    timespec_get(&final_time, TIME_UTC);

    delta_time_s = final_time.tv_sec - initial_time.tv_sec;
    delta_time_ns = final_time.tv_nsec - initial_time.tv_nsec;

    delta_time = delta_time_s + delta_time_ns / 1000000000;
    printf("Time elapsed: \t\t\t%.9Lfs\n\n", delta_time);

    return 0;
}
