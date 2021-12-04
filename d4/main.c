#include <stdio.h>
#include <stdlib.h>
#include <stdint-gcc.h>
#include <string.h>
#include <stdbool.h>

#define BUFSIZE 512
#define GRID 5

char head[1024][BUFSIZE];
char grid[1024][BUFSIZE];
int bingo_length, head_length;

void print_line(char *line, int line_nbr)
{
    printf("%d (%ld) : %s\n", line_nbr, strlen(line), line);
}

void parse_file(FILE *fp, void func(char *, int))
{
    char next[BUFSIZE];
    char *ptr;
    int next_ctr = 0;
    int head_ctr = 0;

    fgets(next, BUFSIZE, fp);
    ptr = strtok(next, ",\n");
    while (ptr != NULL)
    {
        printf("%s-", ptr);
        strcpy(head[head_ctr], ptr);
        ptr = strtok(NULL, ",\n");
        head_ctr++;
    }
    printf("\n");
    head_length = head_ctr;
    // strcpy(head, next);

    // func(head, -1);

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
}
bool check_grid(int grid_nbr)
{
    char *ptr;
    char cpy[BUFSIZE] = "";
    int row_cnt = 0;
    int col_cnt[GRID] = {0};
    int col_ctr = 0;
    // printf("\ngrid[%d]: \n", grid_nbr);
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
        // if (grid_nbr == 2)
        // printf("grid_row[%d]: %s\n", i, cpy);
        strcpy(grid[i], cpy);
        if (row_cnt == GRID)
        {
            // printf("row_cnt = %d\n", row_cnt);
            return true;
        }
        row_cnt = 0;
    }
    for (int i = 0; i < GRID; i++)
    {
        // printf("col_cnt[%d] = %d\n", i, col_cnt[i]);
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

int play_bingo()
{
    bool bingo = false;
    char *ptr;
    // char *rep;
    char cpy[BUFSIZE] = "";
    int head_ctr = 0;
    int row = 0;
    int nbr = 0;
    int sum = 0;
    int order = 0;
    int grid_order[1024] = {0};
    volatile bool regd = false;

    // head_length = strlen(head);

    // memcpy(gridcpy, grid, sizeof(grid));
    while (!bingo && head_ctr < head_length)
    {
        for (size_t i = 0; i < bingo_length; i++)
        {
            regd = false;
            strcpy(cpy, "");
            // if (i < 12 && i > 9)
            //     printf("head[%d]=%s in grid[%ld]=%s ?\n", head_ctr, head[head_ctr], i, grid[i]);
            // strcpy(cpy, grid[i]);
            ptr = strtok(grid[i], " \n");
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
            strcpy(grid[i], cpy);
            // printf("%d in grid_order [ ", (int)i / GRID);
            for (int k = 0; k < order; k++)
            {
                if (grid_order[k] == ((int)i / GRID))
                {
                    printf("%d == %d ", grid_order[k], (int)i / GRID);
                    regd = true;
                    break;
                }
            }
            // printf("\n");
            if (!regd)
            {
                printf("regd false\n");
                if (check_grid((int)i / GRID))
                {
                    bingo = true;
                    row = (int)i;
                    nbr = atoi(head[head_ctr]);
                    sum = sum_grid(row / GRID);
                    grid_order[order] = row / GRID;
                    order++;
                    printf("\n");
                    printf("winner row: %d\n", row);
                    printf("winner grid: %d\n", row / GRID);
                    printf("last number: %d\n", nbr);
                    printf("sum of grid: %d\n", sum);
                    printf("last x sum: %d\n", nbr * sum);
                    // break;
                }
            }
        }

        head_ctr++;
        // break;
    }
    return 0;
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
    bingo_length = 0;

    FILE *fp = open_file("Bingo file name");
    parse_file(fp, print_line);
    // parse_support_rating(fp);
    fclose(fp);

    printf("\nPLAY!\n\n");
    play_bingo();
    printf("\nDONE!\n");

    return 0;
}