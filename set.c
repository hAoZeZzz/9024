// TODO: Add any extra #includes that you need

#include "set.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


// TODO: Add your data structure definitions
typedef struct node{
    string data;
    struct node *next;
} Node;

typedef struct SetRepr {
    Node *top;
    int size;
} SetRepr;



// TODO: Fill in these function stubs

set set_create() {
    set s = malloc(sizeof(SetRepr));
    assert(s != NULL);
    s->top = NULL;
    s->size = 0;
    return s;
}

void set_destroy(set s) {
    if (s == NULL) {
        return;
    }
    Node *current = s->top;
    while(current != NULL) {
        Node *temp = current->next;
        free(current->data);
        free(current);
        current = temp;
    }
    free(s);
}

void set_insert(set s, string dat) {
    Node *current = s->top;
    while (current != NULL) {
        if (strcmp(current->data, dat) == 0) {
            return;
        } else {
            current = current->next;
        }
    }
    Node *new = malloc(sizeof(Node));
    new->data = strdup(dat);
    new->next = s->top;
    s->top = new;
    s->size++;
}

void set_remove(set s, string dat) {
    Node *prev = NULL;
    Node *current = s->top;
    while (current != NULL) {
        if (strcmp(current->data, dat) == 0) {
            if (prev == NULL) {
                s->top = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

bool set_contains(set s, string dat) {
    if (s == NULL || s->top == NULL) {
        return false;
    }
    Node *current = s->top;
    while (current != NULL) {
        if (strcmp(current->data, dat) == 0) {
            return true;
        } else {
            current = current->next;
        }
    }
    return false;
}
