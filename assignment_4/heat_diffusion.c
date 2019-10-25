#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <CL/cl.h>
#include <string.h>
#define MAX_SOURCE_SIZE (0x100000)

// arguments
int iterations = -1;
float diff_const = -1;


int width;
int height;
int i;

double *current_temperatures;
double *new_temperatures;

int main(int argc, char **argv)
{
    // ########################################################################
    // parse arguments
    // ########################################################################
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
    // input_file = fopen("/home/hpc2019/a4_grading/test_data/diffusion_100_100", "r");
    // input_file = fopen("/home/hpc2019/a4_grading/test_data/diffusion_10000_1000", "r");

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
    double *current_temperatures = calloc(width * height, sizeof(double));
    double *new_temperatures = calloc(width * height, sizeof(double));

    // Read initial values for temperature map
    int index_1;
    int index_2;
    double initial_value;

    while(fgets(char_line, 50, input_file)){
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

    exit(1);

    FILE *fp;
    char *source_str;
    size_t source_size;

    fp = fopen("kernel.cl", "r");
    if (!fp)
    {
        fprintf(stderr, "Failed to load kernel.\n");
        exit(1);
    }
    source_str = (char *)malloc(MAX_SOURCE_SIZE);
    source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
    fclose(fp);

    // Get platform and device information
    // DONE!
    cl_platform_id platform_id = NULL;
    cl_device_id device_id = NULL;
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
    cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_ALL, 1,
                         &device_id, &ret_num_devices);

    // Create an OpenCL context
    // DONE!
    cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

    // Create a command queue
    // DONE!
    cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, &ret);

    // Create memory buffers on the device for each vector
    // DONE!
    cl_mem cur_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, array_length * sizeof(double), NULL, &ret);
    cl_mem new_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, array_length * sizeof(double), NULL, &ret);

    // Copy the lists A and B to their respective memory buffers
    // TODO: use our own matrices here
    ret = clEnqueueWriteBuffer(command_queue, a_mem_obj, CL_TRUE, 0, LIST_SIZE * sizeof(int), A, 0, NULL, NULL);
    ret = clEnqueueWriteBuffer(command_queue, b_mem_obj, CL_TRUE, 0, LIST_SIZE * sizeof(int), B, 0, NULL, NULL);
    // end of use our own matrices here

    // Create a program from the kernel source
    // DONE!
    cl_program program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);

    // Build the program
    // DONE!
    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

    // Create the OpenCL kernel
    // TODO: rename string? not sure if more is needed
    cl_kernel kernel = clCreateKernel(program, "vector_add", &ret);
    //end of rename string? not sure if more is needed


    // Set the arguments of the kernel
    // TODO: use our own args here
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_mem_obj);
    ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_mem_obj);
    ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&c_mem_obj);
    // end of use our own args here


    // Execute the OpenCL kernel on the list
    // TODO: TBD
    size_t global_item_size = LIST_SIZE; // Process the entire lists
    size_t local_item_size = 64;         // Process in groups of 64
    ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);
    // end of TBD


    // Read the memory buffer C on the device to the local variable C
    // TODO: read our own results
    int *C = (int *)malloc(sizeof(int) * LIST_SIZE);
    ret = clEnqueueReadBuffer(command_queue, c_mem_obj, CL_TRUE, 0, LIST_SIZE * sizeof(int), C, 0, NULL, NULL);
    // end of read our own results


    // Display the result to the screen
    // TODO: output our results
    for (i = 0; i < LIST_SIZE; i++)
        printf("%d + %d = %d\n", A[i], B[i], C[i]);
    // end of output our results


    // Clean up
    // TODO: clean up our variables
    ret = clFlush(command_queue);
    ret = clFinish(command_queue);
    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);
    ret = clReleaseMemObject(a_mem_obj);
    ret = clReleaseMemObject(b_mem_obj);
    ret = clReleaseMemObject(c_mem_obj);
    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);
    free(A);
    free(B);
    free(C);
    // end of clean up our variables

    return 0;
}
