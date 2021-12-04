#include <stdio.h>
#include <stdlib.h>
#include <stdint-gcc.h>
#include <string.h>
#include <stdbool.h>

#include "common.h"

FILE *open_file(char *msg, char *arg)
{
    FILE *fp;
    char filename[BUFSIZE];
    if (arg == NULL)
    {
        printf("%s:\n", msg);
        fgets(filename, BUFSIZE, stdin);
    }
    else
        strcpy(filename, arg);

    fp = fopen(strtok(filename, "\n"), "r");
    if (fp == NULL)
    {
        perror("Could not open file");
        exit(EXIT_FAILURE);
    }
    return fp;
}