#ifndef __LIST_H__
#define __LIST_H__

/* your list data structure declarations */

#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
    int value;
    struct node *next; 
};


/* your function declarations associated with the list */
void list_clear(struct node *);

void list_print_matches(const char *, const struct node *);

void list_print(const struct node *);

int list_delete(const char *, struct node **);

void list_append(const char *, struct node **);

void list_reverse(struct node **);

void list_sort(struct node **);

int get_num_tokens(const char *);

char** tokenify(const char *);



#endif // __LIST_H__
