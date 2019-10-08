#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <complex.h>

#define lower_bound 0.000001
#define upper_bound 10000000000

int threads = -1, lines = -1, degree;

pthread_mutex_t result_mutex, done_mutex;

int **roots;
int **iters;
char *done;

// TODO: Therese
void compute_line(int line)
{
    double complex roots_h[degree];
    int *result_roots = malloc(lines * sizeof(int));
    int *result_iters = malloc(lines * sizeof(int));
    double complex z;
    double old_im_z = 2.0 - (4.0 * line / lines);
    double distance;
    size_t conv;
    size_t attr;
    double sq_abs_z;
    size_t cx = 0;

    switch (degree)
    {
    case 1:
        roots_h[0] = 1.0 + 0.0 * I;
        break;
    case 2:
        roots_h[0] = 1.0 + 0.0 * I;
        roots_h[1] = -1.0 + 0.0 * I;
        break;
    case 3:
        roots_h[0] = 1.0 + 0.0 * I;
        roots_h[1] = -0.5 - 0.86603 * I;
        roots_h[2] = -0.5 + 0.86603 * I;
        break;
    case 4:
        roots_h[0] = -1.0 + 0.0 * I;
        roots_h[1] = 1.0 + 0.0 * I;
        roots_h[2] = 0.0 - 1.0 * I;
        roots_h[3] = 0.0 - 1 * I;
        break;
    case 5:
        roots_h[0] = 1.0 + 0.0 * I;
        roots_h[1] = -0.80902 - 0.58779 * I;
        roots_h[2] = 0.30902 + 0.95106 * I;
        roots_h[3] = 0.30902 - 0.95106 * I;
        roots_h[4] = -0.80902 + 0.58779 * I;
        break;
    case 6:
        roots_h[0] = -1.0 + 0.0 * I;
        roots_h[1] = 1.0 + 0.0 * I;
        roots_h[2] = -0.5 - 0.86603 * I;
        roots_h[3] = 0.5 + 0.86603 * I;
        roots_h[4] = 0.5 - 0.86603 * I;
        roots_h[5] = -0.5 + 0.86603 * I;
        break;
    case 7:
        roots_h[0] = 1.0 + 0.0 * I;
        roots_h[1] = -0.90097 - 0.43388 * I;
        roots_h[2] = 0.62349 + 0.78183 * I;
        roots_h[3] = -0.22252 - 0.97493 * I;
        roots_h[4] = -0.22252 + 0.97493 * I;
        roots_h[5] = 0.62349 - 0.78183 * I;
        roots_h[6] = -0.90097 + 0.43388 * I;
        break;
    case 8:
        roots_h[0] = -1.0 + 0.0 * I;
        roots_h[1] = 1.0 + 0.0 * I;
        roots_h[2] = 0.0 - 1.0 * I;
        roots_h[3] = 0.0 + 1.0 * I;
        roots_h[4] = -0.70711 - 70711 * I;
        roots_h[5] = 0.70711 + 0.70711 * I;
        roots_h[6] = 0.70711 - 0.70711 * I;
        roots_h[7] = -0.70711 + 0.70711 * I;
        break;
    case 9:
        roots_h[0] = 1.0 + 0.0 * I;
        roots_h[1] = -0.93969 - 0.34202 * I;
        roots_h[2] = 0.76604 + 0.64279 * I;
        roots_h[3] = -0.5 - 0.86603 * I;
        roots_h[4] = 0.17365 + 0.98481 * I;
        roots_h[5] = 0.17365 - 0.98481 * I;
        roots_h[6] = 0.5 + 0.86603 * I;
        roots_h[7] = 0.76604 - 0.64279 * I;
        roots_h[8] = -0.93969 + 0.34202 * I;
        break;
    case 10:
        roots_h[0] = -1.0 + 0.0 * I;
        roots_h[1] = 1.0 + 0.0 * I;
        roots_h[2] = -0.80902 - 0.58779 * I;
        roots_h[3] = 0.80902 + 0.58779 * I;
        roots_h[4] = -0.30902 - 0.95106 * I;
        roots_h[5] = 0.30902 + 0.95106 * I;
        roots_h[6] = 0.30902 - 0.95106 * I;
        roots_h[7] = -0.30902 + 0.95106 * I;
        roots_h[8] = 0.80902 - 0.58779 * I;
        roots_h[9] = -0.80902 + 0.58779 * I;
        break;
    default:
        fprintf(stderr, "unexpected degree\n");
        exit(1);
    }

    for (double re_z = -2.0; cx < lines; ++cx, re_z += (4.0 / lines))
    {
        z = re_z + old_im_z * I;
        sq_abs_z = creal(z) * creal(z) + cimag(z) * cimag(z);
        for (conv = 0, attr = -1;; ++conv)
        {
            // if real or imag part of z is bigger than upper_bound, assume it will diverge
            if (creal(z) > upper_bound || cimag(z) > upper_bound)
            {
                attr = 0;
                break;
            }
            // if z sqaured is close to origin, assume it will diverge
            if (sq_abs_z < lower_bound)
            {
                attr = 0;
                break;
            }
            for (size_t ix = 0; ix < degree; ++ix)
            {
                distance = (creal(roots_h[ix]) - creal(z)) *
                               (creal(roots_h[ix]) - creal(z)) +
                           (cimag(roots_h[ix]) - cimag(z)) *
                               (cimag(roots_h[ix]) - cimag(z));
                if (distance < lower_bound)
                {
                    attr = ix + 1;
                    // printf("attr: %d\n", attr);
                    break;
                }
            }
            switch (degree)
            {
            case 1:
                z = -1.0;
                break;
            case 2:
                z = z * (1.0 / 2) + 1.0 / (2.0 * z);
                break;
            case 3:
                z = z * (2.0 / 3) + 1.0 / (3 * z * z);
                break;
            case 4:
                z = z * (3.0 / 4) + 1.0 / (4 * z * z * z);
                break;
            case 5:
                z = z * (4.0 / 5) + 1.0 / (5 * z * z * z * z);
                break;
            case 6:
                z = z * (5.0 / 6) + 1.0 / (6 * z * z * z * z * z);
                break;
            case 7:
                z = z * (6.0 / 7) + 1.0 / (7 * z * z * z * z * z * z);
                break;
            case 8:
                z = z * (7.0 / 8) + 1.0 / (8 * z * z * z * z * z * z * z);
                break;
            case 9:
                z = z * (8.0 / 9) + 1.0 / (9 * z * z * z * z * z * z * z * z);
                break;
            case 10:
                z = z * (9.0 / 10) + 1.0 / (10 * z * z * z * z * z * z * z * z * z);
                break;
            default:
                fprintf(stderr, "unexpected degree\n");
                exit(1);
            }
            if (attr == 0)
            {
                result_roots[cx] = attr;
            }
            else
            {
                result_roots[cx] = attr + 1;
            }
            result_iters[cx] = fmin(conv, 55);

            if (attr != -1)
            {
                break;
            }
        }
    }

    pthread_mutex_lock(&result_mutex);
    roots[line] = result_roots;
    iters[line] = result_iters;
    pthread_mutex_unlock(&result_mutex);

    pthread_mutex_lock(&done_mutex);
    done[line] = 1;
    pthread_mutex_unlock(&done_mutex);
    // printf("roots[0]: %d\n", roots[line][0]);
}

