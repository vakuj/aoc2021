#include <stdio.h>
#include <stdlib.h>
#include <stdint-gcc.h>
#include <string.h>

#define BUFSIZE 1024

int meas[BUFSIZE] = {0};
int cnt = 0;
int depths = 0;

void count_depths(void)
{
    for (size_t i = 1; i < BUFSIZE; i++)
    {
        if (meas[i] > meas[i - 1])
            depths++;
    }
}

void insert_meas(char *next)
{
    if (cnt < BUFSIZE)
    {
        meas[cnt] = atoi(next);
        cnt++;
    }
    else
    {
        count_depths();
        init_array();
        meas[cnt] = atoi(next);
    }
}

void init_array(void)
{
    cnt = 0;
    for (size_t i = 0; i < BUFSIZE; i++)
    {
        meas[i] = -1;
    }
}

int main()
{
    char next[255], filename[30];
    FILE *fp;

    init_array();

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
        printf("%s", next);
        insert_meas(next);
    }

    printf("\n");
    fclose(fp);
    count_depths();
    printf("Depths increase count: %d\n", depths);

    return 0;
}