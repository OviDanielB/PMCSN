//
// Created by emanuele on 07/09/17.
//

#ifndef PMCSN_OUTPUT_H
#define PMCSN_OUTPUT_H

#endif //PMCSN_OUTPUT_H

#include "utils/constants.h"

double t_star;

struct Batch_stat {
    long completions;               // # job completed
    double response_time_avg;       // (response_time_sum / completions )
    double response_time_std;       // standard deviation for E[T]
    double response_time_sum_sd;    // sum of squared deviations for Welford's algorithm

    double throughput;
};

struct End_stat {
    double response_time_avg;
    double response_time_std;

    double throughput;
};

struct Batch_stat *batch_stat;
struct End_stat *end_stat;

void init_output_stats();

double update_batch_running_mean_response_time(int, double);

double update_batch_running_std_response_time(int, double);

double compute_throughput(int);

void compute_end_response_time_stat();

double compute_end_throughput();

double estimate_interval_endpoint(double);




