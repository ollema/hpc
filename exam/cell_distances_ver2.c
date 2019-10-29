#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <emmintrin.h>
#include <immintrin.h>

#define number_of_coords 10000
#define block_size 10000

int *distances;

float raw_coord_to_float(const char *p)
{
    float x;
    x = (p[1] - '0') * 10000 + (p[2] - '0') * 1000 + (p[4] - '0') * 100 + (p[5] - '0') * 10 + (p[6] - '0');
    if (*p == '-')
    {
        return -x;
    }
    return x;
}

int main(int argc, char **argv)
{
    // ########################################################################
    // computation part below
    // ########################################################################
    FILE *input_file;
    // input_file = fopen("test_input", "r");
    input_file = fopen("cells", "r");

    char *raw_coords_1 = malloc(sizeof(char) * block_size * 24);
    char *raw_coords_2 = malloc(sizeof(char) * block_size * 24);

    float *coords_1x = malloc(sizeof(float) * block_size);
    float *coords_1y = malloc(sizeof(float) * block_size);
    float *coords_1z = malloc(sizeof(float) * block_size);

    float *coords_2x = malloc(sizeof(float) * block_size);
    float *coords_2y = malloc(sizeof(float) * block_size);
    float *coords_2z = malloc(sizeof(float) * block_size);

    float *results = malloc(sizeof(float) * 4);

    __m128 vdp;
    float *f_result_1;
    float *f_result_2;
    float *f_result_3;
    float *f_result_4;

    int i = 0, j = 0;
    int k = 0, l = 0;
    int coord;

    distances = calloc(3465, sizeof distances);

    for (i = 0; i < number_of_coords; i += block_size)
    {
        for (j = i; j < number_of_coords; j += block_size)
        {
            fseek(input_file, i * 24, SEEK_SET);
            fread(raw_coords_1, sizeof(char), 24 * block_size, input_file);

            fseek(input_file, j * 24, SEEK_SET);
            fread(raw_coords_2, sizeof(char), 24 * block_size, input_file);

            for (coord = 0; coord < block_size; coord++)
            {
                coords_1x[coord] = raw_coord_to_float(raw_coords_1 + 0  + 24 * coord);
                coords_1y[coord] = raw_coord_to_float(raw_coords_1 + 8  + 24 * coord);
                coords_1z[coord] = raw_coord_to_float(raw_coords_1 + 16 + 24 * coord);

                coords_2x[coord] = raw_coord_to_float(raw_coords_2 + 0  + 24 * coord);
                coords_2y[coord] = raw_coord_to_float(raw_coords_2 + 8  + 24 * coord);
                coords_2z[coord] = raw_coord_to_float(raw_coords_2 + 16 + 24 * coord);
            }

            {
                for (k = 0; k < block_size; k++)
                {
                    for (l = k + 1; l < block_size; l+=4)
                    {
                        __m128 vcoords_1x = _mm_loadu_ps(&coords_1x[l]);
                        __m128 vcoords_1y = _mm_loadu_ps(&coords_1y[l]);
                        __m128 vcoords_1z = _mm_loadu_ps(&coords_1z[l]);

                        __m128 vcoords_2x = _mm_load_ps1(&coords_2x[k]);
                        __m128 vcoords_2y = _mm_load_ps1(&coords_2y[k]);
                        __m128 vcoords_2z = _mm_load_ps1(&coords_2z[k]);

                        __m128 vdiffx = _mm_sub_ps(vcoords_1x, vcoords_2x);
                        __m128 vdiffy = _mm_sub_ps(vcoords_1y, vcoords_2y);
                        __m128 vdiffz = _mm_sub_ps(vcoords_1z, vcoords_2z);

                        __m128 vdpx = _mm_mul_ps(vdiffx, vdiffx);
                        __m128 vdpy = _mm_mul_ps(vdiffy, vdiffy);
                        __m128 vdpz = _mm_mul_ps(vdiffz, vdiffz);

                        __m128 vfirst_sum = _mm_add_ps(vdpx, vdpy);
                        __m128 vsecond_sum = _mm_add_ps(vfirst_sum, vdpz);

                        __m128 vres = _mm_sqrt_ps(vsecond_sum);
                        // _mm_storeu_ps(results, vres);


                        // int distance_1 = (int)((results[0] * 0.1) + 0.5);
                        // int distance_2 = (int)((results[1] * 0.1) + 0.5);
                        // int distance_3 = (int)((results[2] * 0.1) + 0.5);
                        // int distance_4 = (int)((results[3] * 0.1) + 0.5);
                        // distances[distance_1] += 1;
                        // distances[distance_2] += 1;
                        // distances[distance_3] += 1;
                        // distances[distance_4] += 1;
                    }
                }
            }
        }
    }

    int distance;
    int number;
    int rest;

    for (int i = 0; i < 10; i++)
    {
        distance = distances[i];
        if (distance != 0)
        {
            number = i / 100;
            rest = i % 100;
            printf("%02d.%02d %d\n", number, rest, distance);
        }
    }
}
