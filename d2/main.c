#include <stdio.h>
#include <stdlib.h>
#include <stdint-gcc.h>
#include <string.h>

#include "../common/common.h"

#define UP "up"
#define DOWN "down"
#define FORW "forward"

int depths = 0,
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
    while (fgets(next, BUFSIZE, fp) != NULL)
    {
        func(next);
    }
}

int main(int argc, char *argv[])
{

    FILE *fp; // = open_file("Pilot file name");
    char outp[BUFSIZE] = "output";

    if (argc > 3)
    {
        perror("Usages:\t./<name>\n\t./<name> <input file>\n\t./<name> <input file> <output file>\n");
        exit(EXIT_FAILURE);
    }
    if (argc == 1)
        fp = open_file("Pilot file name", NULL);
    if (argc == 2)
        fp = open_file("", argv[1]);
    if (argc == 3)
    {
        fp = open_file("", argv[1]);
        strcpy(outp, argv[2]);
    }

    parse_file(fp, parse_line);
    fclose(fp);
    fp = fopen(outp, "w");
    fprintf(fp, "Part 1:\n\tDpth: %d\n\tFwrd: %d\n", depths, forward);
    fprintf(fp, " Dpth x Fwrd: %d\n", depths * forward);
    fprintf(fp, "\nPart 2:\n\tAimed Dpth: %d\n", aimed_depths);
    fprintf(fp, " Aimed Dpth x Fwrd: %d\n", aimed_depths * forward);
    fclose(fp);
    return 0;
}