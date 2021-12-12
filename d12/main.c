#include <stdio.h>
#include <stdlib.h>
#include <stdint-gcc.h>
#include <string.h>
#include <stdbool.h>

#include "../common/common.h"

#define ARRSIZE 1024

#define NEWLN " \n"
#define SPACE " "
#define DELIM "-"

#define END "end"
#define START "start"

typedef struct
{
    bool start;
    bool end;
    bool major_src;
    bool major_dst;
    char src[BUFSIZE];
    char dst[BUFSIZE];
} cave_t;

char caves[ARRSIZE][BUFSIZE];
char paths[ARRSIZE][BUFSIZE];
cave_t cpair[ARRSIZE];
int added = 0;

void print_line(char *line, int line_nbr)
{
    printf("%d : %s\n", line_nbr, line);
}

void find_paths(void)
{
    for (size_t i = 0; i < added; i++)
    {
        printf("Cave path:    %ld\n", i);
        printf("is start:     %d\n", cpair[i].start);
        printf("is end:       %d\n", cpair[i].end);
        printf("source:       %s\n", cpair[i].src);
        printf("src major:    %d\n", cpair[i].major_src);
        printf("destination:  %s\n", cpair[i].dst);
        printf("dst major:    %d\n", cpair[i].major_dst);
        printf("----------\n");
    }
}

void parse_caves(void)
{
    char cave[BUFSIZE];
    char *ptr;
    for (size_t i = 0; i < added; i++)
    {
        cpair[i].start = false;
        cpair[i].end = false;
        cpair[i].major_src = false;
        cpair[i].major_dst = false;
        strcpy(cpair[i].src, "");
        strcpy(cpair[i].dst, "");

        strcpy(cave, caves[i]);

        ptr = strtok(cave, DELIM);
        while (ptr != NULL)
        {
            if (strcmp(ptr, START) == 0)
            {

                cpair[i].start = true;
                cpair[i].end = false;
                if (strlen(cpair[i].src) > 0)
                {
                    strcpy(cpair[i].dst, cpair[i].src);
                    strcpy(cpair[i].src, START);
                }
            }
            else if (strcmp(ptr, END) == 0)
            {
                cpair[i].start = false;
                cpair[i].end = true;

                if (strlen(cpair[i].dst) > 0)
                {
                    strcpy(cpair[i].src, cpair[i].dst);
                    strcpy(cpair[i].dst, END);
                }
            }
            else
            {
                if (cpair[i].start)
                {
                    cpair[i].major_dst = ((ptr[0] >= 'A') && (ptr[0] <= 'Z'));
                    strcpy(cpair[i].dst, ptr);
                }
                else if (cpair[i].end)
                {
                    cpair[i].major_src = ((ptr[0] >= 'A') && (ptr[0] <= 'Z'));
                    strcpy(cpair[i].src, ptr);
                }
                else if (strlen(cpair[i].src) > 0)
                {
                    cpair[i].major_dst = ((ptr[0] >= 'A') && (ptr[0] <= 'Z'));
                    strcpy(cpair[i].dst, ptr);
                }
                else
                {
                    cpair[i].major_src = ((ptr[0] >= 'A') && (ptr[0] <= 'Z'));
                    strcpy(cpair[i].src, ptr);
                }
            }

            ptr = strtok(NULL, DELIM);
        }
    }
}

void parse_file(FILE *fp, void func(char *, int), bool silent)
{
    char next[BUFSIZE];
    char *ptr;
    int next_ctr = 0;

    while (fgets(next, BUFSIZE, fp) != NULL)
    {
        ptr = strtok(next, NEWLN);
        strcpy(caves[next_ctr], ptr);
        if (!silent)
            func(next, next_ctr);
        next_ctr++;
    }
    added = next_ctr;
}

int main(int argc, char *argv[])
{
    char outp[BUFSIZE] = "output";
    FILE *fp;

    if (argc > 3)
    {
        perror("Usages:\t./<name>\n\t./<name> <input file>\n\t./<name> <input file> <output file>\n");
        exit(EXIT_FAILURE);
    }
    if (argc == 1)
        fp = open_file("Start state file name", NULL);
    if (argc == 2)
        fp = open_file("", argv[1]);
    if (argc == 3)
    {
        fp = open_file("", argv[1]);
        strcpy(outp, argv[2]);
    }

    parse_file(fp, print_line, true);
    fclose(fp);
    parse_caves();
    find_paths();
    fp = fopen(outp, "w");
    fprintf(fp, "Part 1 not solved yet.\n");
    fprintf(fp, "Part 1 not solved yet.\n");
    fclose(fp);
    printf("Done, see \"%s\" for result\n", outp);
    return 0;
}