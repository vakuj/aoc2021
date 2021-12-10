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

#define ORIG 0
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

int heights[ARRSIZE][ARRSIZE] = {0};
int bsize[ARRSIZE] = {0};
int xlen = 0;
int ylen = 0;
int bind = 0;

void print_line(char *line, int line_nbr)
{
    printf("%d : %s\n", line_nbr, line);
}

void print_arr(void)
{
    for (size_t i = 0; i <= ylen; i++)
    {
        for (size_t j = 0; j <= xlen; j++)
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

int count_bsize(size_t i, size_t j, int dir, int ctr)
{
    if (heights[i][j] == 9)
    {
        return ctr;
    }

    if (dir == ORIG) // try go to all directions
    {
        ctr++;
        ctr = count_bsize(i - 1, j, UP, ctr);
        printf("(%ld,%ld) = %d from up\n", i, j, ctr);
        ctr = count_bsize(i + 1, j, DOWN, ctr);
        printf("(%ld,%ld) = %d from down\n", i, j, ctr);
        ctr = count_bsize(i, j - 1, LEFT, ctr);
        printf("(%ld,%ld) = %d from left\n", i, j, ctr);
        ctr = count_bsize(i, j + 1, RIGHT, ctr);
        printf("(%ld,%ld) = %d from right\n", i, j, ctr);
    }
    if (dir == UP) // going up -> don't go down
    {
        // ctr = count_bsize(i, j + 1, DOWN, ctr);
        if (heights[i][j] > heights[i + 1][j])
        {
            ctr++;
            ctr = count_bsize(i - 1, j, UP, ctr);
            // ctr = count_bsize(i + 1, j, DOWN, ctr);
            ctr = count_bsize(i, j - 1, LEFT, ctr);
            ctr = count_bsize(i, j + 1, RIGHT, ctr);
        }
    }
    if (dir == DOWN) // going down -> don't go up
    {
        // ctr = count_bsize(i, j - 1, UP, ctr);

        if (heights[i][j] > heights[i - 1][j])
        {
            ctr++;
            // ctr = count_bsize(i - 1, j, UP, ctr);
            ctr = count_bsize(i + 1, j, DOWN, ctr);
            ctr = count_bsize(i, j - 1, LEFT, ctr);
            ctr = count_bsize(i, j + 1, RIGHT, ctr);
        }
    }
    if (dir == LEFT) // going left -> don't go right
    {
        // ctr = count_bsize(i + 1, j, RIGHT, ctr);

        if (heights[i][j] > heights[i][j + 1])
        {
            ctr++;
            ctr = count_bsize(i - 1, j, UP, ctr);
            // ctr = count_bsize(i + 1, j, DOWN, ctr);
            ctr = count_bsize(i, j - 1, LEFT, ctr);
            // ctr = count_bsize(i, j + 1, RIGHT, ctr);
        }
    }
    if (dir == RIGHT) // going right -> don't go left
    {
        // ctr = count_bsize(i - 1, j, LEFT, ctr);
        if (heights[i][j] > heights[i][j - 1])
        {
            ctr++;
            // ctr = count_bsize(i - 1, j, UP, ctr);
            ctr = count_bsize(i + 1, j, DOWN, ctr);
            // ctr = count_bsize(i, j - 1, LEFT, ctr);
            ctr = count_bsize(i, j + 1, RIGHT, ctr);
        }
    }

    return ctr;
}

void fill_border(void)
{
    for (size_t i = 0; i <= ylen; i++)
    {
        for (size_t j = 0; j <= xlen; j++)
        {
            if (i == 0)
                heights[i][j] = 9;
            else if (i == ylen)
                heights[i][j] = 9;
            else if (j == 0)
                heights[i][j] = 9;
            else if (j == xlen)
                heights[i][j] = 9;
        }
    }
}

int count_risk(void)
{
    int risk = 0;
    int tmp = 0;
    for (size_t i = 1; i < ylen; i++)
    {
        for (size_t j = 1; j < xlen; j++)
        {
            tmp = check_risk(heights[i][j], heights[i - 1][j], heights[i + 1][j], heights[i][j - 1], heights[i][j + 1]);
            if (tmp > 0)
            {
                bsize[bind] = count_bsize(i, j, 0, 0);
                risk += tmp;
            }
        }
    }
    return risk;
}

void parse_file(FILE *fp, void func(char *, int), bool silent)
{
    char next[BUFSIZE];
    char *ptr;
    int next_ctr = 1;
    int ptr_ctr = 0;
    while (fgets(next, BUFSIZE, fp) != NULL)
    {
        xlen = strlen(next) + 1;
        ptr_ctr = 1;
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
    fill_border();
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