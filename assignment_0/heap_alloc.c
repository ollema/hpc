#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#define SIZE 4000000

int main()
{
    int *array = (int*) malloc(sizeof(int) * SIZE);
    for ( size_t i = 0; i < SIZE; ++i )
        array[i] = 0;

    printf("%d\n", array[0]);

    free(array);

    return 0;
}

