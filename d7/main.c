#include <stdio.h>
#include <stdlib.h>
#include <stdint-gcc.h>
#include <string.h>
#include <stdbool.h>

#include "../common/common.h"

#define ARRSIZE 4096

#define TOK " ,\n"

unsigned long int state[ARRSIZE] = {0};
unsigned long int fuel_costs[ARRSIZE] = {0};
unsigned long int added = 0;
unsigned long int arr_max, arr_min, arr_ave;

void print_line(char *line, int line_nbr)
{
    printf("%d : %s\n", line_nbr, line);
}

void print_arr(unsigned long int *arr, int start, int end)
{
    int line = 0;
    printf("%4d: ", line);

    for (size_t i = start; i < end; i++)
    {
        printf("%4ld, ", arr[i]);
        if ((i - start) % 10 == 9)
        {
            line += 10;
            printf("\n%4d: ", line);
        }
    }
    printf("\n");
}

unsigned long int count(int length)
{
    unsigned long int ctr = 0;
    for (size_t i = 0; i < length; i++)
    {
        ctr += state[i];
    }
    return ctr;
}

void update_stats(unsigned long int *arr, int start, int stop)
{
    unsigned long int tmp_max = 0,
                      tmp_min = 0,
                      tmp_ave = 0;
    if (arr[start] >= arr[start + 1])
    {
        tmp_max = arr[start];

        tmp_min = arr[start + 1];
    }
    else
    {
        tmp_min = arr[start];
        tmp_max = arr[start + 1];
    }
    tmp_ave += tmp_min + tmp_max;
    for (size_t i = start + 2; i < stop; i++)
    {
        if (arr[i] > tmp_max)
            tmp_max = arr[i];
        if (arr[i] < tmp_min)
            tmp_min = arr[i];
        tmp_ave += arr[i];
    }

    arr_max = tmp_max;
    arr_min = tmp_min;
    arr_ave = tmp_ave / added;
}

unsigned long int move_to(unsigned long int pos)
{
    unsigned long int fuel_cost = 0;
    for (size_t itr = 0; itr < added; itr++)
    {
        if (state[itr] >= pos)
            fuel_cost += (state[itr] - pos);
        else
            fuel_cost += (pos - state[itr]);
        // printf("%4ld -> %4ld += %4ld\n", state[itr], pos, fuel_cost);
    }
    return fuel_cost;
}

void update_fuel_cost(int start_pos, int stop_pos)
{
    for (size_t i = start_pos; i < stop_pos; i++)
    {
        fuel_costs[i] = move_to(i);
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

    parse_file(fp, print_line, true);
    fclose(fp);
    printf("added: %ld\n", added);
    print_arr(state, 0, added);
    update_stats(state, 0, added);
    printf("%s\nmin: %ld\nmax: %ld\nave: %ld\n", "state", arr_min, arr_max, arr_ave);

    update_fuel_cost(0, arr_max);
    print_arr(fuel_costs, 0, arr_max);

    update_stats(fuel_costs, 0, arr_max);
    printf("%s\nmin: %ld\nmax: %ld\nave: %ld\n", "fuel cost", arr_min, arr_max, arr_ave);
    // int tmp = move_to(331);
    // 355610 too high
    // fp = fopen(outp, "a+");
    // fprintf(fp, "Simulated %d days -> %ld lanternfish!\n", d1, fishes1);
    // fclose(fp);
    printf("Done, see \"%s\" for result\n", outp);
    return 0;
}