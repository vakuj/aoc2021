#include <stdio.h>
#include <stdlib.h>
#include <stdint-gcc.h>
#include <string.h>
#include <stdbool.h>

#include "../common/common.h"

#define ARRSIZE 400

#define NEWLN " \n"
#define SPACE " "
#define DELIM "1234567890"

typedef struct
{
    int x;
    int y;
} coord_t;

char risk[ARRSIZE][ARRSIZE] = {0};
bool been[ARRSIZE][ARRSIZE] = {0};

int added = 0;

void print_line(char *line, int line_nbr)
{
    printf("%d : %s\n", line_nbr, line);
}

void print_arr(int len)
{
    for (size_t i = 0; i < len; i++)
    {
        for (size_t j = 0; j < len; j++)
        {
            printf("%2d", risk[i][j]);
        }
        printf("\n");
    }
}

void get_nexts(coord_t *nexts, coord_t origin, int offset)
{
    nexts[0].x = origin.x - offset;
    nexts[0].y = origin.y;
    nexts[1].x = origin.x + offset;
    nexts[1].y = origin.y;
    nexts[2].x = origin.x;
    nexts[2].y = origin.y - offset;
    nexts[3].x = origin.x;
    nexts[3].y = origin.y + offset;
}

bool inbounds(coord_t this)
{
    return (this.x >= 0 && this.y >= 0 && this.x < added && this.y < added);
}

bool is_end(coord_t this)
{
    return (this.x == added - 1 && this.y == added - 1);
}

bool has_been(coord_t next)
{
    return been[next.y][next.x];
}

void set_been(coord_t this)
{
    been[this.y][this.x] = true;
}

int travese(coord_t this, coord_t prev, int tot_ctr)
{
    // if (!inbounds(this))
    //     return tot_ctr;
    // if (has_been(this))
    //     return tot_ctr;
    if (is_end(this))
        return tot_ctr;

    coord_t nexts[4];
    bool prev_been[ARRSIZE][ARRSIZE] = {0};
    int min_ctr = added * added * 10;
    int tmp_ctr = 0;
    set_been(this);
    get_nexts(nexts, this, 1);
    // memset(prev_been, 0, sizeof(prev_been));
    // min_ind = find_min(nexts, prev);

    for (size_t i = 0; i < 4; i++)
    {
        if (inbounds(nexts[i]) && !has_been(nexts[i]))
        {
            memmove(prev_been, been, sizeof(bool[ARRSIZE][ARRSIZE]));
            printf("(%d,%d) -> (%d,%d)\n", this.x, this.y, nexts[i].x, nexts[i].y);
            tmp_ctr = travese(nexts[i], this, tot_ctr + risk[nexts[i].y][nexts[i].x]);
            if (tmp_ctr > 0)
            {
                if (tmp_ctr < min_ctr)
                    min_ctr = tot_ctr;
                // if (is_end(nexts[i]))
            }
            memmove(been, prev_been, sizeof(been));
            set_been(nexts[i]);
        }
    }
    return min_ctr;
}

void parse_file(FILE *fp, void func(char *, int), bool silent)
{
    char next[BUFSIZE];
    char *ptr;
    int next_ctr = 0;
    int ptr_ctr = 0;
    memset(risk, 0, sizeof(risk));
    while (fgets(next, BUFSIZE, fp) != NULL)
    {
        ptr = strtok(next, "\n");
        ptr_ctr = 0;
        ptr = strpbrk(ptr, DELIM);
        while (ptr != NULL)
        {
            risk[next_ctr][ptr_ctr] = *ptr - 48;
            ptr_ctr++;
            ptr = strpbrk(ptr + 1, DELIM);
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

    print_arr(added);
    int tot = travese((coord_t){0, 0}, (coord_t){-1, -1}, 0);
    printf("%d\n", tot);
    fp = fopen(outp, "w");
    fprintf(fp, "Part 1 not solved yet.");
    fprintf(fp, "Part 2 not solved yet.");
    fclose(fp);
    printf("Done, see \"%s\" for result\n", outp);
    return 0;
}