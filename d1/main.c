#include <stdio.h>
#include <stdlib.h>
#include <stdint-gcc.h>
#include <string.h>

#include "../common/common.h"

#define ARRSIZE 2048

int meas[ARRSIZE] = {0};
int count = 0;
int depths = 0;
int slide_depths = 0;

void count_depths(void)
{
    for (size_t i = 1; i < count; i++)
    {
        if (meas[i] == -1)
            break;
        if (meas[i] > meas[i - 1])
            depths++;
    }
}

void count_sliding(void)
{
    int prev = meas[0] + meas[1] + meas[2];
    int curr = 0;
    for (size_t i = 1; i < count - 1; i++)
    {
        // if (meas[i + 1] == -1)
        //     break;
        curr = meas[i - 1] + meas[i] + meas[i + 1];
        if (curr > prev)
            slide_depths++;
        prev = curr;
    }
}

void insert_meas(char *next)
{
    meas[count] = atoi(next);
    count++;
}

void init_array(int *arr)
{
    count = 0;
    for (size_t i = 0; i < ARRSIZE; i++)
    {
        arr[i] = -1;
    }
}

int main(int argc, char *argv[])
{
    char next[BUFSIZE], outp[BUFSIZE] = "output";
    FILE *fp;
    // init_array(meas);

    if (argc > 3)
    {
        perror("Usages:\t./<name>\n\t./<name> <input file>\n\t./<name> <input file> <output file>\n");
        exit(EXIT_FAILURE);
    }
    if (argc == 1)
        fp = open_file("Meaurement file name", NULL);
    if (argc == 2)
        fp = open_file("", argv[1]);
    if (argc == 3)
    {
        fp = open_file("", argv[1]);
        strcpy(outp, argv[2]);
    }

    while (fgets(next, BUFSIZE, fp) != NULL)
    {
        insert_meas(next);
    }

    fclose(fp);

    count_depths();
    count_sliding();

    fp = fopen(outp, "w");
    fprintf(fp, "Part 1: %d\n", depths);
    fprintf(fp, "Part 2: %d\n", slide_depths);
    fclose(fp);

    return 0;
}