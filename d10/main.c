#include <stdio.h>
#include <stdlib.h>
#include <stdint-gcc.h>
#include <string.h>
#include <stdbool.h>

#include "../common/common.h"

#define ARRSIZE 1024

#define NEWLN " \n"

#define ALL "<{[(>}])"
#define OPEN "<{[("
#define CLOSE ">}])"

#define RNDOP '('
#define RNDCL ')'
#define SQROP '['
#define SQRCL ']'
#define CRLOP '{'
#define CRLCL '}'
#define NGLOP '<'
#define NGLCL '>'

// Syntax error points
#define ERND 3
#define ESQR 57
#define ECRL 1197
#define ENGL 25137

// Incomplete points
#define IRND 1
#define ISQR 2
#define ICRL 3
#define INGL 4

typedef struct
{
    int rnd;
    int sqr;
    int crl;
    int ngl;

} chunk_t;

char lines[ARRSIZE][BUFSIZE];
char ilines[ARRSIZE][BUFSIZE];
unsigned long int iscores[ARRSIZE];
int added = 0;
int iadded = 0;

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
            return ((chunk[strlen(chunk) - 2] - chunk[strlen(chunk) - 1] < -2) ||
                    (chunk[strlen(chunk) - 2] - chunk[strlen(chunk) - 1] > 2));
        }
    }
    return false;
}

void sort_iscores(void)
{
    inline int score_comp(const void *a, const void *b)
    {
        if (*(unsigned long int *)a < *(unsigned long int *)b)
            return -1;
        if (*(unsigned long int *)a > *(unsigned long int *)b)
            return 1;
        return 0;
    }
    qsort(iscores, iadded, sizeof(unsigned long int), score_comp);
}

unsigned long int iscore(char *end)
{
    unsigned long int score = 0;
    for (size_t i = 0; i < strlen(end); i++)
    {
        score *= 5;
        if (end[i] == RNDCL || end[i] == RNDOP)
            score += IRND;
        else if (end[i] == SQRCL || end[i] == SQROP)
            score += ISQR;
        else if (end[i] == CRLCL || end[i] == CRLOP)
            score += ICRL;
        else if (end[i] == NGLCL || end[i] == NGLOP)
            score += INGL;
    }
    return score;
}

unsigned long int complete_line(char *chunk)
{
    char rev[BUFSIZE] = "";
    int clen = strlen(chunk);
    // Reverses order but does not change symbol to the inverse
    for (size_t i = 0; i < clen; i++)
    {
        rev[i] = chunk[clen - i - 1];
    }
    return iscore(rev);
}

int parse_syntax_error(void)
{
    char *ptr;
    char line[BUFSIZE] = "";
    char chunk[BUFSIZE] = "";
    bool incomplete = true;
    chunk_t ctr;
    ctr.rnd = 0;
    ctr.sqr = 0;
    ctr.crl = 0;
    ctr.ngl = 0;

    for (size_t i = 0; i < added; i++)
    {
        incomplete = true; // assume line is incomplete until proven otherwise
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
                    incomplete = false; // line is corrupt, i.e. not incomplete
                    break;
                }
            }
            ptr = strpbrk(ptr + 1, ALL);
        }
        if (incomplete)
        {
            // The remaining chunk after parsing for syntax error
            // is the inverse (or reverse) of the incomplete line
            iscores[iadded] = complete_line(chunk);
            iadded++;
        }
    }
    return (ctr.rnd * ERND + ctr.sqr * ESQR + ctr.crl * ECRL + ctr.ngl * ENGL);
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
    sort_iscores();

    fp = fopen(outp, "w");
    fprintf(fp, "Syntax error score: %d\n", syntax_error);
    fprintf(fp, "Mid incomplete score: %ld \n", iscores[iadded / 2]);
    fclose(fp);
    printf("Done, see \"%s\" for result\n", outp);
    return 0;
}