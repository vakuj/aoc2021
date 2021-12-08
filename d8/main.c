#include <stdio.h>
#include <stdlib.h>
#include <stdint-gcc.h>
#include <string.h>
#include <stdbool.h>

#include "../common/common.h"

#define ARRSIZE 4096

#define NEWLN " \n"
#define SPACE " "
#define DELIM "|\n"

char pre_segs[ARRSIZE][BUFSIZE];
char post_segs[ARRSIZE][BUFSIZE];
int decoded[ARRSIZE] = {0};
int added = 0;

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

unsigned long int count_easy(int length)
{
    unsigned long int ctr = 0;
    char *ptr;
    char next[BUFSIZE] = "";
    int next_length = 0;
    for (size_t i = 0; i < length; i++)
    {
        strcpy(next, post_segs[i]);
        // printf("%ld: %s\n", i, next);
        ptr = strtok(next, SPACE);
        while (ptr != NULL)
        {
            // printf("%ld: %s\n", i, ptr);
            next_length = strlen(ptr);
            if (next_length == 2) // 1
                ctr++;
            if (next_length == 4) // 4
                ctr++;
            if (next_length == 3) // 7
                ctr++;
            if (next_length == 7) // 8
                ctr++;

            ptr = strtok(NULL, SPACE);
        }
    }
    return ctr;
}

int check_match(char *search, char *find)
{
    int ctr = 0;
    char *ptr = strpbrk(search, find);
    while (ptr != NULL)
    {
        ctr++;
        ptr = strpbrk(ptr + 1, find);
    }
    return ctr;
}

unsigned long int decode(int index)
{
    /**
     * @brief 
     *       aaa
     *      f   b
     *      f   b
     *       ggg
     *      e   c
     *      e   c
     *       ddd
     */

    unsigned long int value = 0;
    int power = 1000;
    char *ptr;
    char next[BUFSIZE] = "";
    char order[10][BUFSIZE];
    char rest[10][BUFSIZE];
    int str_len = 0;
    int ctr = 0;
    strcpy(next, pre_segs[index]);
    ptr = strtok(next, SPACE);
    // Find the easy ones
    while (ptr != NULL)
    {
        str_len = strlen(ptr);
        if (str_len == 2) // 1
            strcpy(order[1], ptr);
        else if (str_len == 4) // 4
            strcpy(order[4], ptr);
        else if (str_len == 3) // 7
            strcpy(order[7], ptr);
        else if (str_len == 7) // 8
            strcpy(order[8], ptr);
        else
        {
            strcpy(rest[ctr], ptr);
            ctr++;
        }
        ptr = strtok(NULL, SPACE);
    }
    // Find the rest
    for (size_t i = 0; i < ctr; i++)
    {
        strcpy(next, rest[i]);
        str_len = strlen(next);
        if (str_len == 5) // 2, 3 or 5
        {
            if (check_match(next, order[1]) == 2) // 3
                strcpy(order[3], rest[i]);
            else if (check_match(next, order[4]) == 3) // 5
                strcpy(order[5], rest[i]);
            else // 2
                strcpy(order[2], rest[i]);
        }
        if (str_len == 6) // 0, 6 or 9
        {
            if (check_match(next, order[4]) == 4) // 9
                strcpy(order[9], rest[i]);
            else if (check_match(next, order[1]) == 1) // 6
                strcpy(order[6], rest[i]);
            else
                strcpy(order[0], rest[i]);
        }
    }

    strcpy(next, post_segs[index]);
    ptr = strtok(next, SPACE);
    while (ptr != NULL)
    {
        str_len = strlen(ptr);
        if (str_len == 2) // 1
            value += power;
        else if (str_len == 4) // 4
            value += power * 4;
        else if (str_len == 3) // 7
            value += power * 7;
        else if (str_len == 7) // 8
            value += power * 8;
        else if (str_len == 5) // 2,3,5
        {
            if (check_match(ptr, order[2]) == 5) // 2
                value += power * 2;
            else if (check_match(ptr, order[5]) == 5) // 5
                value += power * 5;
            else // 3
                value += power * 3;
        }
        else // 0,6,9
        {
            if (check_match(ptr, order[6]) == 6) // 6
                value += power * 6;
            else if (check_match(ptr, order[9]) == 6) // 9
                value += power * 9;
            else // 0
            {
                printf("ZERO with power: %d\n", power);
                value += 0;
            }
        }
        // printf(" -> %d: %s\n", value, ptr);
        if (value > 1)
            power = power / 10;
        ptr = strtok(NULL, SPACE);
    }
    // for (size_t i = 0; i < 10; i++)
    // {
    //     printf("%2ld: %s\n", i, order[i]);
    // }
    // printf("%40s -> %ld\n", post_segs[index], value);

    return value;
}

unsigned long int decode_all(void)
{
    unsigned long int sum = 0;
    for (int i = 0; i < added; i++)
    {
        decoded[i] = decode(i);
        sum += decoded[i];
        printf("%3d: %4d -> %ld\n", i, decoded[i], sum);
        // sum += decode(i);
    }
    return sum;
}

void parse_file(FILE *fp, void func(char *, int), bool silent)
{
    char next[BUFSIZE];
    char *ptr;
    int next_ctr = 0;
    int ptr_ctr = 0;

    while (fgets(next, BUFSIZE, fp) != NULL)
    {
        ptr = strtok(next, DELIM);
        ptr_ctr = 0;
        while (ptr != NULL)
        {

            if (strlen(ptr) >= 1)
            {
                if (!silent)
                    func(ptr, ptr_ctr);
                if (ptr_ctr > 0)
                    strcpy(post_segs[next_ctr], ptr);
                else
                    strcpy(pre_segs[next_ctr], ptr);
                ptr_ctr++;
            }
            ptr = strtok(NULL, DELIM);
        }
        next_ctr++;
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
    // unsigned long int cnt1 = count_easy(added);
    // printf("Counted easy post segs: %ld\n", cnt1);
    unsigned long int cnt2 = decode_all();
    printf("Decoded count post segs: %ld\n", cnt2);
    // 1149557 to high
    // fp = fopen(outp, "w");
    // fprintf(fp, "Simulated %d days -> %ld lanternfish!\n", d1, fishes1);
    // fclose(fp);
    printf("Done, see \"%s\" for result\n", outp);
    return 0;
}