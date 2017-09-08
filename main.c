
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

void init_arrival() {

    create_and_insert_event(EVENT_CLASS_1_ARRIVAL, getArrivalClass1());
    create_and_insert_event(EVENT_CLASS_2_ARRIVAL, getArrivalClass2());
}

int read_int(char **argv,int arg_index){
    return (int) strtol(argv[arg_index],NULL, 10);
}


int main(int argc, char **argv) {

    if (argc != 5) {
        fprintf(stderr, "Usage %s <N> <S> <#Batch> <Batch size>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    N = read_int(argv,1);
    S = read_int(argv,2);
    batch_number = read_int(argv,3);
    batch_time = read_int(argv,4);

    /* plants a seed for the stream generator (automatically generates seed for every stream) */
    PlantSeeds(SEED);

    init_arrival();
    init_output_stats();

    return 0;
}