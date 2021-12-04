#include <stdio.h>
#include <stdlib.h>
#include <stdint-gcc.h>
#include <string.h>
#include <stdbool.h>

#define BUFSIZE 512
#define GRID 5

char head[1024][BUFSIZE];
char grid[1024][BUFSIZE];
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
    printf("Header:\n\t%s\n", next);
    ptr = strtok(next, ",\n");
    while (ptr != NULL)
    {
        strcpy(head[head_ctr], ptr);
        ptr = strtok(NULL, ",\n");
        head_ctr++;
    }
    printf("Bingo grid:\n");
    head_length = head_ctr;

    while (fgets(next, BUFSIZE, fp) != NULL)
    {
        ptr = strtok(next, "\n");
        if (ptr != NULL)
        {
            if (strlen(ptr) > 1)
            {
                func(next, next_ctr);
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
                    printf("\n");
                    printf("winner row: %d\n", row);
                    printf("winner grid: %d\n", row / GRID);
                    printf("last number: %d\n", nbr);
                    printf("sum of grid: %d\n", grid_sum);
                    printf("last x sum: %d\n", nbr * grid_sum);
                    // printf("Remaining: \n");
                    // print_grid(0, bingo_length);
                    sum = 0;
                    for (int i = 0; i < nbr_grids; i++)
                    {
                        sum += grid_order[i];
                    }
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

FILE *open_file(char *msg)
{
    FILE *fp;
    char filename[BUFSIZE];
    printf("%s:\n", msg);
    fgets(filename, BUFSIZE, stdin);
    fp = fopen(strtok(filename, "\n"), "r");
    if (fp == NULL)
    {
        perror("Could not open file");
        exit(EXIT_FAILURE);
    }
    return fp;
}

int main()
{
    FILE *fp = open_file("Bingo file name");
    parse_file(fp, print_line);
    fclose(fp);

    printf("\nPLAY!\n\n");
    play_bingo();
    printf("\nDONE!\n");

    return 0;
}