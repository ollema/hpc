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

typedef void (*complex_function)(double complex *);
complex_function derivative;

void derivative_1(complex *z)
{
    *z = -1.0;
}

void derivative_2(complex *z)
{
    *z = *z * (1.0 / 2) + 1.0 / (2.0 * *z);
}

void derivative_3(complex *z)
{
    *z = *z * (2.0 / 3) + 1.0 / (3 * *z * *z);
}

void derivative_4(complex *z)
{
    *z = *z * (3.0 / 4) + 1.0 / (4 * *z * *z * *z);
}

void derivative_5(complex *z)
{
    *z = *z * (4.0 / 5) + 1.0 / (5 * *z * *z * *z * *z);
}

void derivative_6(complex *z)
{
    *z = *z * (5.0 / 6) + 1.0 / (6 * *z * *z * *z * *z * *z);
}

void derivative_7(complex *z)
{
    *z = *z * (6.0 / 7) + 1.0 / (7 * *z * *z * *z * *z * *z * *z);
}

void derivative_8(complex *z)
{
    *z = *z * (7.0 / 8) + 1.0 / (8 * *z * *z * *z * *z * *z * *z * *z);
}

void derivative_9(complex *z)
{
    *z = *z * (8.0 / 9) + 1.0 / (9 * *z * *z * *z * *z * *z * *z * *z * *z);
}

void derivative_10(complex *z)
{
    *z = *z * (9.0 / 10) + 1.0 / (10 * *z * *z * *z * *z * *z * *z * *z * *z * *z);
}

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
    double sq_abs_z;

    double distance;
    int conv;
    int attr;

    int cx = 0;

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
            // if z squared is close to origin, assume it will diverge
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
                    break;
                }
            }
            derivative(&z);
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
            if (conv > 54) {
                result_iters[cx] = 55;
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
}

void *compute_lines(void *restrict arg)
{
    int start_line = ((int *)arg)[0];
    int offset = ((int *)arg)[1];
    free(arg);

    for (int line = start_line; line < lines; line += offset)
    {
        // printf("thread %d computing line %d\n", start_line, line);
        compute_line(line);
    }

    return NULL;
}

void *writer_function()
{
    // char colors[] = "000 000 255 000 255 000 255 000 000 100 100 100 020 020 020 120 000 120 000 020 160 255 080 020 080 190 030 255 025 080 ";
    char colors[] = "0 0 8 0 8 0 8 0 0 1 1 5 1 5 1 5 1 1 2 2 2 3 2 2 2 3 2 3 2 2";
    char root_chars[6 * lines];
    char iter_chars[9 * lines];

    FILE *fptr_roots;
    FILE *fptr_iters;

    // initialize ppm by printing headers
    fptr_roots = fopen("roots.ppm", "w");
    fptr_iters = fopen("iters.ppm", "w");
    fprintf(fptr_roots, "P3\n%d %d\n8\n", lines, lines);
    fprintf(fptr_iters, "P3\n%d %d\n55\n", lines, lines);

    struct timespec sleep_timespec = {0};

    // sleep time one microsecond
    sleep_timespec.tv_nsec = 1 * 1000000;

    for (int current_line = 0; current_line < lines;)
    {
        pthread_mutex_lock(&done_mutex);
        if (done[current_line] == 1)
        {
            pthread_mutex_unlock(&done_mutex);

            pthread_mutex_lock(&result_mutex);
            int *local_roots = roots[current_line];
            int *local_iter = iters[current_line];
            pthread_mutex_unlock(&result_mutex);

            int root_index = 0;
            int iter_index = 0;

            for (int column = 0; column < lines; column++)
            {
                strncpy(root_chars + root_index, colors + local_roots[column] * 6, 6);
                root_index += 6;
                iter_index += sprintf(iter_chars + iter_index, "%d %d %d  ", local_iter[column], local_iter[column], local_iter[column]);

            }

            fwrite(root_chars, sizeof(char), 6 * lines, fptr_roots);
            fwrite(iter_chars, sizeof(char), strlen(iter_chars), fptr_iters);

            fwrite("\n", 1, 1, fptr_roots);
            fwrite("\n", 1, 1, fptr_iters);
            current_line++;
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
    switch (degree)
    {
    case 1:
        derivative = derivative_1;
        break;
    case 2:
        derivative = derivative_2;
        break;
    case 3:
        derivative = derivative_3;
        break;
    case 4:
        derivative = derivative_4;
        break;
    case 5:
        derivative = derivative_5;
        break;
    case 6:
        derivative = derivative_6;
        break;
    case 7:
        derivative = derivative_7;
        break;
    case 8:
        derivative = derivative_8;
        break;
    case 9:
        derivative = derivative_9;
        break;
    case 10:
        derivative = derivative_10;
        break;
    }

    int ret, line, thread, offset;
    pthread_t pthreads[threads];

    // offset = lines / threads;
    offset = threads;

    // create result matrices
    iters = calloc(lines, sizeof iters);
    roots = calloc(lines, sizeof roots);
    done = malloc(lines * sizeof done);

    // create threads
    pthread_mutex_init(&result_mutex, NULL);
    pthread_mutex_init(&done_mutex, NULL);

    for (thread = 0, line = 0; thread < threads; thread++)
    {
        int *arg = malloc(4 * sizeof arg);
        arg[0] = thread;
        arg[1] = offset;

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
