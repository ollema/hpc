#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
    struct timespec initial_time, final_time;
    int delta_time_s;
    long double delta_time_ns;
    long double delta_time;

    long *out_data = (long *)malloc(1048576 * sizeof(long));
    long *in_data = (long *)malloc(1048576 * sizeof(long));
    for (long i = 0; i < 1048576; i++)
        out_data[i] = i;

    timespec_get(&initial_time, TIME_UTC);
    FILE *out_file = fopen("/run/mount/scratch/hpcuser004/writing_to_ssd.data", "wb");
    fwrite(out_data, sizeof(long), 1048576, out_file);
    fclose(out_file);
    timespec_get(&final_time, TIME_UTC);

    delta_time_s = final_time.tv_sec - initial_time.tv_sec;
    delta_time_ns = final_time.tv_nsec - initial_time.tv_nsec;

    delta_time = delta_time_s + delta_time_ns / 1000000000;
    printf("Write time: \t\t\t%.9Lfs\n", delta_time);

    timespec_get(&initial_time, TIME_UTC);
    FILE *in_file = fopen("/run/mount/scratch/hpcuser004/writing_to_ssd.data", "rb");
    fread(in_data, sizeof(long), 1048576, in_file);
    fclose(in_file);
    timespec_get(&final_time, TIME_UTC);

    delta_time_s = final_time.tv_sec - initial_time.tv_sec;
    delta_time_ns = final_time.tv_nsec - initial_time.tv_nsec;

    delta_time = delta_time_s + delta_time_ns / 1000000000;
    printf("Read time: \t\t\t%.9Lfs\n", delta_time);

    return 0;
}