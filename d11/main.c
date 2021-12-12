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
    int y;
} coord_t;

int energy[ARRSIZE][ARRSIZE] = {0};
coord_t sites[ARRSIZE];

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
            printf("%3d", energy[i][j]);
        }
        printf("\n");
    }
}

void update_region(coord_t origin)
{
    // update only around origin
    for (int i = origin.y - 1; i < origin.y + 2; i++)
    {
        for (int j = origin.x - 1; j < origin.x + 2; j++)
        {
            if (i >= 0 && j >= 0 && i < ylen && j < xlen)
                energy[i][j] += 1;
        }
    }
}

void update_energy(void)
{
    // update entire map
    for (size_t i = 0; i < ylen; i++)
    {
        for (size_t j = 0; j < xlen; j++)
        {
            energy[i][j] += 1;
        }
    }
}

bool has_flashed(coord_t origin, int flashes)
{
    // ensure only one flash per cycle
    if (flashes == 0)
        return false;
    for (size_t i = 0; i < flashes; i++)
    {
        if (sites[i].x == origin.x && sites[i].y == origin.y)
            return true;
    }
    return false;
}

int check_flash(void)
{
    int flashes = 0;
    coord_t origin;
    origin.x = 0;
    origin.y = 0;
    bool flashed = true;

    while (flashed) // iterate until no more flashes
    {
        flashed = false; // assume no flash
        for (size_t i = 0; i < ylen; i++)
        {
            for (size_t j = 0; j < xlen; j++)
            {
                if (energy[i][j] > 9)
                {
                    origin.y = i;
                    origin.x = j;
                    if (!has_flashed(origin, flashes))
                    {
                        update_region(origin);
                        sites[flashes] = origin;
                        flashed = true;
                        flashes++;
                    }
                }
            }
        }
        for (size_t i = 0; i < flashes; i++)
        {
            energy[sites[i].y][sites[i].x] = 0;
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
        flashes += check_flash();
        ctr++;
    }
    return flashes;
}

int find_sync(int start_iter)
{
    int ctr = start_iter;
    int flashes = 0;
    while (flashes != xlen * ylen)
    {
        update_energy();
        flashes = check_flash();
        ctr++;
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
    int flashes = evolve_energy(100);
    int sync_iter = find_sync(100);
    fp = fopen(outp, "w");
    fprintf(fp, "Number of flashes after 100 iters: %d\n", flashes);
    fprintf(fp, "Synchronization achieved at: %d\n", sync_iter);
    fclose(fp);
    printf("Done, see \"%s\" for result\n", outp);
    return 0;
}