// int *result_roots = malloc(lines * sizeof result_roots);
// int *result_iters = malloc(lines * sizeof result_iters);

// compute roots and iterations for a line
// just doing a gradient for now
// for (int i = 0; i < lines; i++)
// {
//     result_roots[i] = i % 10;
//     result_iters[i] = i % 55;
// }

// pthread_mutex_lock(&result_mutex);
// roots[line] = result_roots;
// iters[line] = result_iters;
// pthread_mutex_unlock(&result_mutex);

// pthread_mutex_lock(&done_mutex);
// done[line] = 1;
// pthread_mutex_unlock(&done_mutex);
// }

void *compute_lines(void *restrict arg)
{

    int start_line = ((int *)arg)[0];
    int end_line = ((int *)arg)[1];
    free(arg);

    for (int line = start_line; line < end_line; line++)
    {
        compute_line(line);
    }

    return NULL;
}

void *writer_function()
{
    char colors[] = "000 000 255 000 255 000 255 000 000 100 100 100 020 020 020 120 000 120 000 020 160 255 080 020 080 \
190 030 255 025 080 ";
    char temp_char[13];
    char iter_char[37];
    double temp_lines = lines;
    int size_char_array = log10(temp_lines) + 1;
    char size_header[size_char_array];
    FILE *fptr_roots;
    FILE *fptr_iter;

    // Initialize ppm by printing headers
    sprintf(size_header, "%i", lines);
    fptr_roots = fopen("roots.ppm", "w");
    fptr_iter = fopen("iter.ppm", "w");
    char ppm_header[3] = "P3";
    fwrite(ppm_header, 1, 2, fptr_roots);
    fwrite(ppm_header, 1, 2, fptr_iter);
    fwrite("\n", 1, 1, fptr_roots);
    fwrite("\n", 1, 1, fptr_iter);
    fwrite(size_header, 1, sizeof(size_header), fptr_roots);
    fwrite(size_header, 1, sizeof(size_header), fptr_iter);
    fwrite(" ", 1, 1, fptr_roots);
    fwrite(size_header, 1, sizeof(size_header), fptr_roots);
    fwrite(" ", 1, 1, fptr_iter);
    fwrite(size_header, 1, sizeof(size_header), fptr_iter);
    fwrite("\n", 1, 1, fptr_roots);
    fwrite("255", 1, 1, fptr_roots);
    fwrite("\n", 1, 1, fptr_roots);
    fwrite("\n", 1, 1, fptr_iter);
    fwrite("55", 1, 2, fptr_iter);
    fwrite("\n", 1, 1, fptr_iter);
    struct timespec sleep_timespec = {0};
    // Sleep time one microsecond
    sleep_timespec.tv_nsec = 1 * 1000000;

    for (size_t current_line = 0; current_line < lines;)
    {
        pthread_mutex_lock(&done_mutex);
        if (done[current_line] == 1)
        {
            int *local_roots = roots[current_line];
            int *local_iter = iters[current_line];
            for (size_t index = 0; index < lines; index++)
            {
                pthread_mutex_lock(&result_mutex);

                strncpy(temp_char, colors + local_roots[index] * 12, 12);
                fwrite(temp_char, sizeof(char), 12, fptr_roots);

                sprintf(iter_char, "%i %i %i ", local_iter[index], local_iter[index], local_iter[index]);
                fwrite(iter_char, sizeof(char), strlen(iter_char), fptr_iter);

                pthread_mutex_unlock(&result_mutex);
            }
            fwrite("\n", 1, 1, fptr_roots);
            fwrite("\n", 1, 1, fptr_iter);
            current_line++;
            pthread_mutex_unlock(&done_mutex);
        }
        else
        {
            pthread_mutex_unlock(&done_mutex);
            nanosleep(&sleep_timespec, NULL);
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char **argv)
{
    // ########################################################################
    // parse arguments
    // ########################################################################
    extern char *optarg;
    extern int optind;
    int c, err = 0;

    static char usage[] = "\nusage: %s -t threads -l lines d degree\n";

    while ((c = getopt(argc, argv, "t:l:")) != -1)
    {
        switch (c)
        {
        case 't':
            threads = atoi(optarg);
            break;
        case 'l':
            lines = atoi(optarg);
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
    if (lines == -1)
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
        degree = atoi(argv[optind]);
        printf("threads: %d\nlines:   %d\ndegree:  %d\n", threads, lines, degree);

        if (threads < 1 || threads > lines)
        {
            printf("error: t should be an integer between 1 to lines, or in other words 1 to %d!\n", lines);
            fprintf(stderr, usage, argv[0]);
            exit(1);
        }
        if (lines < 1 || lines > 100000)
        {
            printf("error: l should be an integer between 1 to 100000!\n");
            fprintf(stderr, usage, argv[0]);
            exit(1);
        }
        if (degree < 1 || degree > 9)
        {
            printf("error: d should be an integer between 1 to 9!\n");
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
    // argument parsing done!

    // ########################################################################
    // computation part below
    // ########################################################################
    int ret, line, thread, block_size, rest;
    pthread_t pthreads[threads];

    block_size = lines / threads;
    rest = lines % threads;

    // create result matrices
    iters = calloc(lines, sizeof iters);
    roots = calloc(lines, sizeof roots);
    done = malloc(lines * sizeof done);

    // create threads
    pthread_mutex_init(&result_mutex, NULL);
    pthread_mutex_init(&done_mutex, NULL);

    for (thread = 0, line = 0; thread < threads; thread++, line += block_size)
    {
        int *arg = malloc(4 * sizeof arg);
        arg[0] = line;
        if ((thread == threads - 1) && (rest != 0))
        {
            // if lines is not divisible by threads, we assign the rest of the lines to the last thread. hopefully,
            // this should not affect the performance too much, since the last lines should be easy to compute anyways
            arg[1] = line + block_size + rest;
        }
        else
        {
            arg[1] = line + block_size;
        }

        if ((ret = pthread_create(pthreads + thread, NULL, compute_lines, (void *)arg)))
        {
            printf("Error creating thread: %d\n", ret);
            exit(1);
        }
    }

    pthread_t writer_thread;
    pthread_create(&writer_thread, NULL, writer_function, NULL);

    // join threads
    for (thread = 0; thread < threads; thread++)
    {
        if ((ret = pthread_join(pthreads[thread], NULL)))
        {
            printf("Error joining thread: %d\n", ret);
            exit(1);
        }
    }

    pthread_join(writer_thread, NULL);

    pthread_mutex_destroy(&result_mutex);
    pthread_mutex_destroy(&done_mutex);

    // free memory
    for (line = 0; line < lines; line++)
    {
        free(roots[line]);
        free(iters[line]);
    }
    free(roots);
    free(iters);
    free(done);

    return 0;
}
