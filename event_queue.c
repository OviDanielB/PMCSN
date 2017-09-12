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

/**
 * Check if the event list is empty
 * @return boolean
 */
int is_empty() {
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

    if (!is_empty()) {
        struct event *p = head;
        printf("[ ");
        while(p != NULL){
            printf("(%f:%d) ",p->time, p->type);
            p = p->next;
        }
        printf("}\n");
    }
}

/**
 * Print all tasks in the queue in reverse order.
 */
void print_events_reverse() {

    if (!is_empty()) {
        struct event *p = tail;
        printf("[ ");
        while (p != NULL) {
            printf("(%f:%d) ", p->time, p->type);
            p = p->prev;
        }
        printf("]\n");
    }
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

    if (event == NULL){
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

    struct event *current = tail;

    if (is_empty()) {
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

    if (is_empty()) {
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
    return current;}

/**
 * Pop from queue the first task according to the scheduling.
 * @return task
 */
struct event *next_event() {
    if(is_empty()){
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
    event->interrupted_size = -1; // default value

    if (!is_empty()) {
        if (event->time < head->time) {
            event->next = head;
            event->next->prev = event;
            event->prev = NULL;
            head = event;
            inserted = 1;
        }

        struct event * prev = head;
        struct event * current = head->next;
        
        while (current != NULL && inserted == 0) {
            if (event->time < current->time) {
                prev->next = event;
                event->prev = prev;
                event->next = current;
                event->next->prev = event;
                inserted = 1;
            }
            prev = prev->next;
            current = current->next;
        }

        if (inserted == 0) { // append to list after last one
            prev->next = event;
            event->next = NULL;
            event->prev = prev;
            tail = event;
            inserted = 1;
        }
    } else {
        head = event;
        head->next = NULL;
        head->prev = NULL;
        tail = event;
    }
    return event;
}


void print_event(struct event* ev){
    char *ev_type;
    if(ev == NULL){
        printf("NULL EVENT\n");
        return;
    }
    switch(ev->type){
        case 0:
            ev_type = "EVENT_CLASS_1_ARRIVAL";
            break;
        case 1:
            ev_type = "EVENT_CLASS_1_CLOUDLET_COMPLETION";
            break;
        case 2:
            ev_type = "EVENT_CLASS_1_CLOUD_COMPLETION";
            break;
        case 3:
            ev_type = "EVENT_CLASS_2_ARRIVAL";
            break;
        case 4:
            ev_type = "EVENT_CLASS_2_CLOUDLET_COMPLETION";
            break;
        case 5:
            ev_type = "EVENT_CLASS_2_CLOUD_COMPLETION";
            break;
        case 6:
            ev_type = "EVENT_CLASS_2_SETUP";
            break;
        default:
            printf("No event type defined : %d\n", ev->type);
    }

    /* arrival*/
    if(ev->job_size == -1){
        printf("Next event = {time : %.2f, type: %s } \n", ev->time, ev_type);
    } else { /* completion*/
        printf("Next event = {time : %.2f, type: %s, job_size: %.2f } \n", ev->time, ev_type, ev->job_size);
    }
}
