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

#define TOK " ,->\n"

int grid[GRID][GRID] = {0};
int coor[GRID][4] = {0};
int max_coor = 0;
int nbr_coor = 0;

void print_line(char *line, int line_nbr)
{
    printf("%3d : %s\n", line_nbr, line);
}

void print_coor(int start, int end)
{
    for (int i = start; i < end; i++)
    {
        printf("(X1,Y1),(X2,Y2): (%d,%d),(%d,%d)\n", coor[i][X1], coor[i][Y1], coor[i][X2], coor[i][Y2]);
    }
}

void print_vents(int start, int stop)
{
    for (int i = start; i < stop; i++)
    {
        for (int j = start; j < stop; j++)
        {
            printf("%1d ", grid[j][i]);
        }
        printf("\n");
    }
}

int check_valid(int loc, bool diag)
{
    /** check if valid move
     *  returns "move type" 
     * -1: Invalid "move type", should be ignored
     *  0: single dot, no iteration required
     *  1: move only in x-dir
     *  2: move only in y-dir
     *  3: move on diagonal (move in both x-dir and y-dir)
     *
     */

    if ((coor[loc][X1] == coor[loc][X2]) && (coor[loc][Y1] == coor[loc][Y2]))
        return 0;
    if ((coor[loc][X1] != coor[loc][X2]) && (coor[loc][Y1] == coor[loc][Y2]))
        return 1;
    if ((coor[loc][X1] == coor[loc][X2]) && (coor[loc][Y1] != coor[loc][Y2]))
        return 2;
    if ((coor[loc][X1] != coor[loc][X2]) && (coor[loc][Y1] != coor[loc][Y2]))
    {
        if (diag)
            return 3;
        return -1;
    }

    return -1;
}

int count_vents(int lim)
{
    int ctr = 0;
    for (int i = 0; i < max_coor; i++)
    {
        for (int j = 0; j < max_coor; j++)
        {
            if (grid[j][i] >= lim)
                ctr++;
        }
    }
    return ctr;
}

void insert_vents(bool diag)
{
    /**
     * @brief parses coordinates and insert to grid
     * 
     * If diag false: inserts only on horizontal and vertical directions.
     * If diag true: inserts on diagonal directions as well
     * 
     */

    int move = -1,
        start = 0,
        stop = 0,
        xdir = 0,
        ydir = 0;

    for (int i = 0; i < nbr_coor; i++)
    {
        if (max_coor < coor[i][Y1])
            max_coor = coor[i][Y1] + 1;
        if (max_coor < coor[i][X1])
            max_coor = coor[i][X1] + 1;
        if (max_coor < coor[i][Y2])
            max_coor = coor[i][Y2] + 1;
        if (max_coor < coor[i][X2])
            max_coor = coor[i][X2] + 1;

        move = check_valid(i, diag);
        if (move >= 0) // valid move
        {
            if (move == 0)
            {

                grid[coor[i][X1]][coor[i][Y1]] += 1;
            }
            if (move == 1)
            {
                if (coor[i][X1] > coor[i][X2])
                {
                    start = coor[i][X2];
                    stop = coor[i][X1];
                }
                else
                {
                    start = coor[i][X1];
                    stop = coor[i][X2];
                }
                for (int k = start; k <= stop; k++)
                {
                    grid[k][coor[i][Y1]] += 1;
                }
            }
            if (move == 2)
            {
                if (coor[i][Y1] > coor[i][Y2])
                {
                    start = coor[i][Y2];
                    stop = coor[i][Y1];
                }
                else
                {
                    start = coor[i][Y1];
                    stop = coor[i][Y2];
                }
                for (int k = start; k <= stop; k++)
                {
                    grid[coor[i][X1]][k] += 1;
                }
            }
            if (move == 3)
            {
                if (coor[i][Y1] > coor[i][Y2])
                {
                    start = coor[i][Y2];
                    stop = coor[i][Y1];
                    ydir = -1;
                }
                else
                {
                    start = coor[i][Y1];
                    stop = coor[i][Y2];
                    ydir = +1;
                }
                if (coor[i][X1] > coor[i][X2])
                    xdir = -1;
                else
                    xdir = 1;

                for (int k = 0; k <= stop - start; k++)
                {
                    grid[coor[i][X1] + (xdir * k)][coor[i][Y1] + (ydir * k)] += 1;
                }
            }
        }
    }
}

void parse_file(FILE *fp, void func(char *, int), bool silent)
{
    char next[BUFSIZE];
    char *ptr;
    int next_ctr = 0;
    int coor_ctr = 0;

    while (fgets(next, BUFSIZE, fp) != NULL)
    {
        coor_ctr = 0;
        ptr = strtok(next, TOK);
        while (ptr != NULL)
        {
            if (strlen(ptr) >= 1)
            {
                if (!silent)
                    func(next, next_ctr);
                coor[next_ctr][coor_ctr] = atoi(ptr);
                coor_ctr++;
            }
            ptr = strtok(NULL, TOK);
        }
        next_ctr++;
    }
    nbr_coor = next_ctr;
}

int main(int argc, char *argv[])
{
    char outp[BUFSIZE] = "output";
    FILE *fp;
    int nbr_vents1 = 0;
    int nbr_vents2 = 0;
    int danger_lim = 2;
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

    parse_file(fp, print_line, true);
    fclose(fp);

    insert_vents(false);
    nbr_vents1 = count_vents(danger_lim);

    /** clear prev grid for safety*/
    memset(grid, 0, sizeof(grid));

    insert_vents(true);
    nbr_vents2 = count_vents(danger_lim);

    fp = fopen(outp, "w");
    fprintf(fp, "Part 1: (>= %d)\nDanger vents: %d \n\n", danger_lim, nbr_vents1);
    fprintf(fp, "Part 2: (>= %d)\nDanger vents: %d \n", danger_lim, nbr_vents2);
    fclose(fp);
    printf("Done, see \"%s\" for result\n", outp);

    return 0;
}