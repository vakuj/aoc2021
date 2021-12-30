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

struct coord
{
    int x;
    int y;
    int val;
    struct coord *next;
};
typedef struct coord coord_t;

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

void print_list(coord_t *head)
{
    coord_t *next;
    int tot_ctr = 0;
    if (head == NULL)
        printf("NULL\n");
    next = head;
    while (next != NULL)
    {
        tot_ctr += next->val;
        printf("(%d,%d) = %d (%d) \n", next->x, next->y, next->val, tot_ctr);
        next = next->next;
    }
    printf("--- END => %d\n", tot_ctr);
}
bool inbounds(int x, int y)
{
    return (x >= 0 && y >= 0 && x < added && y < added);
}

bool is_end(int x, int y)
{
    return (x == added - 1 && y == added - 1);
}

bool check_coord(coord_t *this, int x, int y)
{
    return (this->x == x && this->y == y);
}

coord_t *new_coord(int x, int y, int val)
{
    coord_t *new = (coord_t *)malloc(sizeof(coord_t));
    new->x = x;
    new->y = y;
    new->val = val;
    new->next = NULL;
    return new;
}

coord_t *del_coord(coord_t *head, int x, int y)
{
    coord_t *prev = head;
    coord_t *this = prev->next;
    coord_t *next = this->next;
    coord_t *found;

    if (head == NULL)
        return NULL;
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
            this = next->next;
            next = this->next;
            free(found);
        }
        else
        {
            prev = this;
            this = this->next;
            next = next->next;
        }
    }
    return head;
}

coord_t *find_coord(coord_t *head, int x, int y)
{
    coord_t *next;
    if (head == NULL)
        return NULL;
    next = head->next;
    while (next != NULL)
    {
        if (check_coord(next, x, y))
            return next;
        next = next->next;
    }
    return NULL;
}

coord_t *prepend(coord_t *head, int x, int y, int val)
{
    coord_t *new_head = new_coord(x, y, val);
    // if (is_end(x, y))
    // print_list(head);
    if (head != NULL)
        new_head->next = head;
    return new_head;
}

coord_t *append(coord_t *head, int x, int y, int val)
{
    coord_t *this = head;
    coord_t *next = head->next;
    coord_t *tail = new_coord(x, y, val);
    if (this == NULL)
        return tail;
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

int tot_count(coord_t *head)
{
    coord_t *next;
    int ctr = 0;
    if (head == NULL)
        return 0;
    next = head->next;
    ctr += head->val;
    while (next != NULL)
    {
        if (next->x != 0 && next->y != 0)
            ctr += next->val;
        next = next->next;
    }
    return ctr;
}

coord_t *travese(coord_t *head, int x, int y)
{
    coord_t *up, *down, *left, *right;
    int min_ctr = added * added * 10;
    int tmp_ctr = 0;

    if (!inbounds(x, y))
    {
        return head;
    }
    if (is_end(x, y))
    {
        head = prepend(head, x, y, risk[y][x]);
        return head;
    }
    if (find_coord(head, x, y) != NULL)
    {
        return head;
    }
    head = prepend(head, x, y, risk[y][x]);
    up = head;
    down = head;
    left = head;
    right = head;

    /** should look for smallest value
     *  shortest way is diagonal -> optimal path
     *  risk level changes optimal path   
     */
    right = travese(right, x + 1, y);
    up = travese(up, x, y + 1);
    // left = travese(left, x - 1, y);
    // down = travese(down, x, y - 1);

    tmp_ctr = tot_count(right);
    if (is_end(right->x, right->y) && tmp_ctr < min_ctr)
    {
        head = right;
        free(right);
        min_ctr = tmp_ctr;
    }
    tmp_ctr = tot_count(up);
    if (is_end(up->x, up->y) && tmp_ctr < min_ctr)
    {
        head = up;
        free(up);
        min_ctr = tmp_ctr;
    }
    // tmp_ctr = tot_count(left);
    // if (is_end(left->x, left->y) && tmp_ctr < min_ctr)
    // {
    //     head = left;
    //     free(left);
    //     min_ctr = tmp_ctr;
    // }
    // tmp_ctr = tot_count(down);
    // if (is_end(down->x, down->y) && tmp_ctr < min_ctr)
    // {
    //     head = down;
    //     free(down);
    //     min_ctr = tmp_ctr;
    // }
    return head;
}

int find_opt(void)
{
    coord_t *head = new_coord(0, 0, risk[0][0]);
    coord_t *right = travese(head, 1, 0);
    coord_t *up = travese(head, 0, 1);
    int min_ctr = added * added * 10;
    int tmp_ctr = tot_count(right);
    if (is_end(right->x, right->y) && tmp_ctr < min_ctr)
    {
        head = right;
        min_ctr = tmp_ctr;
    }
    if (is_end(up->x, up->y) && tmp_ctr < min_ctr)
    {
        head = up;
        min_ctr = tmp_ctr;
    }
    print_list(up);
    print_list(right);
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
    int tot = find_opt();
    printf("%d\n", tot);
    fp = fopen(outp, "w");
    fprintf(fp, "Part 1 not solved yet.");
    fprintf(fp, "Part 2 not solved yet.");
    fclose(fp);
    printf("Done, see \"%s\" for result\n", outp);
    return 0;
}