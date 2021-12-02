#include <stdio.h>
#include <stdlib.h>
#include <stdint-gcc.h>
#include <string.h>

#define BUFSIZE 255

#define UP "up"
#define DOWN "down"
#define FORW "forward"

int depths = 0,
    aim = 0,
    forward = 0,
    aimed_depths = 0;

void parse_line(char *line)
{
    char *div;
    char val;
    div = strtok(line, " ");
    val = atoi(strtok(NULL, " "));
    if (strcmp(div, UP) == 0)
    {
        depths -= val;
    }
    if (strcmp(div, DOWN) == 0)
    {
        depths += val;
    }
    if (strcmp(div, FORW) == 0)
    {
        forward += val;
        aimed_depths += depths * val;
    }
}

void parse_file(FILE *fp, void func(char *))
{
    char next[BUFSIZE];
    while (fgets(next, 255, fp) != NULL)
    {
        func(next);
    }
    fclose(fp);
}

FILE *open_file(char *msg)
{
    FILE *fp;
    char filename[BUFSIZE];
    printf("%s:\n", msg);
    fgets(filename, BUFSIZE, stdin);
    fp = fopen(strtok(filename, "\n"), "r");
    if (fp == NULL)
    {
        perror("Could not open file");
        exit(EXIT_FAILURE);
    }
    return fp;
}

int main()
{

    FILE *fp = open_file("Pilot file name");
    parse_file(fp, parse_line);

    printf("\n");
    printf("Depth: %d\nForward: %d\n", depths, forward);
    printf("Depth x Forward: %d\n", depths * forward);
    printf("\n");
    printf("Aimed depth: %d\n", aimed_depths);
    printf("Aimed Depth x Forward: %d\n", aimed_depths * forward);

    return 0;
}