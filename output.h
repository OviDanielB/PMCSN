//
// Created by emanuele on 07/09/17.
//

#ifndef PMCSN_OUTPUT_H
#define PMCSN_OUTPUT_H

#endif //PMCSN_OUTPUT_H

#include "utils/constants.h"

double update_batch_running_mean_response_time(double);

double update_batch_running_std_response_time(double);

double compute_throughput();

double compute_end_response_time_std();

double compute_end_throughput();

double estimate_interval_endpoint(double, double);




