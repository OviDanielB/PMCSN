/**
 * Sorted queue manager.
 */

#include <stdlib.h>
#include <stdio.h>
#include "task_queue.h"

#define FIFO 1
#define LIFO 2

/* task scheduling */
int policy = 1;



/* first item of the queue */
struct event *head = NULL;
/* last item of the queue */
struct event *tail = NULL;

struct event *current = NULL;


/**
 * Check if the queue is empty.
 */
int isEmpty() {
    return head == NULL;
}

/**
 * Get the number of the tasks in the queue
 * @return int number
 */
int get_length() {
    int length = 0;
    struct event *current;

    for (current = head; current != NULL; current = current->next) {
        length++;
    }
    return length;
}

/**
 * Set queue policy.
 * @param p 1 for FIFO, 2 for LIFO
 */
void set_policy(int p) {
    if (p == FIFO || p == LIFO)
        policy = p;
    else
        fprintf(stderr, "Policy queue not managed.\n");
}

/**
 * Print all tasks in the queue.
 */
void print_events() {

    struct event *p = head;
    printf("[");
    while (p != NULL) {
        printf(" (%d,%f)",p->type,p->time);
        p = p->next;
    }
    printf(" ]\n");
}

/**
 * Print all tasks in the queue in reverse order.
 */
void print_events_reverse() {

    struct event *p = tail;
    printf("\n[ ");
    while (p != NULL) {
        printf("(%d,%f) ",p->type,p->time);
        p = p->prev;
    }
    printf(" ]");
}

/**
 * Allocation of an element in the queue.
 *
 * @return allocated task
 */
struct event *alloc_event() {

    struct event *t;
    t = malloc(sizeof(struct event));
    if (t == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    return t;
}

/**
 * Insert a new task at the head of the queue.
 * @param new task
 */
void insert_first(struct event *new) {

    if (isEmpty()) {
        /* update last task reference */
        tail = new;
    } else {
        /* update first prev task*/
        head->prev = new;
        /* point it to old first task */
        new->next = head;
    }
    /* point first to new first task */
    head = new;
}

/**
 * Insert a new task at the end of the queue.
 * @param new task
 */
void insert_last(struct event *new) {

    if (isEmpty()) {
        /* update last task reference */
        head = new;
    } else {
        /* make task a new last task */
        tail->next = new;
        /* mark old last node as prev of new task */
        new->prev = tail;
    }
    /* point last to new last task */
    tail = new;
}

/**
 * Deallocation of an element in the queue
 *
 * @param t
 */
void free_event(struct event *t) {
    free(t);
}

/**
 * Remove a task from the queue at the first or at the last position in the queue.
 * @param p
 */
struct event *remove_task(struct event *p) {

    struct event *tmp = p;
    if (!isEmpty()) {
        if (p == head) {
            head = head->next;
            tmp->next->prev = tmp->prev;
        }
        else if (p == tail) {
            tail = tail->prev;
            tmp->prev->next = tmp->next;
        } else {
            struct event *current = head;
            while (current->time != p->time) {
                if (current->next == NULL) {
                    /* if not found a match */
                    return NULL;
                } else
                    current = current->next;
            }
            /* found a match */
            current->prev->next = current->next;
            current->next->prev = current->prev;
            return current;
        }
    }
    return p;
}

/**
 * Remove from the queue the first occurrence of the task of type "class",
 * scanning from the tail.
 * @param class
 * @return task removed
 */
struct event *remove_last_event_by_type(int type) {

    struct event *current = tail;

    if (isEmpty()) {
        return NULL;
    }

    while (current->type != type) {
        if (current->prev == NULL) {
            /* if not found a match */
            return NULL;
        } else
            current = current->prev;
    }
    /* if found a match */

    if (current == head) {
        head = head->next;
    } else {
        current->prev->next = current->next;
    }

    if (current == tail) {
        tail = current->prev;
    } else {
        current->next->prev = current->prev;
    }
    return current;
}

/**
 * Remove from the queue the first occurrence of the task of type "class",
 * scanning from the head.
 * @param class
 * @return task removed
 */
struct event *remove_first_event_by_type(int type) {

    struct event *current = head;

    if (isEmpty()) {
        return NULL;
    }

    while (current->type != type) {
        if (current->next == NULL) {
            /* if not found a match */
            return NULL;
        } else
            current = current->next;
    }
    /* if found a match */

    if (current == head) {
        head = head->next;
    } else {
        current->prev->next = current->next;
    }

    if (current == tail) {
        tail = current->prev;
    } else {
        current->next->prev = current->prev;
    }
    return current;
}

/**
 * Pop from queue the first task according to the scheduling.
 * @return task
 */
struct event *pop_event() {
    switch (policy) {
        case LIFO:
            return remove_task(tail);
        default: // FIFO
            return remove_task(head);
    }
}

/**
 * Push a new task as the last item in the queue
 * @param new
 * @param next
 */
void insert_after_event(struct event *new, struct event *p) {

    if (p == NULL) {
        insert_first(new);
    } else if (p == tail) {
        insert_last(new);
    } else {
        new->prev = p;
        new->next = p->next;
        p->next = new;
    }
}

/**
 * Insert a new task sorting by time value.
 * @param new task
 */
void insert_sorted_queue(struct event *new) {

    struct event *p;
    for (p = head; p != NULL; p = p->next) {
        if (p->time > new->time) {
            /* insert new after pnext (before p) */
            insert_after_event(new, p->prev);
            return;
        }
    }
    /* task to insert at the end of the queue */
    insert_after_event(new, tail);
}

/**
 * Push a new task in the queue.
 * @param class
 * @param time
 */
void push_event(int type, double time) {

    struct event *new = alloc_event();
    new->type = type;
    new->time = time;

    if (isEmpty()) {
        insert_first(new);
    } else {
        insert_sorted_queue(new);
    }
}


int main() {
    push_event(1,0.2);
    printf("HEAD %f TAIL %f\n", head->time, tail->time);
    push_event(2,0.3);
    printf("HEAD %f TAIL %f\n", head->time, tail->time);
    push_event(1,0.1);
    printf("HEAD %f TAIL %f\n", head->time, tail->time);
    push_event(1,0.05);
    printf("HEAD %f TAIL %f\n", head->time, tail->time);
    print_events();
}