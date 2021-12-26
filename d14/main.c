#include <stdio.h>
#include <stdlib.h>
#include <stdint-gcc.h>
#include <string.h>
#include <stdbool.h>

#include "../common/common.h"

#define ARRSIZE 1024
#define XLBUF 10186
#define LUTSIZE 625

#define NEWLN "\n"
#define SPACE " "
#define DELIM " -> "

#define TMPFILE "tmp/tmp_file"

typedef struct
{
    char pair[16];
    char opt[16];
} poly_t;

poly_t poly[ARRSIZE] = {0};
char seq[ARRSIZE] = "";

size_t counted[25] = {0};
size_t pair_ctr[25][25] = {0};
bool inseq[25][25] = {0};

int added = 0;

void print_line(char *line, int line_nbr)
{
    printf("%d : %s\n", line_nbr, line);
}

void print_input(void)
{
    printf("Current sequence: %s\n", seq);
    for (size_t i = 0; i < added; i++)
    {
        printf("%3s: %2s\n", poly[i].pair, poly[i].opt);
    }
}

bool is_tracked(const char *p)
{
    return inseq[(int)p[0] - 65][(int)p[1] - 65];
}

void set_tracked(bool arr[25][25], const char *p)
{
    arr[(int)p[0] - 65][(int)p[1] - 65] = true;
}

void add_pair_ctr(size_t arr[25][25], const char *p, const size_t cnt)
{
    arr[(int)p[0] - 65][(int)p[1] - 65] += cnt;
}

size_t get_pair_ctr(const size_t arr[25][25], const char *p)
{
    return arr[(int)p[0] - 65][(int)p[1] - 65];
}

void add_counted(const char *p, const size_t cnt)
{
    counted[p[0] - 'A'] += cnt;
    counted[p[1] - 'A'] += cnt;
}

void set_counted(const char *p, const size_t cnt)
{
    counted[p[0] - 'A'] = cnt;
    counted[p[1] - 'A'] = cnt;
}

void update_pair_ctr(int iters)
{
    char p[16] = "";
    char opt[16] = "";
    char np[16] = "";
    size_t cnt = 0;
    int i = 0;

    size_t new_ctr[25][25] = {0};
    bool new_track[25][25] = {0};

    /** Set or reset to default values */
    memset(counted, 0, sizeof(size_t[25]));
    memset(pair_ctr, 0, sizeof(size_t[25][25]));
    memset(inseq, 0, sizeof(bool[25][25]));
    memset(new_ctr, 0, sizeof(size_t[25][25]));
    memset(new_track, 0, sizeof(bool[25][25]));

    /** Add initial sequence to pair counter */
    for (size_t i = 0; i < strlen(seq) - 1; i++)
    {
        memmove(p, seq + i, 2);
        set_tracked(inseq, p);
        add_pair_ctr(pair_ctr, p, 1);
    }

    while (iters > 0)
    {
        i = 0;
        while (i < added)
        {
            /** If pair is tracked update it */
            if (is_tracked(poly[i].pair))
            {
                /** Get previous count */
                cnt = get_pair_ctr(pair_ctr, poly[i].pair);
                if (cnt == 0)
                    cnt = 1;

                /** Find new pairs */
                strncpy(opt, poly[i].pair, 1);
                strncpy(opt + 1, poly[i].opt, 1);
                strncpy(opt + 2, poly[i].pair + 1, 1);

                /** Add first pair */
                memmove(np, opt, 2);
                set_tracked(new_track, np);
                add_pair_ctr(new_ctr, np, cnt);
                /** Add second pair */
                memmove(np, opt + 1, 2);
                set_tracked(new_track, np);
                add_pair_ctr(new_ctr, np, cnt);
            }
            i++;
        }
        /** Move new pairs to for next iter */
        memmove(pair_ctr, new_ctr, sizeof(size_t[25][25]));
        memmove(inseq, new_track, sizeof(bool[25][25]));
        /** Reset for next iter */
        memset(new_ctr, 0, sizeof(size_t[25][25]));
        memset(new_track, 0, sizeof(bool[25][25]));

        iters--;
    }
    /** Move pair counters to letter counters */
    for (size_t i = 0; i < added; i++)
    {
        cnt = get_pair_ctr(pair_ctr, poly[i].pair);
        add_counted(poly[i].pair, cnt);
    }
    /** Fix counts for doublets, e.g. divide by 2
     *  If count is not even divide by 2 and add 1, e.g. round up
     */
    for (size_t i = 0; i < 26; i++)
    {
        if (counted[i] % 2 == 0)
            counted[i] = counted[i] / 2;
        else
            counted[i] = counted[i] / 2 + 1;
    }
}

size_t get_result(bool enable_print)
{
    size_t min = 0,
           max = 0,
           res = 0,
           total = 0;
    for (size_t i = 0; i < 26; i++)
    {
        if (min == 0 && counted[i] != 0)
            min = counted[i];

        if (counted[i] < min && counted[i] != 0)
            min = counted[i];

        if (max == 0 || counted[i] > max)
            max = counted[i];
        total += counted[i];
    }
    res = max - min;

    if (enable_print)
    {
        printf("--- Stats ---\n");
        printf("Tot cnt: %ld\nMax cnt: %ld\nMin cnt: %ld\nMax-Min: %ld\n", total, max, min, res);
        printf("=============\n");
    }

    return res;
}

void parse_file(FILE *fp, void func(char *, int), bool silent)
{
    char next[BUFSIZE];
    char *ptr;
    int next_ctr = 0;
    memset(poly, 0, sizeof(poly));

    while (fgets(next, BUFSIZE, fp) != NULL)
    {
        ptr = strtok(next, NEWLN);
        if (next_ctr == 0)
            strcpy(seq, ptr);
        else
        {
            ptr = strtok(ptr, DELIM);
            if (ptr != NULL)
            {
                strcpy(poly[added].pair, ptr);
                ptr = strtok(NULL, DELIM);
                strcpy(poly[added].opt, ptr);
                added++;
            }
        }

        next_ctr++;
    }
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

    update_pair_ctr(10);
    size_t pt1 = get_result(false);
    update_pair_ctr(40);
    size_t pt2 = get_result(false);

    fp = fopen(outp, "w");
    fprintf(fp, "Difference between max and min count:\n");
    fprintf(fp, "Part 1: %ld\n", pt1);
    fprintf(fp, "Part 2: %ld\n", pt2);
    fclose(fp);
    printf("Done, see \"%s\" for result\n", outp);
    return 0;
}