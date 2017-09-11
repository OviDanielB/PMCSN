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
double update_running_mean(double mean, double value, long i) {
    return mean + (1.0 * (value - mean) / i);
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
    return v + pow(value - mean, 2) * (1.0 * (i - 1) / i);
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
    return (t_star * s) / sqrt(1.0 * batch_number - 1);
}

struct Batch_stat *compute_batch_service_time(int batch) {

    int i;

    batch_stat[batch].service[CLDLET_CLASS_1] =
            1.0 * area[batch].service[CLDLET_CLASS_1] / completed[batch].cloudlet_class_1;
    batch_stat[batch].service[CLDLET_CLASS_2] =
            1.0 * area[batch].service[CLDLET_CLASS_2] / completed[batch].cloudlet_class_2;
    batch_stat[batch].service[CLOUD_CLASS_1] =
            1.0 * area[batch].service[CLOUD_CLASS_1] / completed[batch].cloud_class_1;

    batch_stat[batch].service[CLOUD_CLASS_2] = 1.0 * (area[batch].service[CLOUD_CLASS_2] -
                                                      area[batch].service[CLOUD_INTERRUPTED])
                                               /
                                               (completed[batch].cloud_class_2 - completed[batch].interrupted_class_2);

    batch_stat[batch].service[CLOUD_INTERRUPTED] = 1.0 * (area[batch].service[CLOUD_INTERRUPTED] +
                                                          area[batch].service[CLDLET_INTERRUPTED]) /
                                                   completed[batch].interrupted_class_2;


    for (i = 0; i < 5; i++) {
        int fp = fpclassify(batch_stat[batch].service[i]);
        if (fp == FP_NAN || fp == FP_INFINITE)
            batch_stat[batch].service[i] = 0;
    }

    return batch_stat;
}

struct Probabilities *compute_probabilities() {
    int i;
    long tot = 0, tot_class_2 = 0, tot_class_1 = 0;
    for (i = 0; i < batch_number; i++) {
        tot += completed[i].cloudlet_class_1 + completed[i].cloudlet_class_2 + completed[i].cloud_class_1 +
               completed[i].cloud_class_2;

        tot_class_2 += completed[i].cloud_class_2 + completed[i].cloudlet_class_2;

        tot_class_1 += completed[i].cloud_class_1 + completed[i].cloudlet_class_1;
    }

    for (i = 0; i < batch_number; i++) {
        probs->cloud_class_1 += 1.0 * completed[i].cloud_class_1 / tot;
        probs->cloud_class_2 += 1.0 * (completed[i].cloud_class_2 - completed[i].interrupted_class_2) / tot;
        probs->cloudlet_class_1 += 1.0 * completed[i].cloudlet_class_1 / tot;
        probs->cloudlet_class_2 += 1.0 * completed[i].cloudlet_class_2 / tot;
        probs->cloud_class_2_interrupted += 1.0 * completed[i].interrupted_class_2 / tot;


        probs->cloud_class_1_on_1 += 1.0 * completed[i].cloud_class_1 / tot_class_1;
        probs->cloudlet_class_1_on_1 += 1.0 * completed[i].cloudlet_class_1 / tot_class_1;
        probs->cloud_class_2_on_2 +=
                1.0 * (completed[i].cloud_class_2 - completed[i].interrupted_class_2) / tot_class_2;
        probs->cloudlet_class_2_on_2 += 1.0 * completed[i].cloudlet_class_2 / tot_class_2;
        probs->cloud_class_2_interrupted_on_2 += 1.0 * completed[i].interrupted_class_2 / tot_class_2;
    }

    return probs;
}

void compute_end_statistics() {

    int i = 0, type;
    for (type = 0; type < 5; type++) {
        end_mean->service[type] = batch_stat[0].service[type];
        for (i = 1; i < batch_number; i++) {
            end_mean->service[type] = update_running_mean(end_mean->service[type], batch_stat[i].service[type], i);
            end_std->service[type] = update_running_sample_sum_sd(end_std->service[type], end_mean->service[type],
                                                                  batch_stat[i].service[type], i);

        }
        end_std->service[type] = sqrt(1.0 * end_std->service[type] / i);
    }
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

        printf("batch %d mean service time: %f\n", i, batch_stat[i].glb_service);
        printf("batch %d mean service time for a class 1 job: %f\n", i, batch_stat[i].glb_service_class1);
        printf("batch %d mean service time for a class 2 job: %f\n", i, batch_stat[i].glb_service_class2);

    }

}

void compute_glb_means_and_stds() {
    int i;

    end_mean->glb_service_class1 = batch_stat[0].glb_service_class1;
    end_mean->glb_service_class2 = batch_stat[0].glb_service_class2;
    end_mean->glb_service = batch_stat[0].glb_service;

    for (i = 1; i < batch_number; i++) {
        end_mean->glb_service_class1 = update_running_mean(end_mean->glb_service_class1,
                                                           batch_stat[i].glb_service_class1, i);
        end_std[i].glb_service_class1 = update_running_sample_sum_sd(end_std[i].glb_service_class1,
                                                                     end_mean->glb_service_class1,
                                                                     batch_stat[i].glb_service_class1, i);

        end_mean->glb_service_class2 = update_running_mean(end_mean->glb_service_class2,
                                                           batch_stat[i].glb_service_class2, i);
        end_std[i].glb_service_class2 = update_running_sample_sum_sd(end_std[i].glb_service_class2,
                                                                     end_mean->glb_service_class2,
                                                                     batch_stat[i].glb_service_class2, i);

        end_mean->glb_service = update_running_mean(end_mean->glb_service, batch_stat[i].glb_service, i);
        end_std[i].glb_service = update_running_sample_sum_sd(end_std[i].glb_service, end_mean->glb_service,
                                                              batch_stat[i].glb_service, i);
    }

    end_std->glb_service_class1 = sqrt(1.0 * end_std->glb_service_class1 / i);
    end_std->glb_service_class2 = sqrt(1.0 * end_std->glb_service_class2 / i);
    end_std->glb_service = sqrt(1.0 * end_std->glb_service / i);
}



