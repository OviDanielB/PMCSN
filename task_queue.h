
#ifndef PMCSN_TASK_QUEUE_H
#define PMCSN_TASK_QUEUE_H

#endif //PMCSN_TASK_QUEUE_H

/** Definition of an item of the queue **/
struct event {
    int type;
    double time;
    struct event *prev;
    struct event *next;
};

/**
 * Check if the queue is empty.
 */
int isEmpty();
/**
 * Get the number of the tasks in the queue
 * @return int number
 */
int get_length();
/**
 * Set queue policy.
 * @param p 1 for FIFO, 2 for LIFO
 */
void set_policy(int p);
/**
 * Print all tasks in the queue.
 */
void print_events();
/**
 * Print all tasks in the queue in reverse order.
 */
void print_events_reverse();
/**
 * Allocation of an element in the queue.
 *
 * @return allocated task
 */
struct event *alloc_event();
/**
 * Insert a new task at the head of the queue.
 * @param new task
 */
void insert_first(struct event *new);
/**
 * Insert a new task at the end of the queue.
 * @param new task
 */
void insert_last(struct event *new);
/**
 * Deallocation of an element in the queue
 *
 * @param t
 */
void free_event(struct event *t);
/**
 * Remove a task from the queue at the first or at the last position in the queue.
 * @param p
 */
struct event *remove_event(struct event *p);
/**
 * Remove from the queue the first occurrence of the task of type "class",
 * scanning from the tail.
 * @param class
 * @return task removed
 */
struct event *remove_last_event_by_type(int type);
/**
 * Remove from the queue the first occurrence of the task of type "class",
 * scanning from the head.
 * @param class
 * @return task removed
 */
struct event *remove_first_event_by_type(int type);
/**
 * Pop from queue the first task according to the scheduling.
 * @return task
 */
struct event *pop_event();
/**
 * Push a new task as the last item in the queue
 * @param new
 * @param next
 */
void insert_after_event(struct event *new, struct event *p);
/**
 * Insert a new task sorting by time value.
 * @param new task
 */
void insert_sorted_queue(struct event *new);
/**
 * Push a new task in the queue.
 * @param type
 * @param time
 */
void push_event(int type, double time);
