#include "output.h"

void init_output_stats() {

    completed = calloc((size_t) batch_number, sizeof(struct Completed));
    if (completed == NULL) {
        fprintf(stderr, "error in memory allocation\n");
        exit(EXIT_FAILURE);
    }

    area = calloc((size_t) batch_number, sizeof(struct Area));
    if (area == NULL) {
        fprintf(stderr, "error in memory allocation\n");
        exit(EXIT_FAILURE);
    }

    service = calloc((size_t) batch_number, sizeof(struct Service));
    if (service == NULL) {
        fprintf(stderr, "error in memory allocation\n");
        exit(EXIT_FAILURE);
    }

    batch_stat = calloc((size_t) batch_number, sizeof(struct Batch_stat));
    if (batch_stat == NULL) {
        fprintf(stderr, "error in memory allocation\n");
        exit(EXIT_FAILURE);
    }

    end_stat = calloc(1, sizeof(struct End_stat));
    if (end_stat == NULL) {
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
 *
 * @param sample_mean
 * @param s : standard deviation
 * @param n : # element in the sample
 * @return endpoint for confidence interval
 */
double estimate_interval_endpoint(double s) {
    return (t_star * s) / sqrt(batch_number - 1);
}



