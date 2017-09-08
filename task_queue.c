/**
 * Sorted queue manager.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "task_queue.h"

#define FIFO 1
#define LIFO 2

/* task scheduling */
int policy = 1;

/** Definition of an item of the queue **/
struct task_t {
    int class;
    double time;
    struct task_t *prev;
    struct task_t *next;
};

/* first item of the queue */
struct task_t *head = NULL;
/* last item of the queue */
struct task_t *tail = NULL;

struct task_t *current = NULL;


/**
 * Check if the queue is empty.
 */
bool isEmpty() {
    return head == NULL;
}

/**
 * Get the number of the tasks in the queue
 * @return int number
 */
int get_length() {
    int length = 0;
    struct task_t *current;

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
void print_tasks() {

    struct task_t *p = head;
    printf("[ ");
    while (p != NULL) {
        printf("(%d,%f) ",p->class,p->time);
        p = p->next;
    }
    printf(" ]\n");
}

/**
 * Print all tasks in the queue in reverse order.
 */
void print_tasks_reverse() {

    struct task_t *p = tail;
    printf("\n[ ");
    while (p != NULL) {
        printf("(%d,%f) ",p->class,p->time);
        p = p->prev;
    }
    printf(" ]");
}

/**
 * Allocation of an element in the queue.
 *
 * @return allocated task
 */
struct task_t *alloc_task() {

    struct task_t *t;
    t = malloc(sizeof(struct task_t));
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
void insertFirst(struct task_t *new) {

    if (isEmpty()) {
        /* update last task reference */
        tail = new;
    } else {
        /* update first prev task*/
        head->prev = new;
    }
    /* point it to old first task */
    new->next = head;
    /* point first to new first task */
    head = new;
}

/**
 * Insert a new task at the end of the queue.
 * @param new task
 */
void insertLast(struct task_t *new) {

    if (isEmpty()) {
        /* update last task reference */
        tail = new;
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
void free_task(struct task_t *t) {
    free(t);
}

/**
 * Remove a task from the queue at the first or at the last position in the queue.
 * @param p
 */
struct task_t *remove_task(struct task_t *p) {

    struct task_t *tmp = p;
    if (!isEmpty()) {
        if (p == head) {
            head = head->next;
            tmp->next->prev = tmp->prev;
        }
        else if (p == tail) {
            tail = tail->prev;
            tmp->prev->next = tmp->next;
        } else {
            struct task_t *current = head;
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
struct task_t *remove_last_task_by_class(int class) {

    struct task_t *current = tail;

    if (isEmpty()) {
        return NULL;
    }

    while (current->class != class) {
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
struct task_t *remove_first_task_by_class(int class) {

    struct task_t *current = head;

    if (isEmpty()) {
        return NULL;
    }

    while (current->class != class) {
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
struct task_t *pop_task() {
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
void insert_after_task(struct task_t *new, struct task_t *p) {

    if (p == NULL) {
        insertFirst(new);
    } else if (p == tail) {
        insertLast(new);
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
void insert_sorted_queue(struct task_t *new) {

    struct task_t *p;
    for (p = tail; p != NULL; p = p->prev) {
        if (p->time > new->time) {
            /* insert new after pnext (before p) */
            insert_after_task(new, p->prev);
            return;
        }
    }
    /* task to insert at the end of the queue */
    insert_after_task(new, tail);
}

/**
 * Push a new task in the queue.
 * @param class
 * @param time
 */
void push_task(int class, double time) {

    struct task_t *new = alloc_task();
    new->class = class;
    new->time = time;

    if (isEmpty()) {
        insertFirst(new);
    } else {
        insert_sorted_queue(new);
    }
}