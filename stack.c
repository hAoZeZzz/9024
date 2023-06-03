// TODO: Add any extra #includes that you need

#include "stack.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>


// TODO: Add your data structure definitions
typedef struct node{
   string dat;
   struct node *next;
} Node;

typedef struct StackRepr{
    Node *top;
    int size;
} StackRepr;

// TODO: Fill in these function stubs

stack stack_create() {
    stack s = malloc(sizeof(StackRepr));
    assert(s != NULL);
    s->top = NULL; 
    s->size = 0;
    return s;
}

void stack_destroy(stack s) {
    if (s == NULL) {
        return;
    }
    Node *current = s->top;
    while(current != NULL) {
        Node *temp = current->next;
        free(current->dat);
        free(current);
        current = temp;
    }
    free(s);
}

void stack_push(stack s, string dat) {
    Node *new = malloc(sizeof(Node));
    new->dat = strdup(dat); // make a copy
    new->next = s->top;
    s->top = new;
    s->size++;
}

string stack_pop(stack s) {
    if (s->top == NULL) {
        return NULL;
    }
    Node *temp = s->top;
    string element = temp->dat;
    s->top = temp->next;
    s->size--;
    free(temp);
    return element;
}

string stack_peek(stack s) {
    if (s->top == NULL) {
        return NULL;
    } else {
        string temp = strdup(s->top->dat);
        return temp;
    }
}

bool stack_empty(stack s) {
    if (s->size == 0) {
        return true;
    } else {
        return false;
    }
}
