#include <stdio.h>
#include <stdlib.h>
#include <stdint-gcc.h>
#include <string.h>

#define BUFSIZE 2048

int meas[BUFSIZE] = {0};
int cnt = 0;
int depths = 0;
int slide_depths = 0;

void count_depths(void)
{
    for (size_t i = 1; i < BUFSIZE; i++)
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
    for (size_t i = 1; i < BUFSIZE - 1; i++)
    {
        if (meas[i + 1] == -1)
            break;
        curr = meas[i - 1] + meas[i] + meas[i + 1];
        if (curr > prev)
            slide_depths++;
        prev = curr;
    }
}

void insert_meas(char *next)
{

    meas[cnt] = atoi(next);
    cnt++;
}

void init_array(int *arr)
{
    cnt = 0;
    for (size_t i = 0; i < BUFSIZE; i++)
    {
        arr[i] = -1;
    }
}

int main()
{
    char next[255], filename[30];
    FILE *fp;

    init_array(meas);

    printf("Measurement file name:\n");
    gets(filename);
    printf("\n");
    fp = fopen(filename, "r");

    if (fp == NULL)
    {
        perror("Could not open file");
        exit(EXIT_FAILURE);
    }

    while (fgets(next, 255, fp) != NULL)
    {
        insert_meas(next);
    }

    fclose(fp);
    count_depths();
    printf("Depths increase count: %d\n", depths);
    count_sliding();
    printf("Sliding depths increase count: %d\n", slide_depths);

    return 0;
}