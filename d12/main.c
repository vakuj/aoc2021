#include <stdio.h>
#include <stdlib.h>
#include <stdint-gcc.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "../common/common.h"

#define ARRSIZE 1024
#define CAVES 20

#define NEWLN " \n"
#define SPACE " "
#define DELIM "-"

#define END "end"
#define START "start"

typedef struct
{
    char src[16];        // self
    char dst[CAVES][16]; // array of possible destinations
    int dsts;
} cave_t;

char paths[ARRSIZE][BUFSIZE];
cave_t caves[ARRSIZE];
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
        printf("%s -> ", caves[i].src);
        for (size_t j = 0; j < caves[i].dsts; j++)
        {
            printf("%s, ", caves[i].dst[j]);
        }
        printf("\n");
    }
    printf("\n");
}

void print_paths(void)
{

    for (size_t i = 0; i < padded; i++)
    {
        printf("%3ld: %s\n", i, paths[i]);
    }
}

int find(char *src)
{
    for (size_t i = 0; i < added; i++)
    {
        if (strcmp(caves[i].src, src) == 0)
            return (int)i;
    }
    return -1;
}

void insert(char *src, char *dst)
{
    int index = 0;
    if (strcmp(src, START) == 0 || strcmp(dst, END) == 0)
    {
        index = find(src);
        if (index >= 0)
        {
            strcpy(caves[index].dst[caves[index].dsts], dst);
            caves[index].dsts++;
        }
        else
        {
            strcpy(caves[added].src, src);
            strcpy(caves[added].dst[caves[added].dsts], dst);
            caves[added].dsts++;
            added++;
        }
    }
    else
    {
        index = find(src);
        if (index >= 0)
        {
            strcpy(caves[index].dst[caves[index].dsts], dst);
            caves[index].dsts++;
        }
        else
        {
            strcpy(caves[added].src, src);
            strcpy(caves[added].dst[caves[added].dsts], dst);
            caves[added].dsts++;
            added++;
        }
        index = find(dst);
        if (index >= 0)
        {
            strcpy(caves[index].dst[caves[index].dsts], src);
            caves[index].dsts++;
        }
        else
        {
            strcpy(caves[added].src, dst);
            strcpy(caves[added].dst[caves[added].dsts], src);
            caves[added].dsts++;
            added++;
        }
    }
}

bool visited(char *orig, char *new)
{
    char *ptr = NULL;
    if (new[0] >= 'a' && new[0] <= 'z' && strcmp(new, END) != 0)
    {
        // strcpy(ptr, orig);
        printf("visited (%s) ? : %s \n", new, orig);
        ptr = strtok(orig, new);
        if (ptr == NULL)
        {
            printf("ptr is null\n");
            return false;
        }
    }

    return true;
}

void append_caves(char *start, int n)
{
    cave_t next;
    char orig[BUFSIZE];
    char tmp[BUFSIZE];
    int ctr = 0;
    int index = 0;

    if (n < 0)
        return;
    memmove(&next, caves + n, sizeof(cave_t));
    while (ctr < next.dsts)
    {
        // ensure small caves are only used ones
        strcpy(orig, start);
        if (!visited(orig, next.dst[ctr]))
        {
            if (strcmp(next.dst[ctr], END) == 0)
            {
                strcat(orig, END);
                strcpy(paths[padded], orig);
                printf("%s\n", orig);
                padded++;
            }
            else
            {
                strcat(orig, next.dst[ctr]);
                strcat(orig, ",");
                index = find(next.dst[ctr]);
                append_caves(orig, index);
            }
        }

        ctr++;
    }
}

void parse_caves(void)
{
    cave_t start;
    char cbuf[BUFSIZE];
    int index = 0;
    int ctr = 0;
    int sctr = 0;
    index = find(START);
    if (index < 0)
        printf("Start not found...\n");
    else
    {
        memmove(&start, caves + index, sizeof(cave_t));
    }
    while (sctr < start.dsts)
    {
        memset(cbuf, 0, sizeof(cbuf));
        strcat(cbuf, START);
        strcat(cbuf, ",");
        strcat(cbuf, start.dst[sctr]);
        strcat(cbuf, ",");
        index = find(start.dst[sctr]);
        append_caves(cbuf, index);
        // printf("%s\n", cbuf);

        sctr++;
    }
}

void parse_file(FILE *fp, void func(char *, int), bool silent)
{
    char next[BUFSIZE];
    char *ptr, *pre, *post;
    int next_ctr = 0;

    while (fgets(next, BUFSIZE, fp) != NULL)
    {
        ptr = strtok(next, NEWLN);
        if (!silent)
            func(next, next_ctr);
        pre = strtok(ptr, DELIM);
        post = strtok(NULL, DELIM);

        if (strcmp(pre, START) == 0)
        {
            insert(START, post);
        }
        else if (strcmp(post, START) == 0)
        {
            insert(START, pre);
        }
        else if (strcmp(pre, END) == 0)
        {
            insert(post, END);
        }
        else if (strcmp(post, END) == 0)
        {
            insert(pre, END);
        }
        else
            insert(pre, post);

        // strcpy(caves[next_ctr], ptr);
        next_ctr++;
    }
    // added = next_ctr;
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

    print_caves();
    parse_caves();
    print_paths();

    fp = fopen(outp, "w");
    fprintf(fp, "Part 1 not solved yet.\n");
    fprintf(fp, "Part 2 not solved yet.\n");
    fclose(fp);
    printf("Done, see \"%s\" for result\n", outp);
    return 0;
}