#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <mpi.h>

MPI_Status status;

#define master 0

int main(int argc, char **argv)
{
    // ########################################################################
    // init MPI
    // ########################################################################
    int number_of_tasks, task_id;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &number_of_tasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &task_id);

    // ########################################################################
    // master
    // ########################################################################
    if (task_id == 0)
    {
        // ########################################################################
        // parse arguments
        // ########################################################################

        // arguments
        int iterations = -1;
        double diff_const = -1;

        int width;
        int height;

        double *current_temperatures;
        double *new_temperatures;

        int worker;

        extern char *optarg;
        extern int optind;
        int c, err = 0;

        static char usage[] = "\nusage: %s -n iterations -d diff_const\n";

        while ((c = getopt(argc, argv, "n:d:")) != -1)
        {
            switch (c)
            {
            case 'n':
                iterations = atoi(optarg);
                break;
            case 'd':
                diff_const = atof(optarg);
                break;
            case '?':
                err = 1;
                break;
            }
        }

        if (iterations == -1)
        {
            printf("error: -n is required!\n");
            fprintf(stderr, usage, argv[0]);
            exit(1);
        }
        if (diff_const == -1)
        {
            printf("error: -d is required!\n");
            fprintf(stderr, usage, argv[0]);
            exit(1);
        }
        if (err)
        {
            fprintf(stderr, usage, argv[0]);
            exit(1);
        }

        if (iterations < 1)
        {
            printf("error: n should be an positive integer!\n");
            fprintf(stderr, usage, argv[0]);
            exit(1);
        }
        if (diff_const < 0)
        {
            printf("error: d should be an positive float!\n");
            fprintf(stderr, usage, argv[0]);
            exit(1);
        }
        // argument parsing done!

        // ########################################################################
        // Read width, height and initial data from file
        // ########################################################################
        FILE *input_file;

        // input_file = fopen("test_input", "r");
        input_file = fopen("/home/hpc2019/a4_grading/test_data/diffusion_100_100", "r");
        // input_file = fopen("/home/hpc2019/a4_grading/test_data/diffusion_10000_1000", "r");
        // input_file = fopen("diffusion", "r");

        char char_line[40];
        char *char_token;
        const char deliminator[2] = " ";

        // Read width and height
        fgets(char_line, 40, input_file);
        char_token = strtok(char_line, deliminator);
        width = atoi(char_token);
        char_token = strtok(NULL, deliminator);
        height = atoi(char_token);

        int array_length = width * height;
        current_temperatures = calloc(array_length, sizeof(double));
        new_temperatures = calloc(array_length, sizeof(double));

        // Read initial values for temperature map
        int index_1;
        int index_2;
        double initial_value;

        while (fgets(char_line, 50, input_file))
        {
            fgets(char_line, 50, input_file);
            char_token = strtok(char_line, deliminator);
            index_2 = atoi(char_token);
            char_token = strtok(NULL, deliminator);
            index_1 = atoi(char_token);
            char_token = strtok(NULL, deliminator);
            initial_value = atof(char_token);
            current_temperatures[index_1 * width + index_2] = initial_value;
        }

        // ########################################################################
        // computation part below
        // ########################################################################
        int block_size;
        if (number_of_tasks == 1)
        {
            block_size = array_length;
        }
        else
        {
            block_size = array_length / number_of_tasks;
        }

        int offset;

        for (worker = 1, offset = block_size + array_length % number_of_tasks; worker < number_of_tasks; worker++, offset += block_size)
        {
            MPI_Send(&iterations, 1, MPI_INT, worker, 1, MPI_COMM_WORLD);
            MPI_Send(&diff_const, 1, MPI_DOUBLE, worker, 1, MPI_COMM_WORLD);
            MPI_Send(&width, 1, MPI_INT, worker, 1, MPI_COMM_WORLD);
            MPI_Send(&height, 1, MPI_INT, worker, 1, MPI_COMM_WORLD);
            MPI_Send(&block_size, 1, MPI_INT, worker, 1, MPI_COMM_WORLD);
            MPI_Send(&offset, 1, MPI_INT, worker, 1, MPI_COMM_WORLD);
        }

        for (int iter = 0; iter < iterations; iter++)
        {
            for (worker = 1; worker < number_of_tasks; worker++)
            {
                MPI_Send(current_temperatures, array_length, MPI_DOUBLE, worker, 1, MPI_COMM_WORLD);
            }
            double p1, p2, p3, p4;

            for (int i = 0; i < block_size + array_length % number_of_tasks; i++)
            {
                if (i % width == 0)
                {
                    p1 = 0;
                }
                else
                {
                    p1 = current_temperatures[i - 1];
                }

                if ((i + 1) % width == 0)
                {
                    p2 = 0;
                }
                else
                {
                    p2 = current_temperatures[i + 1];
                }

                if (i < width)
                {
                    p3 = 0;
                }
                else
                {
                    p3 = current_temperatures[i - width];
                }

                if (i >= (width * height - width))
                {
                    p4 = 0;
                }
                else
                {
                    p4 = current_temperatures[i + width];
                }

                new_temperatures[i] = diff_const * ((p1 + p2 + p3 + p4) / 4 - current_temperatures[i]) + current_temperatures[i];
            }

            for (worker = 1, offset = block_size + array_length % number_of_tasks; worker < number_of_tasks; worker++, offset += block_size)
            {
                MPI_Recv(&new_temperatures[offset], block_size, MPI_DOUBLE, worker, 2, MPI_COMM_WORLD, &status);
            }
            memcpy(current_temperatures, new_temperatures, array_length * sizeof(double));
        }

        // Display the result to the screen
        long double sum = 0.0;
        for (size_t i = 0; i < array_length; i++)
        {
            sum += new_temperatures[i];
        }
        long double average = sum / array_length;
        printf("%Lf\n", average);

        long double diff = 0.0;
        for (size_t i = 0; i < array_length; i++)
        {
            diff += fabs(average - new_temperatures[i]);
        }
        long double average_diff = diff / array_length;
        printf("%Lf\n", average_diff);
    }

    // ########################################################################
    // slave
    // ########################################################################
    if (task_id > 0)
    {
        int iters, width, height, block_size, offset;
        double diff_const;
        double *block, *temperatures;
        MPI_Recv(&iters, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&diff_const, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&width, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&height, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&block_size, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&offset, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

        block = malloc(block_size * sizeof(double));
        temperatures = malloc(width * height * sizeof(double));

        double p1, p2, p3, p4;

        for (int iter = 0; iter < iters; iter++)
        {
            MPI_Recv(temperatures, width * height, MPI_DOUBLE, master, 1, MPI_COMM_WORLD, &status);
            for (int j = 0, i = 0; j < block_size; j++)
            {
                i = j + offset;
                if (i % width == 0)
                {
                    p1 = 0;
                }
                else
                {
                    p1 = temperatures[i - 1];
                }

                if ((i + 1) % width == 0)
                {
                    p2 = 0;
                }
                else
                {
                    p2 = temperatures[i + 1];
                }

                if (i < width)
                {
                    p3 = 0;
                }
                else
                {
                    p3 = temperatures[i - width];
                }

                if (i >= (width * height - width))
                {
                    p4 = 0;
                }
                else
                {
                    p4 = temperatures[i + width];
                }

                block[j] = diff_const * ((p1 + p2 + p3 + p4) / 4 - temperatures[i]) + temperatures[i];
            }

            MPI_Send(block, block_size, MPI_DOUBLE, master, 2, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
}
