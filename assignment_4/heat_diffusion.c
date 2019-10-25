#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <CL/cl.h>

#define MAX_SOURCE_SIZE (0x100000)

// arguments
int iterations = -1;
float diff_const = -1;
int width;
int height;
int i;

double **temperatures;

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
    // computation part below
    // ########################################################################

    // TODO: determine if we need two matrices - one for current state vs. next state etc.
    double *temperature_entries = malloc(sizeof(double) * width * height);
    temperatures = malloc(sizeof(double *) * height);
    for (size_t i = 0, j = 0; i < height; ++i, j += width)
        temperatures[i] = temperature_entries + j;

    for (size_t i = 0; i < width; ++i)
        for (size_t j = 0; j < height; ++j)
            temperatures[i][j] = 0.0;
    // end of determine if we need two matrices - one for current state vs. next state etc.

    // TODO: remove and replace with our own above
    const int LIST_SIZE = 1024;
    int *A = (int *)malloc(sizeof(int) * LIST_SIZE);
    int *B = (int *)malloc(sizeof(int) * LIST_SIZE);
    for (i = 0; i < LIST_SIZE; i++)
    {
        A[i] = i;
        B[i] = LIST_SIZE - i;
    }
    // end of remove and replace with our own above

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
    // TODO: use correct size for buffers here
    cl_mem a_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, LIST_SIZE * sizeof(int), NULL, &ret);
    cl_mem b_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, LIST_SIZE * sizeof(int), NULL, &ret);
    cl_mem c_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, LIST_SIZE * sizeof(int), NULL, &ret);
    // end of use correct size for buffers here

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
