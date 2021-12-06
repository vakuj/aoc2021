#include <stdio.h>
#include <stdlib.h>
#include <stdint-gcc.h>
#include <string.h>
#include <stdbool.h>

#include "../common/common.h"

#define ARRSIZE 1000000

#define TOK ",\n"

int state[ARRSIZE] = {0};
int added = 0;

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

int count_fish(void)
{
    for (size_t i = ARRSIZE - 1; i > 0; i--)
    {
        if (state[i] > 0)
        {
            return i;
        }
    }

    return 0;
}

void evolve(int days)
{
    int new_spawn;
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
        for (size_t i = added; i < added + new_spawn; i++)
        {
            state[i] = 8;
        }
        added += new_spawn;
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
    int days = 0;
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
    // printf("Initial state: \n");
    // print_state(0, added);

    days = 80;
    evolve(days);
    fp = fopen(outp, "w");
    fprintf(fp, "After %d days total of %d fish\n", days, added);
    fclose(fp);

    // days = 256;
    // evolve(days);
    // fclose(fp);
    printf("Done, see \"%s\" for result\n", outp);
    return 0;
}