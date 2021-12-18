#include <stdio.h>
#include <stdlib.h>
#include <stdint-gcc.h>
#include <string.h>
#include <stdbool.h>

#include "../common/common.h"

#define ARRSIZE 2024

#define NEWLN " \n"
#define SPACE " "
#define DELIM ","
#define FOLD '='

typedef struct
{
    int x;
    int y;
} coord_t;

coord_t dots[ARRSIZE];
int xdots[ARRSIZE];
int ydots[ARRSIZE];

char folds[BUFSIZE][BUFSIZE];
int ymax, xmax;
int coords;
int added = 0;
int grid[ARRSIZE][ARRSIZE] = {0};

void print_line(char *line, int line_nbr)
{
    printf("%d : %s\n", line_nbr, line);
}

void print_grid()
{
    printf("-------\n");
    for (size_t i = 0; i <= ymax; i++)
    {
        for (size_t j = 0; j <= xmax; j++)
        {
            if (grid[i][j] > 0)
                printf("#");
            else
                printf(".");
        }
        printf("\n");
    }
    printf("-------\n");
}
void insert_grid(void)
{
    memset(grid, 0, sizeof(grid));
    for (size_t i = 0; i < coords; i++)
    {
        grid[dots[i].y][dots[i].x] += 1;
    }
}

void grid_dots(void)
{
    memset(dots, 0, sizeof(dots));
    coords = 0;
    xmax = 0;
    ymax = 0;
    for (size_t i = 0; i < ARRSIZE; i++)
    {
        for (size_t j = 0; j < ARRSIZE; j++)
        {
            if (grid[i][j] > 0)
            {
                dots[coords].y = i;
                dots[coords].x = j;
                coords++;
                if (i > ymax)
                    ymax = (int)(i + 5);
                if (j > xmax)
                    xmax = (int)(j + 5);
            }
        }
    }
}

void make_fold(bool ydir, int loc)
{
    for (int i = 0; i < coords; i++)
    {
        if (ydir)
        {
            if (dots[i].y > loc)
                dots[i].y = 2 * (loc) - (dots[i].y);
        }
        else
        {
            if (dots[i].x > loc)
                dots[i].x = 2 * (loc) - (dots[i].x);
        }
    }
}
void fold_dots(int nbr)
{
    char fold[BUFSIZE] = "";
    char *ptr;
    int loc = 0;
    bool ydir = false;

    for (size_t i = 0; i < nbr; i++)
    {

        strcpy(fold, folds[i]);
        ptr = strchr(fold, FOLD);
        if (ptr != NULL)
        {
            if ((ptr - 1)[0] == 'y')
                ydir = true;
            else
                ydir = false;
            ptr = strtok(ptr, "=");
            loc = atoi(ptr);

            make_fold(ydir, loc);
        }
        else
            printf("NULL pointer in fold_dots\n");

        insert_grid();
        grid_dots();
    }
}
void parse_file(FILE *fp, void func(char *, int), bool silent)
{
    char next[BUFSIZE];
    char *ptr;
    int next_ctr = 0;
    bool fold_section = false;
    ymax = 0;
    xmax = 0;
    int tmp;

    memset(xdots, 0, sizeof(xdots));
    memset(ydots, 0, sizeof(ydots));
    memset(dots, 0, sizeof(dots));
    coords = 0;
    while (fgets(next, BUFSIZE, fp) != NULL)
    {
        ptr = strtok(next, "\n");
        if (!silent)
            func(next, 0);
        if (ptr != NULL)
        {
            if (!fold_section)
            {
                ptr = strtok(ptr, DELIM);
                tmp = atoi(ptr);
                if (tmp > xmax)
                    xmax = tmp;
                xdots[tmp] = 1;
                dots[coords].x = tmp;
                ptr = strtok(NULL, DELIM);

                tmp = atoi(ptr);
                if (tmp > ymax)
                    ymax = tmp;
                ydots[tmp] = 1;
                dots[coords].y = tmp;
                coords++;
            }
            else
            {
                strcpy(folds[next_ctr], ptr);
                next_ctr++;
            }
        }
        else
            fold_section = true;
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
    fold_dots(1);
    int first = coords;

    fold_dots(added);

    fp = fopen(outp, "w");
    fprintf(fp, "Visible after first fold: %d\n", first);
    fprintf(fp, "Activation code:\n");
    for (size_t i = 0; i <= ymax; i++)
    {
        for (size_t j = 0; j <= xmax; j++)
        {
            if (grid[i][j] > 0)
                fprintf(fp, "#");
            else
                fprintf(fp, ".");
        }
        fprintf(fp, "\n");
    }
    fprintf(fp, "\n");
    // fprintf(fp, "Part 1 not solved yet.\n");
    fclose(fp);
    printf("Done, see \"%s\" for result\n", outp);
    return 0;
}