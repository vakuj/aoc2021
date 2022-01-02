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
    int tot_ctr;
    struct coord *next;
};
typedef struct coord coord_t;

char risk[ARRSIZE][ARRSIZE] = {0};
bool been[ARRSIZE][ARRSIZE] = {0};

int current_best = 0;

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
        if (next->tot_ctr < added * added * 10)
        {
            tot_ctr += next->val;

            printf("(%d,%d) = %d, %d (%d) \n", next->x, next->y, next->val, next->tot_ctr, tot_ctr);
        }
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
        else
        {
            prev = this;
            this = this->next;
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
    // if (is_end(x, y))
    // print_list(head);
    // printf("%d\n", head->val);
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

int min_dir(coord_t *head, coord_t *nogo, int x, int y)
{
    int xi = 0;
    int yi = 0;
    int min_risk = 10;
    int ret = -1;
    int weight = 0;
    for (int i = 0; i < 4; i++)
    {
        switch (i)
        {
        case 0:
            xi = x + 1;
            yi = y;
            weight = 0;
            break;
        case 1:
            xi = x;
            yi = y + 1;
            weight = 0;
            break;
        case 2:
            xi = x - 1;
            yi = y;
            weight = 6;
            break;
        case 3:
            xi = x;
            yi = y - 1;
            weight = 6;
            break;
        default:
            xi = x;
            yi = y;
            break;
        }
        if (is_end(xi, yi))
            return i;
        if (inbounds(xi, yi))
        {
            if (find_coord(head, xi, yi) == NULL && find_coord(nogo, xi, yi) == NULL)
            {
                if (risk[yi][xi] + weight < min_risk)
                {
                    min_risk = risk[yi][xi] + weight;
                    ret = i;
                }
            }
        }
    }
    return ret;
}

int find_opt(int xg, int yg)
{
    int temp_val = added * added * 10;
    int ctr = 0;
    int xi = 0;
    int yi = 0;
    coord_t *unvisited = NULL;
    coord_t *visited = NULL;
    coord_t *shortest = NULL;
    coord_t *current_min = NULL;
    coord_t *itr = NULL;
    coord_t *current_node = NULL;

    for (size_t i = 0; i < added; i++)
    {
        for (size_t j = 0; j < added; j++)
        {
            unvisited = append(unvisited, j, i, risk[i][j], temp_val);
            shortest = append(shortest, j, i, risk[i][j], temp_val);
        }
    }

    shortest->tot_ctr = 0;

    while (unvisited != NULL)
    {
        // printf("here\n");
        itr = shortest;
        current_min = NULL;
        while (itr != NULL)
        {
            if (find_coord(visited, itr->x, itr->y) == NULL)
            {
                if (current_min == NULL)
                    current_min = itr;
                else if (itr->tot_ctr < current_min->tot_ctr)
                {
                    current_min = itr;
                }
            }
            itr = itr->next;
        }
        for (size_t i = 0; i < 4; i++)
        {
            switch (i)
            {
            case 0:
                xi = current_min->x - 1;
                yi = current_min->y;
                break;
            case 1:
                xi = current_min->x + 1;
                yi = current_min->y;
                break;
            case 2:
                xi = current_min->x;
                yi = current_min->y - 1;
                break;
            case 3:
                xi = current_min->x;
                yi = current_min->y + 1;
                break;
            default:
                break;
            }
            current_node = find_coord(shortest, xi, yi);
            if (current_node != NULL)
            {
                temp_val = current_min->tot_ctr + current_node->val;
                if (temp_val < current_node->tot_ctr)
                    current_node->tot_ctr = temp_val;
            }
        }
        if (current_min == NULL)
            printf("NULL FOR CURRENT MIN \n");
        unvisited = del_coord(unvisited, current_min->x, current_min->y);
        visited = append(visited, current_min->x, current_min->y, current_min->val, 0);

        ctr++;
        if (ctr > 1000)
        {
            printf("visited: %d\n", length(visited));
            printf("unvisited: %d\n", length(unvisited));
            ctr = 0;
        }
    }
    printf("visited: %d\n", length(visited));
    printf("unvisited: %d\n", length(unvisited));
    print_list(shortest);
    current_node = find_coord(shortest, xg, yg);
    if (current_node == NULL)
        return -1;

    return current_node->tot_ctr;
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

    // print_arr(added);
    int tot = find_opt(added - 1, added - 1);
    printf("%d\n", tot);
    fp = fopen(outp, "w");
    fprintf(fp, "Part 1 not solved yet.");
    fprintf(fp, "Part 2 not solved yet.");
    fclose(fp);
    printf("Done, see \"%s\" for result\n", outp);
    return 0;
}