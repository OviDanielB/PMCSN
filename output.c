//
// Created by emanuele on 07/09/17.
//

#include "output.h"

void init_output_stats() {
    end_stat = calloc(1, sizeof(struct End_stat));
    if (end_stat == NULL) {
        fprintf(stderr, "error in memory allocation\n");
        exit(EXIT_FAILURE);
    }

    batch_stat = calloc(batch_number, sizeof(struct Batch_stat));
    if (batch_stat == NULL) {
        fprintf(stderr, "error in memory allocation\n");
        exit(EXIT_FAILURE);
    }
    t_star = idfStudent(batch_number - 1, 1 - ALPHA / 2);
}

/*
 * PRIVATE
 */

/**
 *  Welford's algorithm
 *  Warning: Call first update_running_sample_sum_sd if used with it
 *
 * @param mean : current mean
 * @param value : new value
 * @param i : current index
 * @return updated mean
 */
double update_running_mean(double mean, double value, long i) {
    return mean + ((value - mean) / i);
}

/**
 * Welford's algorithm
 *
 * @param mean : current mean
 * @param value  : new value
 * @param i : current index
 * @return updated running sample sum of squared deviations
 */
double update_running_sample_sum_sd(double v, double mean, double value, long i) {
    return v + pow(value - mean, 2) * ((i - 1) / i);
}

/**
 *
 * @param values : sample on which calculate statistics
 * @param mean : sample mean
 * @param n : # element in the sample
 * @return sample standard deviation
 */
double standard_deviation(double *values, double mean, long n) {
    double std = 0.0;
    int i;
    for (i = 0; i < n; i++) {
        std += pow((values[i] - mean), 2) / n;
    }
    return sqrt(std);
}

/*
 * PUBLIC
 */

/**
 * Update sample mean with Welford's algorithm
 *
 * @param current_batch : index of the current batch
 * @param value : new collected value
 * @return sample mean
 */
double update_batch_running_mean_response_time(int current_batch, double value) {
    return batch_stat[current_batch].response_time_avg = update_running_mean(
            batch_stat[current_batch].response_time_avg, value, batch_stat[current_batch].completions);
}

/**
 * Update running sample squared sum
 *
 * @param current_batch : index of the current batch
 * @param value : new collected value
 * @return standard deviation
 */
double update_batch_running_std_response_time(int current_batch, double value) {
    struct Batch_stat curr = batch_stat[current_batch];
    curr.response_time_sum_sd = update_running_sample_sum_sd(curr.response_time_sum_sd, curr.response_time_avg, value,
                                                             curr.completions);
    return sqrt(curr.response_time_sum_sd / curr.completions);
}


/**
 * Welford's algorithm to compute standard deviation and sample mean on all batches
 * @return statistics on all batch
 */
void compute_end_response_time_stat() {

    int i, index= 0;
    double mean = batch_stat[0].response_time_avg;
    double sum = 0.0;
    double diff;
    double stdev;

    for (i = 1; i < batch_number; i++) {
        index = i + 1;
        struct Batch_stat *curr = &batch_stat[i];
        diff = curr->response_time_avg - mean;
        sum += pow(diff, 2) * (index - 1 / index);
        mean += diff / (index + 1);
    }

    stdev = sqrt(sum / index);

    end_stat->response_time_std = stdev;
    end_stat->response_time_avg = mean;
}

/**
 * Throughput for the current batch
 * @param current_batch : index of the batch in execution
 *
 * @return current throughput
 */
double compute_throughput(int current_batch) {
    return batch_stat[current_batch].completions / (batch_time * (current_batch + 1));
}

/**
 *
 * @return total throughput
 */
double compute_end_throughput() {
    int i;
    long completions;
    struct Batch_stat curr;

    for (i = 0; i < batch_number; i++) {
        curr = batch_stat[i];
        completions += curr.completions;
    }

    return completions / (batch_number * batch_time);
}

/**
 *
 * @param sample_mean
 * @param s : standard deviation
 * @param n : # element in the sample
 * @return endpoint for confidence interval
 */
double estimate_interval_endpoint(double s) {
    return (t_star * s) / sqrt(batch_number - 1);
}




