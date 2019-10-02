#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <pthread.h>
#include <unistd.h>

int threads = -1, lines = -1, degree;

pthread_mutex_t result_mutex, done_mutex;

int **roots;
int **iters;
char *done;

// TODO: Therese
// TODO: Remove thread_number arg when we are done with development
void compute_line(int line, int thread_number)
{

    // TODO: Remove when we are done
    printf("thread %d computing line %d\n", thread_number + 1, line + 1);
    sleep(1);

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

void *compute_lines(void *restrict arg)
{

    int start_line = ((int *)arg)[0];
    int end_line = ((int *)arg)[1];
    int thread_number = ((int *)arg)[2];
    free(arg);

    for (int line = start_line; line < end_line; line++)
    {
        compute_line(line, thread_number);
    }

    return NULL;
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

    // ########################################################################
    // computation part below
    // ########################################################################
    int ret, line, thread, block_size, rest;
    pthread_t pthreads[threads];

    block_size = lines / threads;
    rest = lines % threads;

    // create result matrices
    iters = (int **)malloc(lines * sizeof(int *));
    roots = (int **)malloc(lines * sizeof(int *));
    done = (char *)malloc(lines * sizeof(char));

    // create threads
    pthread_mutex_init(&result_mutex, NULL);
    pthread_mutex_init(&done_mutex, NULL);

    for (thread = 0, line = 0; thread < threads; thread++, line += block_size)
    {
        int *arg = malloc(4 * sizeof(int));
        arg[0] = line;
        if ((thread == thread - 1) &&(rest !=0)) {
            arg[1] = line + block_size + rest;
        } else {
            arg[1] = line + block_size;
        }
        arg[2] = thread;
        if ((ret = pthread_create(pthreads + thread, NULL, compute_lines, (void *)arg)))
        {
            printf("Error creating thread: %d\n", ret);
            exit(1);
        }
    }

    // TODO: Create writer thread here

    // join threads
    for (thread = 0; thread < threads; thread++)
    {
        if ((ret = pthread_join(pthreads[thread], NULL)))
        {
            printf("Error joining thread: %d\n", ret);
            exit(1);
        }
    }

    // TODO: Join writer thread here

    pthread_mutex_destroy(&result_mutex);
    pthread_mutex_destroy(&done_mutex);

    return 0;
}
