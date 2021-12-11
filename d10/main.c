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
#define ALL "<{[(>}])"

#define RNDOP '('
#define RNDCL ')'
#define SQROP '['
#define SQRCL ']'
#define CRLOP '{'
#define CRLCL '}'
#define NGLOP '<'
#define NGLCL '>'

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

bool check_chunk(char *chunk, char *curr)
{
    int clen = strlen(chunk);
    if (chunk[clen - 1] == RNDOP && *curr == RNDCL)
        return true;
    else if (chunk[clen - 1] == SQROP && *curr == SQRCL)
        return true;
    else if (chunk[clen - 1] == CRLOP && *curr == CRLCL)
        return true;
    else if (chunk[clen - 1] == NGLOP && *curr == NGLCL)
        return true;

    return false;
}

bool check_closure(char *chunk)
{
    if (strchr(CLOSE, chunk[strlen(chunk) - 1]) != NULL) // last is a close?
    {
        if (strchr(OPEN, chunk[strlen(chunk) - 2]) != NULL) // second last is open?
        {
            // if abs diff larger than 2, closure is of different type.
            return ((chunk[strlen(chunk) - 2] - chunk[strlen(chunk) - 1] < -2) || (chunk[strlen(chunk) - 2] - chunk[strlen(chunk) - 1] > 2));
        }
    }
    return false;
}

int parse_syntax_error(void)
{
    char *ptr;
    char line[BUFSIZE] = "";
    char chunk[BUFSIZE] = "";
    chunk_t ctr;
    ctr.rnd = 0;
    ctr.sqr = 0;
    ctr.crl = 0;
    ctr.ngl = 0;

    for (size_t i = 0; i < added; i++)
    {
        strcpy(line, lines[i]);
        ptr = strpbrk(line, ALL);
        strcpy(chunk, "");
        while (ptr != NULL)
        {
            if (check_chunk(chunk, ptr))
                chunk[strlen(chunk) - 1] = 0;
            else
            {
                strncat(chunk, ptr, 1);

                if (check_closure(chunk))
                {
                    if (*ptr == RNDOP || *ptr == RNDCL)
                        ctr.rnd++;
                    else if (*ptr == SQROP || *ptr == SQRCL)
                        ctr.sqr++;
                    else if (*ptr == CRLOP || *ptr == CRLCL)
                        ctr.crl++;
                    else if (*ptr == NGLOP || *ptr == NGLCL)
                        ctr.ngl++;
                    else
                        printf("unkown char: %c\n", *ptr);

                    break;
                }
            }
            ptr = strpbrk(ptr + 1, ALL);
        }
    }
    return (ctr.rnd * ROUND + ctr.sqr * SQUARE + ctr.crl * CURLY + ctr.ngl * ANGLE);
}

void parse_file(FILE *fp, void func(char *, int), bool silent)
{
    char next[BUFSIZE];
    char *ptr;
    int next_ctr = 0;

    while (fgets(next, BUFSIZE, fp) != NULL)
    {
        ptr = strtok(next, NEWLN);
        strcpy(lines[next_ctr], ptr);
        if (!silent)
            func(ptr, next_ctr);

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
    int syntax_error = parse_syntax_error();
    printf("Syntax error score: %d\n", syntax_error);
    // fp = fopen(outp, "w");

    // fclose(fp);
    printf("Done, see \"%s\" for result\n", outp);
    return 0;
}