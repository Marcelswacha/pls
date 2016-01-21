#include <stdlib.h>
#include <stdio.h>

#include "stack.h"

const int DEFAULT_STACK_SIZE = 1024*1024;

struct stack* stack_create(int initial_size)
{
    struct stack* s = (struct stack*)malloc(sizeof (struct stack));

        if (s) {
            s->data = (struct node** )malloc(
                    initial_size * sizeof (struct node*));
            if (s->data) {
                s->data_size = initial_size;
                s->stack_ptr = -1;
            }
            else
                return NULL;
        }

        return s;
}

struct stack* stack_create_default()
{
    return stack_create(DEFAULT_STACK_SIZE);
}

void stack_destroy(struct stack* s)
{
    free(s->data);
}

void stack_push(struct stack* s, struct node* n)
{
    if (s->stack_ptr + 1 == s->data_size) {
        /* need to allocate more memory */
        int new_size = s->data_size * 2;
        struct node** new_data = (struct node**)realloc(s->data, new_size);
        printf("Reallocated %d\n", new_size);

        if (new_data) {
            /* memory reallocated */
            s->data = new_data;
            s->data_size = new_size;
        } else {
            /* put some error? */
            return;
        }
    }

    /* no need to allocate more memory */
    s->stack_ptr++;
    s->data[s->stack_ptr] = n;
}

struct node* stack_top(struct stack* s)
{
    return (s->stack_ptr >= 0)? s->data[s->stack_ptr] : NULL;
}

void stack_pop(struct stack* s)
{
    if (s->stack_ptr >= 0) s->stack_ptr--;
}

int stack_not_empty(struct stack* s)
{
    return (s->stack_ptr >= 0)? 1 : 0;
}


