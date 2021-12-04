#include <stdio.h>
#include <stdlib.h>
#include <stdint-gcc.h>
#include <string.h>

#include "../common/common.h"

#define ONE "1"
#define ZERO "0"

int depths = 0,
    aim = 0,
    forward = 0,
    aimed_depths = 0;

int ones[BUFSIZE] = {0},
    zeros[BUFSIZE] = {0};
int bins = 0;

char oxy_str[BUFSIZE] = "";
char co2_str[BUFSIZE] = "";

void parse_bin_line(char *line)
{
    bins = strlen(strtok(line, "\n"));
    char curr[1] = {"0"};
    for (int i = 0; i < bins; i++)
    {
        curr[0] = (char)line[i];
        if (strcmp(curr, ZERO) == 0)
            zeros[i] += 1;
        if (strcmp(curr, ONE) == 0)
            ones[i] += 1;
    }
}

void parse_file(FILE *fp, void func(char *))
{
    char next[BUFSIZE];
    while (fgets(next, BUFSIZE, fp) != NULL)
    {
        func(next);
    }
}

void parse_support_rating(FILE *fp)
{
    char next[BUFSIZE];
    rewind(fp);
    fgets(next, BUFSIZE, fp);
    bins = strlen(strtok(next, "\n"));
    char curr[BUFSIZE] = "";
    char prev[BUFSIZE] = "";
    int co2_zeros, co2_ones, co2_ctr;
    int oxy_zeros, oxy_ones, oxy_ctr;

    for (int i = 0; i < bins; i++)
    {
        co2_zeros = 0;
        co2_ones = 0;
        co2_ctr = 0;
        oxy_zeros = 0;
        oxy_ones = 0;
        oxy_ctr = 0;
        rewind(fp);
        while (fgets(next, BUFSIZE, fp) != NULL)
        {

            strtok(next, "\n");
            curr[0] = (char)next[i];
            strncpy(prev, next, i);
            if (strlen(oxy_str) > 0)
            {
                if (strcmp(prev, oxy_str) == 0)
                {
                    oxy_ctr++;
                    if (strcmp(curr, ZERO) == 0)
                        oxy_zeros++;
                    if (strcmp(curr, ONE) == 0)
                        oxy_ones++;
                }
                if (strcmp(prev, co2_str) == 0)
                {
                    co2_ctr++;
                    if (strcmp(curr, ZERO) == 0)
                        co2_zeros++;
                    if (strcmp(curr, ONE) == 0)
                        co2_ones++;
                }
            }
            else
            {
                oxy_ctr++;
                co2_ctr++;
                if (strcmp(curr, ZERO) == 0)
                {
                    oxy_zeros++;
                    co2_zeros++;
                }
                if (strcmp(curr, ONE) == 0)
                {
                    oxy_ones++;
                    co2_ones++;
                }
            }
        }
        if (oxy_ctr > 1)
        {
            if (oxy_zeros > oxy_ones)
                strcat(oxy_str, ZERO);
            else
                strcat(oxy_str, ONE);
        }
        else
        {
            if (oxy_ones > 0)
                strcat(oxy_str, ONE);
            if (oxy_zeros > 0)
                strcat(oxy_str, ZERO);
        }
        if (co2_ctr > 1)
        {
            if (co2_zeros <= co2_ones)
                strcat(co2_str, ZERO);
            else
                strcat(co2_str, ONE);
        }
        else
        {
            if (co2_ones > 0)
                strcat(co2_str, ONE);
            if (co2_zeros > 0)
                strcat(co2_str, ZERO);
        }
    }
}

int main(int argc, char *argv[])
{
    FILE *fp;
    char outp[BUFSIZE] = "output";
    int gamma = 0, epsilon = 0;
    int oxy = 0, co2 = 0;
    char tmp[BUFSIZE] = "";

    if (argc > 3)
    {
        perror("Usages:\t./<name>\n\t./<name> <input file>\n\t./<name> <input file> <output file>\n");
        exit(EXIT_FAILURE);
    }
    if (argc == 1)
        fp = open_file("Diagnostic file name", NULL);
    if (argc == 2)
        fp = open_file("", argv[1]);
    if (argc == 3)
    {
        fp = open_file("", argv[1]);
        strcpy(outp, argv[2]);
    }

    parse_file(fp, parse_bin_line);
    parse_support_rating(fp);
    fclose(fp);

    for (size_t i = 0; i < bins; i++)
    {
        if (zeros[i] > ones[i])
            epsilon |= (1 << (bins - i - 1));
        if (ones[i] > zeros[i])
            gamma |= (1 << (bins - i - 1));

        tmp[0] = oxy_str[i];
        if (strcmp(tmp, ONE) == 0)
            oxy |= (1 << (bins - i - 1));
        tmp[0] = co2_str[i];
        if (strcmp(tmp, ONE) == 0)
            co2 |= (1 << (bins - i - 1));
    }

    fp = fopen(outp, "w");
    fprintf(fp, "Part 1:\n\tgam: %d\n\teps: %d\n", gamma, epsilon);
    fprintf(fp, "  gam x eps: %d\n", gamma * epsilon);
    fprintf(fp, "\nPart 2:\n\toxy: %d\n\tco2: %d\n", oxy, co2);
    fprintf(fp, "  oxy x co2: %d\n", oxy * co2);
    fclose(fp);
    return 0;
}