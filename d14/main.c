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

void update_seq(int start, int stop)
{
    FILE *fpr, *fpw;
    char read_file[BUFSIZE], write_file[BUFSIZE];
    fpos_t pos;

    int rwbuf = 8196;
    char rbuf[rwbuf], wbuf[rwbuf];

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

    while (start < stop)
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
                ptr = opt_seq(op, p);

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
}

void update_seq2(int iters)
{
    char buf[8196] = "";
    char nseq[8196] = "";
    char pseq[8196] = "";

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
        printf("buf: %s -> %s\n", buf, nseq);
        while (ctr < iters)
        {
            j = 0;
            printf("%s\n", buf);
            while (j < strlen(buf) - 1)
            {
                memmove(p, buf + j, 2);
                ptr = opt_seq(op, p);
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
            memset(pseq, 0, sizeof(pseq));
            printf("%d: %ld\n", ctr, strlen(buf));
            ctr++;
        }

        memmove(nseq + strlen(nseq), buf, strlen(buf));
        memset(buf, 0, sizeof(buf));
        break;
        i++;
    }
    printf("buf: %s -> %s\n", seq, nseq);
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
    update_seq2(12);
    // update_seq(0, 40);
    // printf("%c=%d\n%s=%d\n", 'A', 'A', "AA", (int)'AA');
    // for (size_t i = 0; i < added; i++)
    // {
    //     printf("%ld: %s=%d\n", i, poly[i].pair, (char)poly[i].pair - 'Z' * 'Z');
    // }

    // size_t pt1 = count_poly(10);
    // for (size_t i = 0; i < 28; i++)
    // {
    //     count_poly(i);
    // }

    // size_t pt2 = count_poly(40);

    // fp = fopen(outp, "w");
    // fprintf(fp, "Part 1:Diff between most and least: %ld\n", pt1);
    // fprintf(fp, "Part 1:Diff between most and least: %ld\n", pt2);
    // fclose(fp);
    printf("Done, see \"%s\" for result\n", outp);
    return 0;
}