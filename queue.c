// TODO: Add any extra #includes that you need

#include "queue.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>


// TODO: Add your data structure definitions
typedef struct node{
   string data;
   struct node *next;
} Node;

typedef struct QueueRepr{
    Node *head;
    Node *rear;
    int length;
} QueueRepr;

// TODO: Fill in these function stubs


queue queue_create() {
    queue q = malloc(sizeof(QueueRepr));
    assert(q != NULL);
    q->head = NULL; 
    q->rear = NULL;
    q->length = 0;
    return q;
}

void queue_destroy(queue q) {
    if (q == NULL) {
        return;
    }
    Node *curr = q->head;
    while(curr != NULL) {
        Node *temp = curr->next;
        free(curr->data);
        free(curr);
        curr = temp;
    }
    free(q);
}

void queue_enqueue(queue q, string dat) {
    Node *new = malloc(sizeof(Node));
    assert(new != NULL);
    new->data = strdup(dat);
    new->next = NULL;
    if (q->length == 0) {
        q->head = new;
        q->rear = new;
    } else {
        q->rear->next = new;
        q->rear = new;
    }
    q->length++;
}

string queue_dequeue(queue q) {
    if (q == NULL || q->head == NULL) {
        return NULL;
    }
    
    Node *temp = q->head;
    string element = temp->data;
    if (q->length == 1) {
        q->head = NULL;
        q->rear = NULL;
    } else {
        q->head = temp->next;
    }
    free(temp);
    q->length--;
    return element;
}

string queue_peek(queue q) {
    if (q->head == NULL || q == NULL) {
        return NULL;
    } else {
        string element = q->head->data;
        return element;
    }
}

bool queue_empty(queue q) {
    return q == NULL || q->head == NULL;
}
