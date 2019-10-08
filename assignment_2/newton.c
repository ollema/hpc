#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
int threads = -1, lines = -1, degree;

pthread_mutex_t result_mutex, done_mutex;

int **roots;
int **iters;
char *done;

// TODO: Therese
void compute_line(int line)
{
    int *result_roots = malloc(lines * sizeof result_roots);
    int *result_iters = malloc(lines * sizeof result_iters);

    // compute roots and iterations for a line
    // just doing a gradient for now
    for (int i = 0; i < lines; i++)
    {
        result_roots[i] = i % 10;
        result_iters[i] = i % 55;
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
