
#include "utils/constants.h"
#include "event_time_generator.h"


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
struct Completed{
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



int main() {

    /* plants a seed for the stream generator (automatically generates seed for every stream) */
    PlantSeeds(SEED);

    printf("Arrival Time Class 1 : %.2f \n", getArrivalClass1());
    printf("Arrival Time Class 2 : %.2f \n", getArrivalClass2());

    return 0;
}