#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#define SIZE 10

int main()
{
    // Not avoiding memory fragmentation:
    int ** array_1 = (int**) malloc(sizeof(int*) * SIZE);
    for ( size_t i = 0; i < SIZE; ++i )
        // Allocating one row at a time - memory could be fragmented
        array_1[i] = (int*) malloc(sizeof(int) * SIZE);

    // Same for both
    for ( size_t i = 0; i < SIZE; ++i )
        for ( size_t j = 0; j < SIZE; ++j )
            array_1[i][j] = 0;

    printf("%d\n", array_1[0][0]);

    for ( size_t i = 0; i < SIZE; ++i )
        free(array_1[i]);
    free(array_1);


    // Avoiding memory fragmentation:
    // All memory is allocated here for the full matrix
    int * array_entries = (int*) malloc(sizeof(int) * SIZE*SIZE);
    int ** array_2 = (int**) malloc(sizeof(int*) * SIZE);
    for ( size_t i = 0, j = 0; i < SIZE; ++i, j+=SIZE )
        // What is this line doing...
        array_2[i] = array_entries + j;

    // Same for both
    for ( size_t i = 0; i < SIZE; ++i )
        for ( size_t j = 0; j < SIZE; ++j )
            array_2[i][j] = 0;

    printf("%d\n", array_2[0][0]);

    free(array_2);
    free(array_entries);

    return 0;
}
