#include <stdio.h>

int main()
{
    int out_data[10][10];
    FILE *out_file = fopen("writing_to_files.data", "wb");

    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            out_data[i][j] = i * j;

    fwrite(out_data, sizeof(int), 10 * 10, out_file);
    fclose(out_file);


    int in_data[10][10];
    FILE *in_file = fopen("writing_to_files.data", "rb");
    fread(in_data, sizeof(int), 10 * 10, in_file);

    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            if (in_data[i][j] != out_data[i][j])
                printf("Mismatch detected! %d != %d\n", in_data[i][j], out_data[i][j]);

    fclose(in_file);
    printf("Read/write successful!\n");

    return 0;
}
