#include <stdio.h>
#include <stdlib.h>
#include <stdint-gcc.h>
#include <string.h>
#include <stdbool.h>

#include "../common/common.h"

#define ARRSIZE 1024

#define NEWLN " \n"

#define OPEN "<{[("
#define CLOSE ">}])"

#define RNDOP "("
#define RNDCL ")"
#define SQROP "["
#define SQRCL "]"
#define CRLOP "{"
#define CRLCL "}"
#define NGLOP "<"
#define NGLCL ">"

#define ROUND 3
#define SQUARE 57
#define CURLY 1197
#define ANGLE 25137

typedef struct
{
    int rnd;
    int sqr;
    int crl;
    int ngl;

} chunk_t;

char lines[ARRSIZE][BUFSIZE];
int added = 0;

void print_line(char *line, int line_nbr)
{
    printf("%d : %s\n", line_nbr, line);
}

void print_arr(unsigned long int *arr, int start, int end)
{
    int line = 0;
    printf("%4d: ", line);

    for (size_t i = start; i < end; i++)
    {
        printf("%4ld, ", arr[i]);
        if ((i - start) % 10 == 9)
        {
            line += 10;
            printf("\n%4d: ", line);
        }
    }
    printf("\n");
}

void parse_syntax(void)
{
    char *ptr;
    char line[BUFSIZE] = "";
    for (size_t i = 0; i < added; i++)
    {
        strcpy(line, lines[i]);
        ptr = strtok(line, OPEN);
        while (ptr != NULL)
        {
            printf("%s,", ptr);
            ptr = strtok(NULL, OPEN);
        }
        printf("\n");
    }
}

void parse_file(FILE *fp, void func(char *, int), bool silent)
{
    char next[BUFSIZE];
    char *ptr;
    int next_ctr = 0;
    int ptr_ctr = 0;

    while (fgets(next, BUFSIZE, fp) != NULL)
    {
        ptr = strtok(next, NEWLN);
        strcpy(lines[next_ctr], ptr);
        if (!silent)
            func(ptr, next_ctr);
        // ptr = strtok(next, NEWLN);
        // ptr_ctr = 0;
        // while (ptr != NULL)
        // {

        //     if (strlen(ptr) >= 1)
        //     {
        //         if (!silent)
        //             func(ptr, ptr_ctr);
        //         if (ptr_ctr > 0)
        //             strcpy(post_segs[next_ctr], ptr);
        //         else
        //             strcpy(pre_segs[next_ctr], ptr);
        //         ptr_ctr++;
        //     }
        //     ptr = strtok(NULL, NEWLN);
        // }
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

    parse_file(fp, print_line, false);
    fclose(fp);
    parse_syntax();
    // fp = fopen(outp, "w");

    // fclose(fp);
    printf("Done, see \"%s\" for result\n", outp);
    return 0;
}