//
// Created by emanuele on 07/09/17.
//

#include "output.h"

long batch_number = BATCH_NUMBER;
int current_batch;                  // batch in execution
double t_current;                    // current time
double batch_time;                   // time dedicated to one batch

double t_star = idfStudent(batch_number - 1, 1 - ALPHA / 2);

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

void initialize_batch_stat() {

    batch_stat = calloc(batch_number, sizeof(struct Batch_stat));
    if (batch_stat == NULL) {
        fprintf(stderr, "error in memory allocation\n");
        exit(EXIT_FAILURE);
    }
}

void initialize_end_stat() {

    end_stat = calloc(1, sizeof(struct End_stat));
    if (end_stat == NULL) {
        fprintf(stderr, "error in memory allocation\n");
        exit(EXIT_FAILURE);
    }
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
    return mean + ((value - *mean) / i);
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
 * @param value : new collected value
 * @return sample mean
 */
double update_batch_running_mean_response_time(double value) {
    return batch_stat[current_batch].response_time_avg = update_running_mean(
            batch_stat[current_batch].response_time_avg,
            value, batch_stat[current_batch],
            batch_stat[current_batch].completitions);
}

/**
 * Update running sample squared sum
 *
 * @param value : new collected value
 * @return standard deviation
 */
double update_batch_running_std_response_time(double value) {
    struct Batch_stat curr = batch_stat[current];
    curr.response_time_sum_sd = update_running_sample_sum_sd(curr.response_time_sum_sd, curr.response_time_avg, value,
                                                             curr.completions);
    return sqrt(curr.response_time_sum_sd / curr.completions);
}


/**
 * Welford's algorithm to compute standard deviation and sample mean on all batches
 * @return statistics on all batch
 */
double compute_end_response_time_std() {

    int i;
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
 * @return current throughput
 */
double compute_throughput() {
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

    return completions / t_current;
}

/**
 *
 * @param sample_mean
 * @param s : standard deviation
 * @param n : # element in the sample
 * @return endpoint for confidence interval
 */
double estimate_interval_endpoint(double sample_mean, double s) {
    return (t_star * s) / sqrt(batch_number - 1);
}




