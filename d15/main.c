#include <stdio.h>
#include <stdlib.h>
#include <stdint-gcc.h>
#include <string.h>
#include <stdbool.h>

#include "../common/common.h"

#define ARRSIZE 1024

#define NEWLN " \n"
#define SPACE " "
#define DELIM "1234567890"

struct coord
{
    int x;
    int y;
    int val;
    int tot_ctr;
    struct coord *next;
};
typedef struct coord coord_t;

char risk[ARRSIZE][ARRSIZE] = {0};
bool been[ARRSIZE][ARRSIZE] = {0};

int fscore_map[ARRSIZE][ARRSIZE] = {0};
int gscore_map[ARRSIZE][ARRSIZE] = {0};

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
            printf("%d", risk[i][j]);
        }
        printf("\n");
    }
}

void print_list(coord_t *head)
{
    coord_t *next;
    printf("--- Start =>\n");
    int tot_ctr = 0;
    if (head == NULL)
        printf("NULL\n");
    next = head;
    while (next != NULL)
    {
        // if (next->tot_ctr <= added * added * 10)
        // {
        //     tot_ctr += next->val;

        printf("(%d,%d) = %d, %d \n", next->x, next->y, next->val, next->tot_ctr);
        // }
        next = next->next;
    }
    printf("--- END => %d\n", tot_ctr);
}

bool check_coord(coord_t *this, int x, int y)
{
    return (this->x == x && this->y == y);
}

coord_t *new_coord(int x, int y, int val, int tot_ctr)
{
    coord_t *new = (coord_t *)malloc(sizeof(coord_t));
    new->x = x;
    new->y = y;
    new->val = val;
    new->tot_ctr = tot_ctr;
    new->next = NULL;
    return new;
}

coord_t *del_coord(coord_t *head, int x, int y)
{
    coord_t *prev = head;
    coord_t *this;
    coord_t *found;

    if (head == NULL)
        return NULL;
    this = prev->next;

    if (check_coord(prev, x, y))
    {
        free(head);
        return this;
    }
    while (this != NULL)
    {
        if (check_coord(this, x, y))
        {
            found = this;
            prev->next = this->next;
            free(found);
            break;
        }
        prev = this;
        this = this->next;
    }
    return head;
}

coord_t *find_coord(coord_t *head, int x, int y)
{
    coord_t *next;
    if (head == NULL)
        return NULL;
    next = head;
    while (next != NULL)
    {
        if (check_coord(next, x, y))
            return next;
        next = next->next;
    }
    return NULL;
}

int length(coord_t *head)
{
    int ctr = 0;
    coord_t *next;

    if (head == NULL)
        return 0;
    next = head;
    while (next != NULL)
    {
        ctr++;
        next = next->next;
    }
    return ctr;
}

coord_t *prepend(coord_t *head, int x, int y, int val, int tot_ctr)
{
    coord_t *new_head = new_coord(x, y, val, tot_ctr);
    if (head != NULL)
        new_head->next = head;
    return new_head;
}

coord_t *append(coord_t *head, int x, int y, int val, int tot_ctr)
{
    coord_t *this = head;
    coord_t *next;
    coord_t *tail = new_coord(x, y, val, tot_ctr);
    if (this == NULL)
        return tail;
    next = head->next;
    while (next != NULL)
    {
        this = next;
        next = next->next;
    }
    this->next = tail;
    return head;
}

coord_t *pop(coord_t *head)
{
    coord_t *next = head->next;
    free(head);
    return next;
}

bool inbounds(int x, int y)
{
    return ((x >= 0 && x < added) && (y >= 0 && y < added));
}

void resize_risk(int n)
{
    int temp_risk = 0;
    int increment = 0;
    for (size_t i = 0; i < added * n; i++)
    {
        for (size_t j = 0; j < added * n; j++)
        {
            increment = (i / added + j / added);
            temp_risk = risk[i % added][j % added];
            risk[i][j] = increment + temp_risk;

            if (risk[i][j] > 9)
                risk[i][j] %= 9;
        }
    }
    added *= n;
}

int find_opt(int xg, int yg)
{
    // A* algorithm
    int temp_val = added * added * 10,
        xi = 0,
        yi = 0,
        xc = 0,
        yc = 0,
        c_gscore = 0,
        n_gscore = 0;

    coord_t *unvisited = NULL,
            *visited = NULL,
            *current_min = NULL,
            *itr = NULL;

    memset(gscore_map, 0, sizeof(gscore_map));
    memset(fscore_map, 0, sizeof(fscore_map));

    for (size_t i = 0; i < added; i++)
    {
        for (size_t j = 0; j < added; j++)
        {
            fscore_map[i][j] = temp_val;
            gscore_map[i][j] = temp_val;
        }
    }
    unvisited = append(unvisited, 0, 0, 0, 0);
    fscore_map[0][0] = 0;
    gscore_map[0][0] = 0;

    while (unvisited != NULL)
    {
        itr = unvisited;
        current_min = NULL;
        while (itr != NULL)
        {
            if (current_min == NULL)
                current_min = itr;
            else if (fscore_map[itr->y][itr->x] < current_min->tot_ctr)
            {
                current_min = itr;
            }
            itr = itr->next;
        }

        xc = current_min->x;
        yc = current_min->y;
        c_gscore = gscore_map[yc][xc];

        unvisited = del_coord(unvisited, xc, yc);
        visited = prepend(visited, xc, yc, risk[yc][xc], fscore_map[yc][xc]);

        if (xc == xg && yc == yg)
        {
            return fscore_map[yg][xg] - risk[0][0];
        }
        for (size_t i = 0; i < 4; i++)
        {
            switch (i)
            {
            case 0:
                xi = xc - 1;
                yi = yc;
                break;
            case 1:
                xi = xc + 1;
                yi = yc;
                break;
            case 2:
                xi = xc;
                yi = yc - 1;
                break;
            case 3:
                xi = xc;
                yi = yc + 1;
                break;
            default:
                xi = -1;
                yi = -1;
                break;
            }

            if (inbounds(xi, yi))
            {
                n_gscore = gscore_map[yi][xi];
                temp_val = c_gscore + risk[yc][xc];
                if (temp_val < n_gscore)
                {
                    gscore_map[yi][xi] = temp_val;
                    fscore_map[yi][xi] = temp_val + risk[yi][xi];
                }

                if (find_coord(unvisited, xi, yi) == NULL && find_coord(visited, xi, yi) == NULL)
                {
                    unvisited = append(unvisited, xi, yi, risk[yi][xi], fscore_map[yi][xi]);
                }
            }
        }
    }
    current_min = find_coord(visited, xg, yg);
    if (current_min == NULL)
        return -1;

    return current_min->tot_ctr;
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

    int tot1 = find_opt(added - 1, added - 1);
    printf("Part 1: %d\n", tot1);
    resize_risk(5);
    printf("Resized input data, starting new search. \n");
    int tot2 = find_opt(added - 1, added - 1);
    printf("Part 2: %d\n", tot2);
    fp = fopen(outp, "w");
    fprintf(fp, "Part 1\nMinimum risk: %d\n", tot1);
    fprintf(fp, "Part 2\nMinimum risk: %d\n", tot2);
    fclose(fp);
    printf("Done, see \"%s\" for result\n", outp);
    return 0;
}