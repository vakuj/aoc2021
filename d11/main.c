#include <stdio.h>
#include <stdlib.h>
#include <stdint-gcc.h>
#include <string.h>
#include <stdbool.h>

#include "../common/common.h"

#define ARRSIZE 128

#define NEWLN " \n"
#define SPACE " "
#define SCANCHR "0123456789"

typedef struct
{
    int x;
    int y
} coord_t;

int energy[ARRSIZE][ARRSIZE] = {0};
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
            printf("%2d", energy[i][j]);
        }
        printf("\n");
    }
}

void update_region(coord_t origin)
{
    return;
}

void update_energy(void)
{
    for (size_t i = 0; i < ylen; i++)
    {
        for (size_t j = 0; j < xlen; j++)
        {
            energy[i][j] += 1;
        }
    }
}

int check_flash(void)
{
    int flashes = 0;
    coord_t origin;
    origin.x = 0;
    origin.y = 0;
    bool flashed = true;

    while (flashed)
    {
        flashed = false;
        for (size_t i = 0; i < xlen; i++)
        {
            for (size_t j = 0; j < ylen; i++)
            {
                if (energy[i][j] > 9)
                {
                    // RECORD FLASHED?!?
                    update_region(origin);
                    flashed = true;
                    flashes++;
                }
            }
        }
    }
    return flashes;
}

int evolve_energy(int max_iter)
{
    int ctr = 0;
    int flashes = 0;
    while (ctr < max_iter)
    {
        update_energy();
        check_flash();
        ctr++;
    }
}

void fill_border(void)
{
    for (size_t i = 0; i <= ylen; i++)
    {
        for (size_t j = 0; j <= xlen; j++)
        {
            if (i == 0)
                energy[i][j] = 9;
            else if (i == ylen)
                energy[i][j] = 9;
            else if (j == 0)
                energy[i][j] = 9;
            else if (j == xlen)
                energy[i][j] = 9;
        }
    }
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
            energy[next_ctr][ptr_ctr] = *ptr - 48;
            ptr = strpbrk(ptr + 1, SCANCHR);
            ptr_ctr++;
        }

        next_ctr++;
    }
    ylen = next_ctr;
    // fill_border();
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

    fp = fopen(outp, "w");
    fprintf(fp, "Part 1 solution not added yet\n");
    fprintf(fp, "Part 2 solution not added yet\n");
    fclose(fp);
    printf("Done, see \"%s\" for result\n", outp);
    return 0;
}