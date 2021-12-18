#include <stdio.h>
#include <stdlib.h>
#include <stdint-gcc.h>
#include <string.h>
#include <stdbool.h>

#include "../common/common.h"

#define ARRSIZE 1024
#define XLBUF 10186

#define NEWLN "\n"
#define SPACE " "
#define DELIM " -> "

#define TMPFILE "tmp/tmp_file"

typedef struct
{
    char pair[16];
    char opt[16];
} poly_t;

poly_t poly[ARRSIZE];
char seq[XLBUF];
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

char *opt_seq(char *oseq, char *p)
{
    for (size_t i = 0; i < added; i++)
    {
        if (strcmp(poly[i].pair, p) == 0)
        {
            memmove(oseq, poly[i].pair, 1);
            memmove(oseq + 1, poly[i].opt, 1);
            memmove(oseq + 2, poly[i].pair + 1, 1);
            return oseq;
        }
    }
    return (char *)NULL;
}

int count_poly(int fnbr)
{
    FILE *fp;
    char fname[BUFSIZE];
    int counts['Z' - 'A'];
    int nchar;
    int min = 0, max = 0, total = 0;
    memset(counts, 0, sizeof(counts));

    sprintf(fname, "%s%d", TMPFILE, fnbr);
    fp = fopen(fname, "r");
    nchar = fgetc(fp);
    while (nchar != EOF)
    {
        counts[nchar - 'A'] += 1;
        nchar = fgetc(fp);
    }
    fclose(fp);
    for (size_t i = 0; i < ('Z' - 'A'); i++)
    {
        if (min == 0 && counts[i] != 0)
            min = counts[i];

        if (counts[i] < min && counts[i] != 0)
            min = counts[i];

        if (max == 0 || counts[i] > max)
            max = counts[i];
        total += counts[i];
    }
    // printf("Total: %d\nMax: %d\nMin: %d\ndiff: %d\n", total, max, min, max - min);
    return max - min;
}

void update_seq(int iters)
{
    FILE *fpr, *fpw;
    char read_file[BUFSIZE], write_file[BUFSIZE];
    fpos_t pos;

    char p[16] = "";
    char op[16] = "";
    char *ptr;
    int ctr = 0;
    int i = 0;

    sprintf(read_file, "%s%d", TMPFILE, ctr);
    fpr = fopen(read_file, "w+");
    fwrite(seq, sizeof(char), strlen(seq), fpr);
    fclose(fpr);

    while (ctr < iters)
    {
        sprintf(read_file, "%s%d", TMPFILE, ctr);
        sprintf(write_file, "%s%d", TMPFILE, ctr + 1);
        fpr = fopen(read_file, "r");
        fpw = fopen(write_file, "w+");
        rewind(fpr);
        rewind(fpw);

        i = 0;

        fread(p, sizeof(char), 2, fpr);
        while (feof(fpr) == 0 && ferror(fpr) == 0)
        {
            ptr = opt_seq(op, p);
            if (ptr != NULL)
            {
                if (i == 0) // Use all three from new sequence
                {
                    fwrite((void *)ptr, sizeof(char), 3, fpw);
                }
                else // only use the last two from sequence (first already added)
                {
                    fwrite((void *)(ptr + 1), sizeof(char), 2, fpw);
                }
            }
            else
                printf("Null pointer encountered, %s not found...\n", p);
            i += 1;
            fseek(fpr, -1, SEEK_CUR);
            fread(p, sizeof(char), 2, fpr);
        }
        fgetpos(fpw, &pos);
        fclose(fpr);
        fclose(fpw);

        printf("%d: %ld\n", ctr, pos.__pos);

        ctr++;
    }
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

    update_seq(10);

    int pt1 = count_poly(10);

    fp = fopen(outp, "w");
    fprintf(fp, "Part 1:Diff between most and least: %d\n", pt1);
    fprintf(fp, "Part 2 not solved yet.\n");
    fclose(fp);
    printf("Done, see \"%s\" for result\n", outp);
    return 0;
}