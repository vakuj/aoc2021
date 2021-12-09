#include <stdio.h>
#include <stdlib.h>
#include <stdint-gcc.h>
#include <string.h>
#include <stdbool.h>

#include "../common/common.h"

#define ARRSIZE 1024

#define NEWLN " \n"
#define SPACE " "
#define SCANCHR "0123456789"

int heights[ARRSIZE][ARRSIZE] = {0};
int decoded[ARRSIZE] = {0};
int xlen = 0;
int ylen = 0;

void print_line(char *line, int line_nbr)
{
    printf("%d : %s\n", line_nbr, line);
}

void print_arr(void)
{
    for (size_t i = 0; i < ylen; i++)
    {
        for (size_t j = 0; j < xlen; j++)
        {
            printf("%2d", heights[i][j]);
        }
        printf("\n");
    }
}
int check_risk(int this, int top, int bot, int left, int right)
{
    if ((this < top) && (this < bot) && (this < left) && (this < right))
        return (1 + this);
    return 0;
}
int count_risk(void)
{
    int risk = 0;
    for (size_t i = 0; i < ylen; i++)
    {
        for (size_t j = 0; j < xlen; j++)
        {
            if ((i == 0) && (j == 0))
            {
                risk += check_risk(heights[i][j], 9, heights[i + 1][j], 9, heights[i][j + 1]);
            }
            else if ((i == xlen - 1) && (j == ylen - 1))
            {
                risk += check_risk(heights[i][j], heights[i - 1][j], 9, heights[i][j - 1], 9);
            }
            else if (j == 0)
            {
                risk += check_risk(heights[i][j], heights[i - 1][j], 9, heights[i][j - 1], 9);
            }
            else if (j == ylen - 1)
            {
                risk += check_risk(heights[i][j], heights[i - 1][j], 9, heights[i][j - 1], 9);
            }
            else
            {
                risk += check_risk(heights[i][j], heights[i - 1][j], heights[i + 1][j], heights[i][j - 1], heights[i][j + 1]);
            }
            printf("%2d", heights[i][j]);
        }
        printf("\n");
    }
    return risk;
}

void parse_file(FILE *fp, void func(char *, int), bool silent)
{
    char next[BUFSIZE];
    char *ptr;
    int next_ctr = 0;
    int ptr_ctr = 0;

    while (fgets(next, BUFSIZE, fp) != NULL)
    {
        xlen = strlen(next);
        ptr_ctr = 0;
        ptr = strpbrk(next, SCANCHR);
        while (ptr != NULL)
        {
            heights[next_ctr][ptr_ctr] = *ptr - 48;
            ptr = strpbrk(ptr + 1, SCANCHR);
            ptr_ctr++;
        }

        next_ctr++;
    }
    ylen += next_ctr;
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
    print_arr();
    int risk = count_risk();
    printf("%d\n", risk);
    // fp = fopen(outp, "w");
    // fprintf(fp, "Counted 1,4,7 and 8 segments: %ld\n", cnt1);
    // fprintf(fp, "Sum of decoded segments: %ld\n", cnt2);
    // fclose(fp);
    printf("Done, see \"%s\" for result\n", outp);
    return 0;
}