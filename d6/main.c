#include <stdio.h>
#include <stdlib.h>
#include <stdint-gcc.h>
#include <string.h>
#include <stdbool.h>

#include "../common/common.h"

#define ARRSIZE 1024

#define TOK ",\n"

int state[ARRSIZE] = {0};
unsigned long int spawn[ARRSIZE] = {0};
unsigned long int added = 0;

void print_line(char *line, int line_nbr)
{
    printf("%d : %s\n", line_nbr, line);
}

void print_state(int start, int end)
{
    for (size_t i = start; i < end; i++)
    {
        printf("%d, ", state[i]);
    }
    printf("\n");
}

unsigned long int count_fish(int days)
{
    unsigned long int fish = added;
    for (size_t i = 0; i < days; i++)
    {
        fish += spawn[i];
    }
    return fish;
}

volatile void evolve(int days)
{
    int new_spawn;
    int temp_state = 0;
    for (size_t itr = 0; itr < days; itr++)
    {
        new_spawn = 0;
        for (size_t i = 0; i < added; i++)
        {
            state[i]--;
            if (state[i] == -1)
            {
                state[i] = 6;
                new_spawn++;
            }
        }
        spawn[itr] = new_spawn;
    }
    for (size_t i = 0; i < days; i++)
    {
        if (spawn[i] > 0)
        {
            temp_state = 8;
            for (size_t j = i + 1; j < days; j++)
            {
                temp_state--;
                if (temp_state == -1)
                {
                    temp_state = 6;
                    spawn[j] += spawn[i];
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
    int ptr_ctr = 0;

    while (fgets(next, BUFSIZE, fp) != NULL)
    {
        ptr = strtok(next, TOK);
        while (ptr != NULL)
        {
            if (strlen(ptr) >= 1)
            {
                if (!silent)
                    func(ptr, ptr_ctr);
                state[ptr_ctr] = atoi(ptr);
                ptr_ctr++;
            }
            ptr = strtok(NULL, TOK);
        }
        next_ctr++;
        added = ptr_ctr;
    }
}

int main(int argc, char *argv[])
{
    char outp[BUFSIZE] = "output";
    FILE *fp;
    int d1 = 80;
    unsigned long int fishes1;
    if (argc > 4)
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
    if (argc == 4)
    {
        fp = open_file("", argv[1]);
        strcpy(outp, argv[2]);
        d1 = atoi(argv[3]);
    }

    parse_file(fp, print_line, true);
    fclose(fp);

    evolve(d1);
    fishes1 = count_fish(d1);

    fp = fopen(outp, "a+");
    fprintf(fp, "Simulated %d days -> %ld lanternfish!\n", d1, fishes1);
    fclose(fp);
    printf("Done, see \"%s\" for result\n", outp);
    return 0;
}