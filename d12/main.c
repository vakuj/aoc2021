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

struct cave
{
    char src[16];            // self
    struct cave *dst[CAVES]; // array of possible destinations
    int dsts;
};
typedef struct cave cave_t;

char caves[ARRSIZE][BUFSIZE];
char paths[ARRSIZE][BUFSIZE];
cave_t *head = NULL;
int added = 0;
int padded = 0;

void print_line(char *line, int line_nbr)
{
    printf("%d : %s\n", line_nbr, line);
}

void print_caves(void)
{
    printf("%s\n", head->src);
    for (size_t i = 0; i < head->dsts; i++)
    {
        printf("%s, ", head->dst[i]->src);
    }
    printf("\n");
}

cave_t *new_cave(char *src)
{
    cave_t *new = (cave_t *)malloc(sizeof(cave_t));
    strcpy(new->src, src);
    for (size_t i = 0; i < CAVES; i++)
    {
        new->dst[i] = NULL;
    }
    new->dsts = 0;
    return new;
}
bool insert(cave_t *head, char *src, char *dst)
{
    if (head == NULL)
        return false;

    if (strcmp(head->src, src) == 0)
    {
        head->dst[head->dsts] = new_cave(dst);
        head->dsts++;
        return true;
    }
    for (size_t i = 0; i < head->dsts; i++)
    {
        if (head->dst[i] != NULL)
        {
            if (insert(head->dst[i], src, dst))
                return true;
        }
        else
            return false;
    }
    return false;
}

cave_t *find(cave_t *head, char *src)
{
    cave_t *found;
    if (head == NULL)
        return NULL;
    if (strcmp(head->src, src) == 0)
        return head;
    for (size_t i = 0; i < head->dsts; i++)
    {
        if (head->dst[i] != NULL)
        {
            found = find(head->dst[i], src);
            if (found != NULL)
                return found;
        }
    }
    return NULL;
}
bool comp_cave(cave_t *c, char *str)
{
    return (bool)(strcmp(c->src, str) == 0);
}

void print_ends(cave_t *head, cave_t *next, char *pre)
{
    // assert(next != NULL);
    if (next == NULL)
        return;
    cave_t *found;
    cave_t *temp;
    int ctr = 0;
    char new_pre[BUFSIZE] = "";
    if (strlen(pre) == 0)
    {
        strcat(pre, head->src);
    }

    strcpy(new_pre, pre);
    strcat(new_pre, "->");
    strcat(new_pre, next->src);
    // if (strcmp(next->src, END) == 0)
    // {
    // strcat(new_pre, END);
    printf("%s\n", new_pre);
    // return;
    // }
    for (size_t i = 0; i < next->dsts; i++)
    {
        temp = next->dst[i];
        if (temp->dst[i] == NULL && strcmp(temp->src, END) != 0)
        {
            printf("NULL at end: %s\n", temp->src);
            found = find(head, temp->src);
            // ctr = 0;
            // while (found != NULL)
            // {
            if (found != NULL)
            {
                printf("Cont at found: %s\n", found->dst[0]->src);
                print_ends(head, found, new_pre);
            }
            // found = find(found->dst[ctr], temp->src);
            // ctr++;
            // }m
        }
    }
}

void print_rec(cave_t *head, cave_t *next, char *pre)
{
    // printf("%s ", pre);
    char new_pre[BUFSIZE] = "";
    cave_t *found;
    strcpy(new_pre, pre);
    strcat(new_pre, "->");
    if (next != NULL)
    {
        strcat(new_pre, next->src);
        if (strcmp(next->src, END) == 0)
        {
            printf("%s\n", new_pre);
            // return;
        }
    }
    else
    {
        strcat(new_pre, head->src);
        if (strcmp(head->src, END) == 0)
            printf("%s\n", new_pre);
    }
    if (next != NULL)
    {
        for (size_t i = 0; i < next->dsts; i++)
        {
            if (next->dst[i] != NULL)
            {
                print_rec(head, next->dst[i], new_pre);
                // if (strcmp(next->src, END) == 0)
                found = find(head, next->src);
                if (found != NULL && found != next)
                    print_rec(head, found, new_pre);
            }
        }
    }
    else
    {
        for (size_t i = 0; i < head->dsts; i++)
        {
            if (head->dst[i] != NULL)
            {
                print_rec(head, head->dst[i], new_pre);
            }
        }
    }

    // printf("\n");
}

void parse_caves(void)
{
    char *pre, *post;
    char rbuf[BUFSIZE];
    int ctr = 0;
    int end_ctr = 0;
    bool inserted = false;
    while (ctr < added)
    {
        strcpy(rbuf, caves[ctr]);
        pre = strtok(rbuf, DELIM);
        post = strtok(NULL, DELIM);
        if (strcmp(pre, END) == 0 || strcmp(post, END) == 0)
            end_ctr++;
        if (strcmp(pre, START) == 0)
        {
            if (head == NULL)
                head = new_cave(START);
            inserted = insert(head, START, post);
            memmove(caves + ctr, caves + ctr + 1, sizeof(char[BUFSIZE]) * (added - ctr - 1));
            added--;
        }
        else if (strcmp(post, START) == 0)
        {
            if (head == NULL)
                head = new_cave(START);
            inserted = insert(head, START, pre);
            memmove(caves + ctr, caves + ctr + 1, sizeof(char[BUFSIZE]) * (added - ctr - 1));
            added--;
        }
        else
            ctr++;
    }
    ctr = 0;
    while (ctr < added)
    {
        strcpy(rbuf, caves[ctr]);
        pre = strtok(rbuf, DELIM);
        post = strtok(NULL, DELIM);
        // printf("%s - %s\n", pre, post);

        if (strcmp(pre, END) == 0)
            inserted = insert(head, post, END);

        else if (strcmp(post, END) == 0)
            inserted = insert(head, pre, END);
        else
        {
            inserted = insert(head, pre, post);
            if (!inserted)
            {
                inserted = insert(head, post, pre);
            }
        }
        if (inserted)
        {
            memmove(caves + ctr, caves + ctr + 1, sizeof(char[BUFSIZE]) * (added - ctr - 1));
            added--;
        }
        else
            ctr++;

        if (ctr > added)
        {
            ctr = 0;
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
    char pbuf[BUFSIZE] = "";
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
    // compact_cpair();
    print_caves();
    // print_rec(head, (cave_t *)NULL, pbuf);
    for (size_t i = 0; i < head->dsts; i++)
    {
        print_ends(head, head->dst[i], pbuf);
    }

    // find_paths();

    fp = fopen(outp, "w");
    fprintf(fp, "Part 1 not solved yet.\n");
    fprintf(fp, "Part 1 not solved yet.\n");
    fclose(fp);
    printf("Done, see \"%s\" for result\n", outp);
    return 0;
}