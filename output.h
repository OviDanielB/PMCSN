#ifndef PMCSN_OUTPUT_H
#define PMCSN_OUTPUT_H

#endif //PMCSN_OUTPUT_H

#include "utils/constants.h"

double t_star;

/**
 * Keeps track of completed tasks
 * useful for calculating throughput
 */
struct Completed {
    long cloudlet_class_1;      //#jobs of class 1 completed in the cloudlet
    long cloudlet_class_2;      //#jobs of class 2 completed in the cloudlet
    long cloud_class_1;         //#jobs of class 1 completed in the cloud
    long cloud_class_2;         //#jobs of class 2 completed in the cloud
    long interrupted_class_2;   //#jobs of class 2 interrupted in the cloudlet
};

/**
 * Statistics gathering structure to calculate
 * time-averaged numbers
 */
struct Area {
    double node;
    double cloudlet_node;
    double cloud_node;
};

struct Service {
    double cloudlet_class_1;
    double cloudlet_class_2;
    double cloud_class_2;
    double cloud_class_1;

    double cloudlet_class_2_interrupted;    // time elapsed in cloudlet by an interrupted job
    double cloud_class_2_interrupted;       // remaining time in cloud + setup
};

struct Batch_stat {
    double avg_node;
    double avg_node_cloudlet;
    double avg_node_cloud;
};

struct End_stat {
    double avg_node;
    double avg_node_cloudlet;
    double avg_node_cloud;
};

struct Completed *completed;
struct Area *area;
struct Service *service;
struct Batch_stat *batch_stat;
struct End_stat *end_stat;

void init_output_stats();

double estimate_interval_endpoint(double);




