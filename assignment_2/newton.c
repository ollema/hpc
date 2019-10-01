#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <pthread.h>

int threads = -1, lines = -1, degree;

pthread_mutex_t result_mutex, done_mutex;

int **roots;
int **iters;
char *done;

int main(int argc, char **argv)
{
    // parse arguments
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
        printf("t:\t%d\nl:\t%d\nd:\t%d\n", threads, lines, degree);

        if (threads < 1 || threads > lines)
        {
            printf("error: threads should be an integer between 1 to lines, or in other words 1 to %d!\n", lines);
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

    // computation part below
    int ret;
    size_t line, column, thread;
    pthread_t threads[threads];

    // create result matrices
    iters = (int **)malloc(lines * sizeof(int *));
    roots = (int **)malloc(lines * sizeof(int *));
    for (line = 0; line < lines; line++)
        iters[line] = (int *)malloc(lines * sizeof(int));
        roots[line] = (int *)malloc(lines * sizeof(int));

    // create threads
    pthread_mutex_init(&result_mutex, NULL);
    pthread_mutex_init(&done_mutex, NULL);

    for (thread = 0; thread < threads; thread++)
    {
        double **arg = malloc(2 * sizeof(double *));
        // create args here
        if (ret = pthread_create(threads + thread, NULL, compute_lines, (void *)arg))
        {
            printf("Error creating thread: %d\n", ret);
            exit(1);
        }
    }

    // join threads
    for (thread = 0; thread < threads; thread++)
    {
        if (ret = pthread_join(threads[thread], NULL))
        {
            printf("Error joining thread: %d\n", ret);
            exit(1);
        }
    }

    pthread_mutex_destroy(&result_mutex);
    pthread_mutex_destroy(&done_mutex);

    return 0;
}

void *compute_lines(void *restrict arg){
    // do something with the args here
}

// TODO: Therese
void *compute_line(int lines, int degree, int line)
{
    int *result_roots = (int *)malloc(sizeof(int) * lines);
    int *result_iters = (int *)malloc(sizeof(int) * lines);

    // compute roots and iterations for a line
    // just doing a gradient for now
    for (int i = 0; i < lines; i++)
    {
        result_roots[i] = i % 10;
        result_iters[i] = (lines - i) % 20;
    }

    pthread_mutex_lock(&result_mutex);
    roots[line] = result_roots;
    iters[line] = result_iters;
    pthread_mutex_unlock(&result_mutex);

    pthread_mutex_lock(&done_mutex);
    done[line] = 1;
    pthread_mutex_unlock(&done_mutex);
}