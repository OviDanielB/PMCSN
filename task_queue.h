
#ifndef PMCSN_TASK_QUEUE_H
#define PMCSN_TASK_QUEUE_H

#endif //PMCSN_TASK_QUEUE_H

/**
 * Check if the queue is empty.
 */
bool isEmpty();
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
void print_tasks();
/**
 * Print all tasks in the queue in reverse order.
 */
void print_tasks_reverse();
/**
 * Allocation of an element in the queue.
 *
 * @return allocated task
 */
struct task_t *alloc_task();
/**
 * Insert a new task at the head of the queue.
 * @param new task
 */
void insertFirst(struct task_t *new);
/**
 * Insert a new task at the end of the queue.
 * @param new task
 */
void insertLast(struct task_t *new);
/**
 * Deallocation of an element in the queue
 *
 * @param t
 */
void free_task(struct task_t *t);
/**
 * Remove a task from the queue at the first or at the last position in the queue.
 * @param p
 */
struct task_t *remove_task(struct task_t *p);
/**
 * Remove from the queue the first occurrence of the task of type "class",
 * scanning from the tail.
 * @param class
 * @return task removed
 */
struct task_t *remove_last_task_by_class(int class);
/**
 * Remove from the queue the first occurrence of the task of type "class",
 * scanning from the head.
 * @param class
 * @return task removed
 */
struct task_t *remove_first_task_by_class(int class);
/**
 * Pop from queue the first task according to the scheduling.
 * @return task
 */
struct task_t *pop_task();
/**
 * Push a new task as the last item in the queue
 * @param new
 * @param next
 */
void insert_after_task(struct task_t *new, struct task_t *p);
/**
 * Insert a new task sorting by time value.
 * @param new task
 */
void insert_sorted_queue(struct task_t *new);
/**
 * Push a new task in the queue.
 * @param class
 * @param time
 */
void push_task(int class, double time);
