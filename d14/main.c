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

poly_t poly[ARRSIZE];
char seq[ARRSIZE];
size_t counted[25];
char poly_lut[LUTSIZE];
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

void init_lut(void)
{
    memset(poly_lut, 0, sizeof(poly_lut));

    for (size_t i = 0; i < added; i++)
    {
        memmove(poly_lut + (poly[i].pair[0] - 65) * 24 + (poly[i].pair[1] - 65), poly[i].opt, 1);
    }
}

char *opt_lut(char *oseq, char *p)
{
    memmove(oseq, p, 1);
    memmove(oseq + 1, poly_lut + ((int)p[0] - 65) * 24 + ((int)p[1] - 65), 1);
    memmove(oseq + 2, p + 1, 1);
    return oseq;
}

void rec_count(char *p, int ctr, int end)
{
    if (ctr == end)
    {
        return;
    }
    ctr++;

    char *oseq = (char *)calloc(0, sizeof(char) * 3);
    char *pre = (char *)calloc(0, sizeof(char) * 2);
    char *post = (char *)calloc(0, sizeof(char) * 2);

    opt_lut(oseq, p);

    counted[(int)oseq[1] - 65] += 1;

    memmove(pre, oseq, 2);
    memmove(post, oseq + 1, 2);
    free(oseq);

    rec_count(pre, ctr, end);
    free(pre);

    rec_count(post, ctr, end);
    free(post);
}

void count_seq(int end)
{
    char *ptr = (char *)calloc(0, sizeof(char) * 2);
    memset(counted, 0, sizeof(counted));
    init_lut();
    int i = 0;
    while (i < strlen(seq) - 1)
    {
        memmove(ptr, seq + i, 2);
        rec_count(ptr, 0, end);
        counted[(int)ptr[1] - 65] += 1;
        i++;
    }
    counted[(int)seq[0] - 65] += 1;
    free(ptr);
}

