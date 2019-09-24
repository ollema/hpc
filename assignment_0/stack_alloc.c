#include <stdio.h>
#include <stddef.h>

#define SIZE 4000000

int main()
{
    int array[SIZE];
    for ( size_t i = 0; i < SIZE; ++i )
        array[i] = 0;

    printf("%d\n", array[0]);

    return 0;
}

