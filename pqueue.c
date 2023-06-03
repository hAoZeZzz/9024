// TODO: Add any extra #includes that you need

#include "pqueue.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

// TODO: Add your data structure definitions

typedef struct node {
    string dat;
    priority prio;
    struct node *next;
} Node;

typedef struct PriorityQueueRepr {
    Node *head;
    Node *rear;
} PriorityQueueRepr;


// TODO: Fill in these function stubs


pqueue pqueue_create() {
    pqueue pq = malloc(sizeof(PriorityQueueRepr));
    assert(pq != NULL);
    pq->head = NULL;
    pq->rear = NULL;
    return pq;
}

void pqueue_destroy(pqueue pq) {
    if (pq == NULL) {
        return;
    }
    Node *curr = pq->head;
    while (curr != NULL) {
        Node *temp = curr->next;
        free(curr->dat);
        free(curr);
        curr = temp;
    }
    free(pq);
}

void pqueue_join(pqueue pq, string dat, priority prio) {
    Node *new = malloc(sizeof(Node));
    assert(new != NULL);
    new->dat = strdup(dat);
    new->prio = prio;
    new->next = NULL;
    if (pq->head == NULL || pq->head->prio > prio) {
        new->next = pq->head;
        pq->head = new;
        if (pq->rear == NULL) {
            pq->rear = new;
        }
    } else {
        Node *curr = pq->head;
        while (curr->next != NULL && curr->next->prio < prio) {
            curr = curr->next;
        }
        new->next = curr->next;
        curr->next = new;
        if (new->next == NULL) {
            pq->rear = new;
        }
    }
}

string pqueue_leave(pqueue pq) {
    if (pq == NULL || pq->head == NULL) {
        return NULL;
    }
    Node *temp = pq->head;
    string element = temp->dat;
    pq->head = temp->next;
    if (pq->head == NULL) {
        pq->rear = NULL;
    }
    free(temp);
    return element;
}

string pqueue_peek(pqueue pq) {
    if (pq == NULL || pq->head == NULL) {
        return NULL;
    } else {
        string element = strdup(pq->head->dat);
        return element;
    }
}

bool pqueue_empty(pqueue pq) {
    return pq == NULL || pq->head == NULL;
}
