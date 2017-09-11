#ifndef PMCSN_OUTPUT_H
#define PMCSN_OUTPUT_H

#endif //PMCSN_OUTPUT_H

#include "utils/constants.h"

/*
 * types of service.
 * i.e. service[CLOUD_INTERRUPTED] = setup + E[remaining]
 *      service[CLDLET_INTERRUPTED] = amount of time elapsed on the clodlet before interruption
 */
#define CLDLET_CLASS_1      0
#define CLDLET_CLASS_2      1
#define CLOUD_CLASS_1       2
#define CLOUD_CLASS_2       3
#define CLOUD_INTERRUPTED   4
#define CLDLET_INTERRUPTED  5


double t_star;

/**
 * Keeps track of completed tasks: useful for calculating throughput.
 * i.e. cloud_class_2 - interrupted_class_2 = #class 2 jobs sent directly to the cloud
 *
 */
struct Completed {
    long cloudlet_class_1;      //#jobs of class 1 completed in the cloudlet
    long cloudlet_class_2;      //#jobs of class 2 completed in the cloudlet
    long cloud_class_1;         //#jobs of class 1 completed in the cloud
    long cloud_class_2;         //#jobs of class 2 completed in the cloud
    long interrupted_class_2;   //#jobs of class 2 interrupted in the cloudlet
};

/**
 * Statistics gathering structure to calculate time-averaged numbers.
 * service[CLOUD_INTERRUPTED:4]:    Time elapsed in cloudlet by an interrupted job
 * service[CLDLET_INTERRUPTED:5]:   Remaining time in cloud + setup
 *
 *i.e cservice[CLOUD_INTERRUPTED:4] - service[CLDLET_INTERRUPTED:5] = amount of time spent in service by class 2 jobs
 *                                                                    sent directly at the cloud node
 *
 */
struct Area {
    double node;
    double cloudlet_node;
    double cloud_node;

    double service[6];
};

struct Probabilities {
    double cloudlet_class_1;                // #class_1 completed on cloudlet / tot
    double cloudlet_class_2;                // #class_2 completed on cloudlet / tot
    double cloud_class_2;                   // #class_2 completed on cloud (without interrupted jobs) / tot
    double cloud_class_1;                   // #class_1 completed on cloud / tot
    double cloud_class_2_interrupted;       // #class_2 interrupted / tot

    double cloudlet_class_1_on_1;           // #class_1 completed on cloudlet / #class_1
    double cloud_class_1_on_1;              // #class_1 completed on cloud / #class_1
    double cloudlet_class_2_on_2;           // class_2 completed on cloudlet / #class_2
    double cloud_class_2_on_2;              // class_2 completed on cloud (without interrupted jobs) / #class_2
    double cloud_class_2_interrupted_on_2;  // class_2 interrupted / class_2

};

struct Batch_stat {
    double avg_node;
    double avg_node_cloudlet;
    double avg_node_cloud;

    double service[5];
    double glb_service_class1;  //E[T] for class_1 jobs
    double glb_service_class2;  //E[T] for a class_2 jobs
    double glb_service;         //E[T] for any jobs

    double gbl_throughput_cloudlet;
    double gbl_throughput_cloud;
    double gbl_throughput;
};

struct End_mean {
    double node;
    double node_cloudlet;
    double node_cloud;

    double service[5];
    double glb_service_class1;  //E[T] for class_1 jobs
    double glb_service_class2;  //E[T] for a class_2 jobs
    double glb_service;         //E[T] for any jobs

    double gbl_throughput_cloudlet;
    double gbl_throughput_cloud;
    double gbl_throughput;
};

struct End_std {
    double node;
    double node_cloudlet;
    double node_cloud;

    double service[5];
    double glb_service_class1;
    double glb_service_class2;
    double glb_service;
};

struct Completed *completed;
struct Area *area;
struct Batch_stat *batch_stat;
struct End_mean *end_mean;
struct End_std *end_std;
struct Probabilities *probs;

void init_output_stats();

struct Batch_stat *compute_batch_service_time(int);

void compute_end_statistics();

void compute_batch_global_statistics(int);

void compute_glb_means_and_stds();

struct Probabilities *compute_probabilities(int);

double estimate_interval_endpoint(double);

void compute_job_number_mean();

void compute_throughput_mean();


