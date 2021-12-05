#include <stdio.h>
#include <stdlib.h>
#include <stdint-gcc.h>
#include <string.h>
#include <stdbool.h>

#include "../common/common.h"

#define GRID 1024
#define X1 0
#define Y1 1
#define X2 2
#define Y2 3

int grid[GRID][GRID] = {0};
int coor[GRID][4] = {0};

void print_line(char *line, int line_nbr)
{
    printf("%3d : %s\n", line_nbr, line);
}

void print_grid(int start, int end)
{
    for (int i = start; i < end; i++)
    {
        printf("%3d : %ls\n", i, grid[i]);
    }
}

void parse_file(FILE *fp, void func(char *, int))
{
    char next[BUFSIZE];
    char *ptr;
    int next_ctr = 0;
    int coor_ctr = 0;

    while (fgets(next, BUFSIZE, fp) != NULL)
    {
        coor_ctr = 0;
        ptr = strtok(next, " ,->\n");
        while (ptr != NULL)
        {
            if (strlen(ptr) >= 1)
            {
                // func(next, next_ctr);
                // printf("%s, ", ptr);
                // strcpy(grid[next_ctr], next);
                coor[next_ctr][coor_ctr] = atoi(ptr);
                coor_ctr++;
            }
            ptr = strtok(NULL, " ,->\n");
        }
        next_ctr++;
    }
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
        fp = open_file("Vent file name", NULL);
    if (argc == 2)
        fp = open_file("", argv[1]);
    if (argc == 3)
    {
        fp = open_file("", argv[1]);
        strcpy(outp, argv[2]);
    }

    parse_file(fp, print_line);
    fclose(fp);

    // play_bingo();

    // fp = fopen(outp, "w");
    // fprintf(fp, "%5s %5s %5s\n", "place", "grid", "result");
    // for (int i = 1; i < nbr_grids + 1; i++)
    // {
    //     fprintf(fp, "%5d %5d %5d\n", i, order[i], result[i]);
    // }
    // fclose(fp);
    printf("Done, see %s for result\n", outp);

    return 0;
}