#include <stdio.h>
#include <stdlib.h>
#include <stdint-gcc.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "../common/common.h"

#define ARRSIZE 4096
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
    char *ptr = strstr(orig, new);
    if (new[0] >= 'a' && new[0] <= 'z' && strcmp(new, END) != 0)
    {
        if (ptr == NULL)
            return false;
        else
            return true;
    }
    return false;
}
bool maxvisits(char *orig, char *new)
{
    char tmp[BUFSIZE] = "";
    char small[BUFSIZE] = "";
    char *ptr = strstr(orig, new);
    char *dptr;
    bool max = false;

    strcpy(tmp, orig);

    dptr = strtok(tmp, ",");
    while (dptr != NULL)
    {
        if (dptr[0] >= 'a' && dptr[0] <= 'z' && strcmp(dptr, END) != 0)
        {
            if (strlen(small) == 0)
            {
                strcat(small, dptr);
                strcat(small, ",");
            }
            else
            {
                if (strstr(small, dptr) != NULL)
                {
                    max = true;
                    if (strcmp(dptr, new) == 0)
                        return true;
                    break;
                }

                strcat(small, dptr);
                strcat(small, ",");
            }
        }
        dptr = strtok(NULL, ",");
    }
    if (new[0] >= 'a' && new[0] <= 'z' && strcmp(new, END) != 0)
    {
        if (ptr == NULL)
            return false;
        else
        {
            if (max)
                return true;
            ptr = strstr(ptr + strlen(new), new);
            if (ptr == NULL)
                return false;
            return true;
        }
        return true;
    }
    return false;
}

void append_caves(char *start, int n, bool (*rule)(char *, char *))
{
    cave_t next;
    char orig[BUFSIZE];

    int ctr = 0;
    int index = 0;

    if (n < 0)
        return;
    memmove(&next, caves + n, sizeof(cave_t));
    strcpy(orig, start);

    while (ctr < next.dsts)
    {
        // ensure small caves follows rules of visit.
        if (!rule(orig, next.dst[ctr]) && strcmp(next.dst[ctr], END) != 0)
        {
            strcat(orig, next.dst[ctr]);
            strcat(orig, ",");
            index = find(next.dst[ctr]);
            append_caves(orig, index, rule);
        }
        if (strcmp(next.dst[ctr], END) == 0)
        {
            strcat(orig, END);
            if (padded < ARRSIZE)
                strcpy(paths[padded], orig);
            padded++;
        }
        strcpy(orig, start);
        ctr++;
    }
}

int parse_caves(bool (*rule)(char *, char *))
{
    cave_t start;
    char cbuf[BUFSIZE];
    int index = 0;
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
        append_caves(cbuf, index, rule);

        sctr++;
    }
    return padded;
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
    int npaths = parse_caves(visited);
    memset(paths, 0, sizeof(paths));
    padded = 0;
    int mpaths = parse_caves(maxvisits);

    fp = fopen(outp, "w");
    fprintf(fp, "Part 1:\nNumber of paths: %d\n", npaths);
    fprintf(fp, "Part 2:\nNumber of paths: %d\n", mpaths);
    fclose(fp);
    printf("Done, see \"%s\" for result\n", outp);
    return 0;
}