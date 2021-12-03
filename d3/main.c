#include <stdio.h>
#include <stdlib.h>
#include <stdint-gcc.h>
#include <string.h>

#define BUFSIZE 255

#define UP "up"
#define DOWN "down"
#define FORW "forward"

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
    int gamma = 0, epsilon = 0;
    int oxy = 0, co2 = 0;
    char tmp[BUFSIZE] = "";
    FILE *fp = open_file("Diagnostic file name");
    parse_file(fp, parse_bin_line);
    parse_support_rating(fp);
    fclose(fp);

    for (size_t i = 0; i < bins; i++)
    {
        if (zeros[i] > ones[i])
        {
            epsilon |= (1 << (bins - i - 1));
        }
        if (ones[i] > zeros[i])
        {
            gamma |= (1 << (bins - i - 1));
        }

        tmp[0] = oxy_str[i];
        if (strcmp(tmp, ONE) == 0)
            oxy |= (1 << (bins - i - 1));
        tmp[0] = co2_str[i];
        if (strcmp(tmp, ONE) == 0)
            co2 |= (1 << (bins - i - 1));
    }

    printf("gamma: %d\nepsilon: %d\n", gamma, epsilon);
    printf("gamma x epsilon: %d\n", gamma * epsilon);

    printf("oxy: %d\nco2: %d\n", oxy, co2);
    printf("oxy x co2: %d\n", oxy * co2);

    return 0;
}