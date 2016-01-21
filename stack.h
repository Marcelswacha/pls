/*
 * stack.h
 *
 *  Created on: 21 sty 2016
 *      Author: m-swacha
 */

#ifndef STACK_H_
#define STACK_H_

#include "node.h"

struct stack
{
    struct node** data;
    int data_size;
    int stack_ptr;
};

struct stack* stack_create(int initial_size);
struct stack* stack_create_default();
void stack_destroy(struct stack* s);
void stack_push(struct stack* s,  struct node* n);
struct node* stack_top(struct stack* s);
void stack_pop(struct stack* s);
int stack_not_empty(struct stack* s);

#endif /* STACK_H_ */
