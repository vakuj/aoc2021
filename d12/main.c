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
    bool start;            // if self is start point
    bool end;              // if self is end pint
    bool major_src;        // if self is major
    char src[16];          // self
    char dst[ARRSIZE][16]; // array of possible destinations
    int dsts;
} cave_t;

char caves[ARRSIZE][BUFSIZE];
char paths[ARRSIZE][BUFSIZE];
cave_t cpair[ARRSIZE];
int added = 0;
int padded = 0;

void print_line(char *line, int line_nbr)
{
    printf("%d : %s\n", line_nbr, line);
}

void print_caves(void)
{

    for (size_t i = 0; i < added; i++)
    {
        printf("Cave path:    %ld\n", i);
        printf("is start:     %d\n", cpair[i].start);
        printf("is end:       %d\n", cpair[i].end);
        printf("source:       %s\n", cpair[i].src);
        printf("src major:    %d\n", cpair[i].major_src);
        printf("destination:  ");
        for (size_t j = 0; j < cpair[i].dsts; j++)
        {
            printf("%s, ", cpair[i].dst[j]);
        }
        printf("\n----------\n");
    }
}

void find_paths(void)
{
    cave_t start;
    int ctr = 0;

    for (size_t i = 0; i < added; i++)
    {
        if (cpair[i].start)
        {
            memmove((cave_t *)&start, (cave_t *)&cpair[i], sizeof(cave_t));
            break;
        }
    }

    while (ctr < start.dsts)
    {
        /** 
         *  Should probably do some recursive iterations here
         *  Current issue is that the compacted array is perhaps
         *  too compact, e.g. all dst's from start is not found
         *  on top level src.
         */

        ctr++;
    }
}

void compact_cpair(void)
{
    int curr, next;
    curr = 0;
    next = 1;
    while (curr < added && next < added)
    {
        if (strcmp(cpair[curr].src, cpair[next].src) == 0)
        {
            memmove(cpair[curr].dst + cpair[curr].dsts, cpair[next].dst, sizeof(char[16]) * (cpair[next].dsts));
            cpair[curr].dsts += cpair[next].dsts;
            memmove(cpair + next, cpair + next + 1, sizeof(cave_t) * (added - next - 1));
            added--;
        }
        else
            next++;

        if (next == added)
        {
            curr++;
            next = curr + 1;
        }
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
        cpair[i].dsts = 0;
        strcpy(cpair[i].src, "");

        strcpy(cave, caves[i]);

        ptr = strtok(cave, DELIM);
        while (ptr != NULL)
        {
            if (strcmp(ptr, START) == 0)
            {
                cpair[i].start = true;
                if (strlen(cpair[i].src) > 0)
                {
                    strcpy(cpair[i].dst[cpair[i].dsts], cpair[i].src);
                    cpair[i].dsts++;
                }
                strcpy(cpair[i].src, START);
            }
            else if (strcmp(ptr, END) == 0)
            {
                cpair[i].end = true;
                strcpy(cpair[i].dst[cpair[i].dsts], END);
                cpair[i].dsts++;
            }
            else
            {
                if (cpair[i].start)
                {
                    strcpy(cpair[i].dst[cpair[i].dsts], ptr);
                    cpair[i].dsts++;
                }
                else if (cpair[i].end)
                {
                    cpair[i].major_src = ((ptr[0] >= 'A') && (ptr[0] <= 'Z'));
                    strcpy(cpair[i].src, ptr);
                }
                else if (strlen(cpair[i].src) > 0)
                {
                    strcpy(cpair[i].dst[cpair[i].dsts], ptr);
                    cpair[i].dsts++;
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
    // find_paths();
    compact_cpair();
    print_caves();
    find_paths();

    fp = fopen(outp, "w");
    fprintf(fp, "Part 1 not solved yet.\n");
    fprintf(fp, "Part 1 not solved yet.\n");
    fclose(fp);
    printf("Done, see \"%s\" for result\n", outp);
    return 0;
}