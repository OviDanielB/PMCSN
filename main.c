
#include <errno.h>
#include "event_time_generator.h"
#include "output.h"


/**
 * represents the number of tasks
 * in each part of the system
 */
struct State {
    long cldlet_1;
    long cldlet_2;
    long cloud_1;
    long cloud_2;
    long setup_2;
} state = {0, 0, 0, 0, 0};


/**
 * statistics gathering structure to calculate
 * time-averaged numbers
 */
struct Area {
    double node;
    double cloudlet_node;
    double cloud_node;
    double service;
    double cloudlet_service;
    double cloud_service;
} area = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};


/**
 * keeps track of completed tasks
 * useful for calculating throughput
 */
struct Completed {
    long cloudlet_class_1;
    long cloudlet_class_2;
    long cloud_class_1;
    long cloud_class_2;
} completed = {0, 0, 0, 0};


/**
 * keeps track of simulation time
 */
struct Time {
    double current;
    double next;
} time = {0.0, 0.0};

struct task_t *event_list;

int current_batch = 0;                   // batch in execution
double time_end;


void init_arrival() {

    push_event(EVENT_CLASS_1_ARRIVAL, getArrivalClass1());
    push_event(EVENT_CLASS_1_ARRIVAL, getArrivalClass1());
    push_event(EVENT_CLASS_2_ARRIVAL, getArrivalClass2());

    print_events();
}

void init_params(int argc, char **argv) {

    if (argc != 5) {
        fprintf(stderr, "Usage %s <N> <S> <#Batch> <Batch size>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    errno = 0;
    N = (int) strtol(argv[1], NULL, 10);
    S = (int) strtol(argv[2], NULL, 10);
    batch_number = (int) strtol(argv[3], NULL, 10);
    batch_time = (int) strtol(argv[4], NULL, 10);
    if (errno != 0) {
        fprintf(stderr, "Usage %s <N> <S> <#Batch> <Batch size>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

}

void process_event(struct event *ev) {

}

long jobs_left() {
    return state.cldlet_1 + state.cldlet_2 + state.cloud_1 + state.cloud_2 + state.setup_2;
}

int main(int argc, char **argv) {

    int i;
    time_end = (batch_number + 1) * batch_time;

    init_params(argc, argv);
    /* plants a seed for the stream generator (automatically generates seed for every stream) */
    PlantSeeds(SEED);
    init_arrival();
    init_output_stats();

    for (i = 0; i < batch_number; i++) {
        while (time.current < time_end || jobs_left() != 0) {
            process_event(pop_event());
        }
    }

    printf("Arrival Time Class 1 : %.2f \n", getArrivalClass1());
    printf("Arrival Time Class 2 : %.2f \n", getArrivalClass2());

    return 0;
}