#include <stdio.h>
#include <stdlib.h>
#include <stdint-gcc.h>
#include <string.h>
#include <stdbool.h>

#include "../common/common.h"

#define ARRSIZE 1024

#define NEWLN "\n"
#define SPACE " "
#define DELIM " -> "

typedef struct
{
    char pair[16];
    char opt[16];
} poly_t;

poly_t poly[ARRSIZE];
char seq[BUFSIZE];
int added = 0;

void print_line(char *line, int line_nbr)
{
    printf("%d : %s\n", line_nbr, line);
}

void print_input(void)
{
    printf("Current sequence: %s\n", seq);
    for (size_t i = 0; i < added; i++)
    {
        printf("%3s: %2s\n", poly[i].pair, poly[i].opt);
    }
}

char *opt_seq(char *oseq, char *p)
{
    for (size_t i = 0; i < added; i++)
    {
        printf("%3ld: %s->%s", i, poly[i].pair, p);
        if (strcmp(poly[i].pair, p) == 0)
        {
            printf("seg\n");
            oseq[0] = poly[i].pair[0];
            oseq[1] = poly[i].pair[0];
            oseq[2] = poly[i].pair[1];
            printf("->%s\n", oseq);
            // printf("%s\n", p);

            return oseq;
        }
        printf("\n");
    }
    return (char *)NULL;
}

void update_seq(int iters)
{
    char nseq[BUFSIZE];
    char p[16] = "";
    char *op;
    int ctr = 0;
    int i = 0;
    while (ctr < iters)
    {
        memset(nseq, 0, sizeof(char[BUFSIZE]));
        i = 0;
        while (i < strlen(seq) - 1)
        {
            memmove(p, seq + i, 2);
            printf("%s\n", p);
            op = opt_seq(op, p);
            if (op != NULL)
            {
                // strncpy(op, , 3);
                strcat(nseq, op);
            }
            i += 1;
        }
        strcpy(seq, nseq);

        ctr++;
    }
}

void parse_file(FILE *fp, void func(char *, int), bool silent)
{
    char next[BUFSIZE];
    char *ptr;
    int next_ctr = 0;
    int ptr_ctr = 0;

    memset(poly, 0, sizeof(poly));

    while (fgets(next, BUFSIZE, fp) != NULL)
    {
        ptr = strtok(next, NEWLN);
        // printf("ptr: %s\n", ptr);
        if (next_ctr == 0)
            strcpy(seq, ptr);
        else
        {
            ptr = strtok(ptr, DELIM);
            if (ptr != NULL)
            {
                // printf("_%s_", ptr);
                strcpy(poly[added].pair, ptr);
                ptr = strtok(NULL, DELIM);
                // printf("_%s_", ptr);
                strcpy(poly[added].opt, ptr);
                added++;
            }
        }

        ptr_ctr = 0;

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
    print_input();
    printf("%s\n", seq);
    update_seq(1);
    printf("%s\n", seq);

    fp = fopen(outp, "w");
    fprintf(fp, "Part 1 not solved yet.\n");
    fprintf(fp, "Part 2 not solved yet.\n");
    fclose(fp);
    printf("Done, see \"%s\" for result\n", outp);
    return 0;
}