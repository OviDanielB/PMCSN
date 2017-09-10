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

    batch_stat = calloc((size_t) batch_number, sizeof(struct Batch_stat));
    if (batch_stat == NULL) {
        fprintf(stderr, "error in memory allocation\n");
        exit(EXIT_FAILURE);
    }

    end_mean = calloc(1, sizeof(struct End_mean));
    if (end_mean == NULL) {
        fprintf(stderr, "error in memory allocation\n");
        exit(EXIT_FAILURE);
    }

    end_std = calloc(1, sizeof(struct End_std));
    if (end_std == NULL) {
        fprintf(stderr, "error in memory allocation\n");
        exit(EXIT_FAILURE);
    }

    probs = calloc(1, sizeof(struct Probabilities));
    if (probs == NULL) {
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
double update_running_mean(double *mean, double value, long i) {
    return *mean += ((value - (*mean)) / i);
}

/**
 * Welford's algorithm
 *
 * @param mean : current mean
 * @param value  : new value
 * @param i : current index
 * @return updated running sample sum of squared deviations
 */
double update_running_sample_sum_sd(double *v, double mean, double value, long i) {
    return *v += pow(value - mean, 2) * ((i - 1) / i);
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

struct Batch_stat *compute_batch_service_time(int batch) {

    int i;

    batch_stat[batch].service[CLDLET_CLASS_1] = area->service[CLDLET_CLASS_1] / completed->cloudlet_class_1;
    batch_stat[batch].service[CLDLET_CLASS_2] = area->service[CLDLET_CLASS_2] / completed->cloudlet_class_2;
    batch_stat[batch].service[CLOUD_CLASS_1] = area->service[CLOUD_CLASS_1] / completed->cloud_class_1;

    batch_stat[batch].service[CLOUD_CLASS_2] = (area->service[CLOUD_CLASS_2] - area->service[CLOUD_INTERRUPTED])
                                               / (completed->cloud_class_2 - completed->interrupted_class_2);

    batch_stat[batch].service[CLOUD_INTERRUPTED] = (area->service[CLOUD_INTERRUPTED] +
                                                    area->service[CLDLET_INTERRUPTED]) / completed->interrupted_class_2;

    for (i = 0; i < 5; i++) {
        int fp = fpclassify(batch_stat[batch].service[i]);
        if (fp == FP_NAN || fp == FP_INFINITE)
            batch_stat[batch].service[i] = 0;
    }

    return batch_stat;
}

struct Probabilities *compute_probabilities() {

    long tot = completed->cloudlet_class_1 + completed->cloudlet_class_2 + completed->cloud_class_1 +
               completed->cloud_class_2;


    long tot_class_2 = completed->cloud_class_2 + completed->cloudlet_class_2;
    long tot_class_1 = completed->cloud_class_1 + completed->cloudlet_class_1;

    probs->cloud_class_1 = (double) completed->cloud_class_1 / tot;
    probs->cloud_class_2 = (double) (completed->cloud_class_2 - completed->interrupted_class_2) / tot;
    probs->cloudlet_class_1 = (double) completed->cloudlet_class_1 / tot;
    probs->cloudlet_class_2 = (double) completed->cloudlet_class_2 / tot;
    probs->cloud_class_2_interrupted = (double) completed->interrupted_class_2 / tot;


    probs->cloud_class_1_on_1 = completed->cloud_class_1 / tot_class_1;
    probs->cloudlet_class_1_on_1 = completed->cloudlet_class_1 / tot_class_1;
    probs->cloud_class_2_on_2 = (completed->cloud_class_2 - completed->interrupted_class_2) / tot_class_2;
    probs->cloudlet_class_2_on_2 = completed->cloudlet_class_2 / tot_class_2;
    probs->cloud_class_2_interrupted_on_2 = completed->interrupted_class_2 / tot_class_2;

    return probs;
}

void compute_end_statistics() {

    int i, type;
    for (type = 0; type < 5; type++) {
        end_mean->service[type] = batch_stat[0].service[type];
        for (i = 1; i < batch_number; i++) {
            double mean = update_running_mean(&end_mean->service[type], batch_stat[i].service[type], i);
            update_running_sample_sum_sd(&end_std->service[type], mean, batch_stat[i].service[type], i);
        }
    }
    end_std->service[type] = sqrt(end_std->service[type] / i);

}

void compute_batch_global_statistics() {

    int i;
    for (i = 0; i < batch_number; i++) {
        double *service = batch_stat[i].service;

        batch_stat[i].glb_service_class1 = probs->cloudlet_class_1_on_1 * service[CLDLET_CLASS_1] +
                                           probs->cloud_class_1_on_1 * service[CLOUD_CLASS_1];

        batch_stat[i].glb_service_class2 = probs->cloudlet_class_2_on_2 * service[CLDLET_CLASS_2] +
                                           probs->cloud_class_2_on_2 * service[CLOUD_CLASS_2] +
                                           probs->cloud_class_2_interrupted_on_2 *
                                           (service[CLDLET_INTERRUPTED] + service[CLOUD_INTERRUPTED]);

        batch_stat[i].glb_service = probs->cloud_class_1 * service[CLOUD_CLASS_1] +
                                    probs->cloudlet_class_1 * service[CLDLET_CLASS_1] +
                                    probs->cloud_class_2 * service[CLOUD_CLASS_2] +
                                    probs->cloudlet_class_2 * service[CLDLET_CLASS_2] +
                                    probs->cloud_class_2_interrupted *
                                    (service[CLOUD_INTERRUPTED] + service[CLDLET_INTERRUPTED]);
    }

}

void compute_glb_means_and_stds() {
    int i;

    end_mean->glb_service_class1 = batch_stat[0].glb_service_class1;
    end_mean->glb_service_class2 = batch_stat[0].glb_service_class2;
    end_mean->glb_service = batch_stat[0].glb_service;

    for (i = 1; i < batch_number; i++) {
        double mean_class_1 = update_running_mean(&end_mean->glb_service_class1, batch_stat[i].glb_service_class1, i);
        update_running_sample_sum_sd(&end_std[i].glb_service_class1, mean_class_1, batch_stat[i].glb_service_class1, i);

        double mean_class_2 = update_running_mean(&end_mean->glb_service_class2, batch_stat[i].glb_service_class2, i);
        update_running_sample_sum_sd(&end_std[i].glb_service_class2, mean_class_2, batch_stat[i].glb_service_class2, i);

        double mean = update_running_mean(&end_mean->glb_service, batch_stat[i].glb_service, i);
        update_running_sample_sum_sd(&end_std[i].glb_service, mean, batch_stat[i].glb_service, i);
    }

    end_std->glb_service_class1 = sqrt(end_std->glb_service_class1 / i);
    end_std->glb_service_class2 = sqrt(end_std->glb_service_class2 / i);
    end_std->glb_service = sqrt(end_std->glb_service / i);

}



