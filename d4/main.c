#include <stdio.h>
#include <stdlib.h>
#include <stdint-gcc.h>
#include <string.h>
#include <stdbool.h>

#include "../common/common.h"

#define GRID 5

char head[1024][BUFSIZE];
char grid[1024][BUFSIZE];
int order[1024] = {0}, result[1024] = {0};
int bingo_length, head_length, nbr_grids;

void print_line(char *line, int line_nbr)
{
    printf("%3d : %s\n", line_nbr, line);
}

void print_grid(int start, int end)
{
    for (int i = start; i < end; i++)
    {
        printf("%3d : %s\n", i, grid[i]);
    }
}

void parse_file(FILE *fp, void func(char *, int))
{
    char next[BUFSIZE];
    char *ptr;
    int next_ctr = 0;
    int head_ctr = 0;

    fgets(next, BUFSIZE, fp);
    // printf("Header:\n\t%s\n", next);
    ptr = strtok(next, ",\n");
    while (ptr != NULL)
    {
        strcpy(head[head_ctr], ptr);
        ptr = strtok(NULL, ",\n");
        head_ctr++;
    }
    // printf("Bingo grid:\n");
    head_length = head_ctr;

    while (fgets(next, BUFSIZE, fp) != NULL)
    {
        ptr = strtok(next, "\n");
        if (ptr != NULL)
        {
            if (strlen(ptr) > 1)
            {
                // func(next, next_ctr);
                strcpy(grid[next_ctr], next);
                next_ctr++;
            }
        }
    }
    bingo_length = next_ctr;
    nbr_grids = bingo_length / GRID;
}
bool check_grid(int grid_nbr)
{
    char *ptr;
    char cpy[BUFSIZE] = "";
    int row_cnt = 0;
    int col_cnt[GRID] = {0};
    int col_ctr = 0;
    for (int i = grid_nbr * GRID; i < (grid_nbr + 1) * GRID; i++)
    {
        strcpy(cpy, "");
        col_ctr = 0;
        ptr = strtok(grid[i], " ");
        while (ptr != NULL)
        {
            if (strcmp("X", ptr) == 0)
            {
                row_cnt++;
                col_cnt[col_ctr]++;
            }
            strcat(cpy, ptr);
            strcat(cpy, " ");
            ptr = strtok(NULL, " ");
            col_ctr++;
        }
        strcpy(grid[i], cpy);
        if (row_cnt == GRID)
        {
            return true;
        }
        row_cnt = 0;
    }
    for (int i = 0; i < GRID; i++)
    {
        if (col_cnt[i] == GRID)
            return true;
    }
    return false;
}

int sum_grid(int grid_nbr)
{
    char *ptr;
    char cpy[BUFSIZE];
    int val = 0;

    for (int i = grid_nbr * GRID; i < (grid_nbr + 1) * GRID; i++)
    {
        strcpy(cpy, "");
        ptr = strtok(grid[i], " ");
        while (ptr != NULL)
        {
            if (strcmp("X", ptr) != 0)
                val += atoi(ptr);
            strcat(cpy, ptr);
            strcat(cpy, " ");
            ptr = strtok(NULL, " ");
        }
        strcpy(grid[i], cpy);
    }
    return val;
}

void play_bingo(void)
{
    bool bingo = false;
    char *ptr;
    char cpy[BUFSIZE] = "";
    int head_ctr = 0;
    int nbr = 0;
    int grid_sum = 0;
    int sum = 0;
    int grid_order[1024] = {0};

    while (!bingo && head_ctr < head_length)
    {
        for (int row = 0; row < bingo_length; row++)
        {
            strcpy(cpy, "");
            ptr = strtok(grid[row], " \n");
            while (ptr != NULL)
            {
                if (strcmp(ptr, head[head_ctr]) == 0)
                {
                    strcat(cpy, "X ");
                }
                else
                {
                    strcat(cpy, ptr);
                    strcat(cpy, " ");
                }

                ptr = strtok(NULL, " ");
            }
            strcpy(grid[row], cpy);

            if (grid_order[row / GRID] == 0)
            {
                if (check_grid((int)row / GRID))
                {
                    nbr = atoi(head[head_ctr]);
                    grid_sum = sum_grid(row / GRID);
                    grid_order[row / GRID] = 1;

                    sum = 0;
                    for (int i = 0; i < nbr_grids; i++)
                    {
                        sum += grid_order[i];
                    }
                    order[sum] = row / GRID;
                    result[sum] = nbr * grid_sum;
                    if (sum == nbr_grids)
                    {
                        bingo = true;
                        break;
                    }
                }
            }
        }

        head_ctr++;
    }
    return;
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
        fp = open_file("Bingo file name", NULL);
    if (argc == 2)
        fp = open_file("", argv[1]);
    if (argc == 3)
    {
        fp = open_file("", argv[1]);
        strcpy(outp, argv[2]);
    }

    parse_file(fp, print_line);
    fclose(fp);

    play_bingo();

    fp = fopen(outp, "w");
    fprintf(fp, "%5s %5s %5s\n", "place", "grid", "result");
    for (int i = 1; i <= 3; i++)
    {
        fprintf(fp, "%5d %5d %5d\n", i, order[i], result[i]);
    }
    fprintf(fp, "%5s %5s %5s\n", "...", "...", "...");
    for (int i = nbr_grids - 2; i <= nbr_grids; i++)
    {
        fprintf(fp, "%5d %5d %5d\n", i, order[i], result[i]);
    }
    fclose(fp);
    printf("Bingo done, see %s for result\n", outp);

    return 0;
}