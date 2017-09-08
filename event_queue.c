/**
 * Sorted queue manager.
 */

#include <stdlib.h>
#include <stdio.h>
#include "event_queue.h"

#define FIFO 1
#define LIFO 2

/* task scheduling */
int policy = 1;



/* first item of the queue */
struct event *head = NULL;
/* last item of the queue */
struct event *tail = NULL;


int isEmpty(){
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

    if(head != NULL){
        struct event * actual = head;
        while(actual != NULL){
            printf(" %f:%d,",actual->time,actual->type);
            actual = actual->next;
        }
        printf("\n");
    }
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
struct event *remove_event(struct event *event) {

    if(event == NULL){
        return NULL;
    }
    struct event * after = event->next;
    struct event * before = event->prev;

    if(before != NULL){
        before->next = before->next->next;
    }
    else{
        head = after;
    }

    if(after != NULL){
        after->prev = after->prev->prev;
    }
    else{
        tail = before;
    }
    event->next = NULL;
    event->prev = NULL;
    return event;
}

/**
 * Remove from the queue the first occurrence of the task of type "class",
 * scanning from the tail.
 * @param class
 * @return task removed
 */
struct event *remove_last_event_by_type(int type) {

        // TODO
}

/**
 * Remove from the queue the first occurrence of the task of type "class",
 * scanning from the head.
 * @param class
 * @return task removed
 */
struct event *remove_first_event_by_type(int type) {

    // TODO
}

/**
 * Pop from queue the first task according to the scheduling.
 * @return task
 */
struct event *pop_event() {
    if(head == NULL){
        return NULL;
    }
    if(head->next == NULL){
        struct event * event = head;
        head = NULL;
        tail = NULL;
        return event;
    }

    struct event * first = head;
    head = head->next;
    head->prev = NULL;

    first->prev = NULL;
    first->next = NULL;


    return first;
}




/**
 * Create a new task and push it in the queue.
 * @param type event
 * @param time
 */
struct event *create_and_insert_event(int type, double time) {

    int inserted = 0;

    /* allocate new event struct */
    struct event *event = alloc_event();
    event->time = time;
    event->type = type;
    event->job_size = -1; // default value

    if(head != NULL)
    {
        if(event->time < head->time)
        {
            event->next = head;
            event->next->prev = event;
            event->prev = NULL;
            head = event;
            inserted = 1;
        }

        struct event * prev = head;
        struct event * current = head->next;
        
        while(current != NULL && inserted == 0)
        {
            if(event->time < current->time)
            {
                prev->next = event;
                event->prev = prev;
                event->next = current;
                event->next->prev = event;
                inserted = 1;
            }
            prev = prev->next;
            current = current->next;
        }

        if(inserted == 0)  // append to list after last one
        {
            prev->next = event;
            event->next = NULL;
            event->prev = prev;
            tail = event;
            inserted = 1;
        }
    }

    else
    {
        head = event;
        head->next = NULL;
        head->prev = NULL;
        tail = event;
    }
    return event;
}