size_t get_result(void)
{
    size_t min = 0,
           max = 0,
           total = 0;
    for (size_t i = 0; i < 25; i++)
    {
        if (min == 0 && counted[i] != 0)
            min = counted[i];

        if (counted[i] < min && counted[i] != 0)
            min = counted[i];

        if (max == 0 || counted[i] > max)
            max = counted[i];
        total += counted[i];
    }
    printf("--- Stats ---\n");
    printf("Tot cnt: %ld\nMax cnt: %ld\nMin cnt: %ld\nMax-Min: %ld\n", total, max, min, max - min);
    printf("=============\n");

    return max - min;
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

size_t count_poly(int fnbr)
{
    FILE *fp;
    char fname[BUFSIZE];
    size_t counts['Z' - 'A'];
    int nchar;
    size_t min = 0, max = 0, total = 0;
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
    printf("--- Stats for %s%d ---\n", TMPFILE, fnbr);
    printf("Tot cnt: %ld\nMax cnt: %ld\nMin cnt: %ld\nMax-Min: %ld\n", total, max, min, max - min);
    printf("=======\n");

    return max - min;
}

void update_seq(int start, int stop)
{
    FILE *fpr, *fpw;
    char read_file[BUFSIZE], write_file[BUFSIZE];
    fpos_t pos;

    int rwbuf = 2000000;
    // char rbuf[rwbuf], wbuf[rwbuf];
    char *rbuf = (char *)calloc(rwbuf, sizeof(char));
    char *wbuf = (char *)calloc(rwbuf, sizeof(char));

    init_lut();

    char p[16] = "";
    char op[16] = "";
    char *ptr;
    int ctr = start;
    int rcnt = 0;
    size_t i = 0;

    /** Put init sequence to file
     *  if start is 0 else assume
     *  sequence already started
     *  and file exists.
     */
    if (ctr == 0)
    {
        sprintf(read_file, "%s%d", TMPFILE, ctr);
        fpr = fopen(read_file, "w+");
        fwrite(seq, sizeof(char), strlen(seq), fpr);
        fclose(fpr);
    }

    /** Init r/w buffers */
    memset(wbuf, 0, sizeof(char[rwbuf]));
    memset(rbuf, 0, sizeof(char[rwbuf]));

    while (ctr < stop)
    {
        /** Init r/w files */
        sprintf(read_file, "%s%d", TMPFILE, ctr);
        sprintf(write_file, "%s%d", TMPFILE, ctr + 1);
        fpr = fopen(read_file, "r");
        fpw = fopen(write_file, "w+");
        rewind(fpr);
        rewind(fpw);

        /** Read to read buffer */
        fread(rbuf, sizeof(char), rwbuf / 2 - 3, fpr);
        rcnt = 0;
        while (true)
        {
            i = 0;
            /** Process read buffer */
            while (i < strlen(rbuf) - 1)
            {
                memmove(p, rbuf + i, 2);
                // ptr = opt_seq(op, p);
                ptr = opt_lut(op, p);

                if (ptr != NULL)
                {
                    if (i == 0) // Use all three from new sequence
                        memmove(wbuf + strlen(wbuf), ptr, 3);
                    else // only use the last two from sequence (first already added)
                        memmove(wbuf + strlen(wbuf), ptr + 1, 2);
                }
                else
                    printf("Null pointer encountered, %s not found...\n", p);
                i += 1;
            }
            /** Write write buffer to file.
             *  Write the entire buffer on first call.
             *  Else: Write all except 0th index.
             */
            if (rcnt == 0)
            {
                fwrite(wbuf, sizeof(char), strlen(wbuf), fpw);
                rcnt = 1;
            }
            else
                fwrite(wbuf + 1, sizeof(char), strlen(wbuf) - 1, fpw);

            /** Reset r/w buffers */
            memset(wbuf, 0, sizeof(char[rwbuf]));
            memset(rbuf, 0, sizeof(char[rwbuf]));

            /** Try read more from file */
            if (feof(fpr) != 0)
            {
                break;
            }
            else
            {
                fseek(fpr, -1, SEEK_CUR);
                fread(rbuf, sizeof(char), rwbuf / 2 - 3, fpr);
            }
        }
        fgetpos(fpw, &pos);
        /** Close files */
        fclose(fpr);
        fclose(fpw);

        /** Prgress indicator */
        printf("%d: %ld\n", ctr, pos.__pos);

        ctr++;
    }
    free(wbuf);
    free(rbuf);
}

void update_seq2(int iters)
{
    // char buf[8196] = "";
    // char nseq[8196] = "";
    // char pseq[8196] = "";
    size_t bufsize = 200000000;
    char *buf = (char *)calloc(bufsize, sizeof(char)),
         *nseq = (char *)calloc(bufsize, sizeof(char)),
         *pseq = (char *)calloc(bufsize, sizeof(char));

    init_lut();
    char p[16] = "";
    char op[16] = "";
    char *ptr;
    int i = 0;
    int j = 0;
    int ctr = 0;
    int offset = 0;

    while (i < strlen(seq) - 1)
    {
        ctr = 0;

        memmove(buf, seq + i, 2);
        // printf("buf: %s -> %s\n", buf, nseq);
        while (ctr < iters)
        {
            j = 0;
            // printf("%s\n", buf);
            while (j < strlen(buf) - 1)
            {
                memmove(p, buf + j, 2);
                // ptr = opt_seq(op, p);
                ptr = opt_lut(op, p);
                if (ptr != NULL)
                {
                    if (i == 0 && j == 0)
                        memmove(pseq, ptr, 3);
                    else
                        memmove(pseq + strlen(pseq), ptr + 1, 2);
                }
                j++;
            }
            memmove(buf, pseq, strlen(pseq));
            memset(pseq, 0, sizeof(char) * bufsize);
            printf("%d: %ld\n", ctr, strlen(buf));
            ctr++;
        }

        memmove(nseq + strlen(nseq), buf, strlen(buf));
        memset(buf, 0, sizeof(char) * bufsize);
        break;
        i++;
    }
    free(buf);
    free(nseq);
    free(pseq);
    // printf("buf: %s -> %s\n", seq, nseq);
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

    count_seq(10);
    size_t pt1 = get_result();
    count_seq(40);
    size_t pt2 = get_result();

    for (size_t i = 0; i < 26; i++)
    {
        printf("%c: %ld\n", (char)(i + 65), counted[i]);
    }

    // fp = fopen(outp, "w");
    // fprintf(fp, "Part 1:Diff between most and least: %ld\n", pt1);
    // fprintf(fp, "Part 1:Diff between most and least: %ld\n", pt2);
    // fclose(fp);
    printf("Done, see \"%s\" for result\n", outp);
    return 0;
}