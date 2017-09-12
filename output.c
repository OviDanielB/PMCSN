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

    probs = calloc((size_t) batch_number, sizeof(struct Probabilities));
    if (probs == NULL) {
        fprintf(stderr, "error in memory allocation\n");
        exit(EXIT_FAILURE);
    }

    generated = calloc(1, sizeof(struct Batch_stat));
    if (generated == NULL) {
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
            1.0 * (area[batch].service[CLDLET_CLASS_2]) / completed[batch].cloudlet_class_2;

    batch_stat[batch].service[CLOUD_CLASS_1] =
            1.0 * area[batch].service[CLOUD_CLASS_1] / completed[batch].cloud_class_1;

    batch_stat[batch].service[CLOUD_CLASS_2] =
            1.0 * area[batch].service[CLOUD_CLASS_2] / completed[batch].cloud_class_2;

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

struct Probabilities *compute_probabilities(int batch) {

    long tot = completed[batch].cloudlet_class_1 + completed[batch].cloudlet_class_2 + completed[batch].cloud_class_1 +
               completed[batch].cloud_class_2;

    long tot_class_2 = completed[batch].cloud_class_2 + completed[batch].cloudlet_class_2;

    long tot_class_1 = completed[batch].cloud_class_1 + completed[batch].cloudlet_class_1;


    probs[batch].cloud_class_1 += 1.0 * completed[batch].cloud_class_1 / tot;
    probs[batch].cloud_class_2 += 1.0 * (completed[batch].cloud_class_2) / tot;
    probs[batch].cloudlet_class_1 += 1.0 * completed[batch].cloudlet_class_1 / tot;
    probs[batch].cloudlet_class_2 += 1.0 * completed[batch].cloudlet_class_2 / tot;

    //not used very much
    probs[batch].cloud_class_2_interrupted += 1.0 * completed[batch].interrupted_class_2 / tot;

    probs[batch].cloud_class_1_on_1 += 1.0 * completed[batch].cloud_class_1 / tot_class_1;
    probs[batch].cloudlet_class_1_on_1 += 1.0 * completed[batch].cloudlet_class_1 / tot_class_1;
    probs[batch].cloud_class_2_on_2 +=
            1.0 * (completed[batch].cloud_class_2) / tot_class_2;
    probs[batch].cloudlet_class_2_on_2 += 1.0 * completed[batch].cloudlet_class_2 / tot_class_2;
    probs[batch].cloud_class_2_interrupted_on_2 += 1.0 * completed[batch].interrupted_class_2 / tot_class_2;


//    probs[batch].p11 = 1.0 * completed->cloudlet_class_1 / (completed->cloudlet_class_1 + completed->cloudlet_class_2);
//    probs[batch].p12 = 1.0 * completed->cloudlet_class_2 / (completed->cloudlet_class_1 + completed->cloudlet_class_2);

    probs[batch].p11 = 1.0 * generated->cloudlet_class_1 / (generated->cloudlet_class_1 + generated->cloudlet_class_2);
    probs[batch].p12 = 1.0 * generated->cloudlet_class_2 / (generated->cloudlet_class_1 + generated->cloudlet_class_2);

    return probs;

}

void compute_utilization(int batch) {

    double service_cloudlet = probs[batch].cloudlet_class_1 * batch_stat[batch].service[CLDLET_CLASS_1] +
                              probs[batch].cloudlet_class_2 * batch_stat[batch].service[CLDLET_CLASS_2];

    batch_stat[batch].ro = 1.0 * (probs[batch].p11 * CLASS_1_ARRIVAL_RATE + probs[batch].p12 * CLASS_2_ARRIVAL_RATE) *
                           service_cloudlet / N;
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

void compute_batch_global_statistics(int batch) {

    double *service = batch_stat[batch].service;

    batch_stat[batch].glb_service_class1 = probs[batch].cloudlet_class_1_on_1 * service[CLDLET_CLASS_1] +
                                           probs[batch].cloud_class_1_on_1 * service[CLOUD_CLASS_1];

    batch_stat[batch].glb_service_class2 = probs[batch].cloudlet_class_2_on_2 * service[CLDLET_CLASS_2] +
                                           probs[batch].cloud_class_2_on_2 * service[CLOUD_CLASS_2];

    batch_stat[batch].glb_service = probs[batch].cloud_class_1 * service[CLOUD_CLASS_1] +
                                    probs[batch].cloudlet_class_1 * service[CLDLET_CLASS_1] +
                                    probs[batch].cloud_class_2 * service[CLOUD_CLASS_2] +
                                    probs[batch].cloudlet_class_2 * service[CLDLET_CLASS_2];


    batch_stat[batch].gbl_throughput_cloudlet =
            (completed[batch].cloudlet_class_1 + completed[batch].cloudlet_class_2) / batch_time;

    batch_stat[batch].gbl_throughput_cloud =
            (completed[batch].cloud_class_1 + completed[batch].cloud_class_2)
            / batch_time;

    batch_stat[batch].gbl_throughput =
            (completed[batch].cloudlet_class_1 + completed[batch].cloud_class_1
             + completed[batch].cloudlet_class_2 + completed[batch].cloud_class_2) / batch_time;
}

void compute_glb_means_and_stds() {
    int i;

    end_mean->glb_service_class1 = batch_stat[0].glb_service_class1;
    end_mean->glb_service_class2 = batch_stat[0].glb_service_class2;
    end_mean->glb_service = batch_stat[0].glb_service;
    end_mean->ro = batch_stat[0].glb_service;

    for (i = 1; i < batch_number; i++) {
        end_mean->glb_service_class1 = update_running_mean(end_mean->glb_service_class1,
                                                           batch_stat[i].glb_service_class1, i);
        end_std->glb_service_class1 = update_running_sample_sum_sd(end_std->glb_service_class1,
                                                                   end_mean->glb_service_class1,
                                                                   batch_stat[i].glb_service_class1, i);

        end_mean->glb_service_class2 = update_running_mean(end_mean->glb_service_class2,
                                                           batch_stat[i].glb_service_class2, i);
        end_std->glb_service_class2 = update_running_sample_sum_sd(end_std->glb_service_class2,
                                                                   end_mean->glb_service_class2,
                                                                   batch_stat[i].glb_service_class2, i);

        end_mean->glb_service = update_running_mean(end_mean->glb_service, batch_stat[i].glb_service, i);
        end_std->glb_service = update_running_sample_sum_sd(end_std->glb_service, end_mean->glb_service,
                                                            batch_stat[i].glb_service, i);

        end_mean->ro = update_running_mean(end_mean->ro, batch_stat[i].ro, i);
        end_std->ro = update_running_sample_sum_sd(end_std->ro, end_mean->ro,
                                                            batch_stat[i].ro, i);
    }

    end_std->glb_service_class1 = sqrt(1.0 * end_std->glb_service_class1 / i);
    end_std->glb_service_class2 = sqrt(1.0 * end_std->glb_service_class2 / i);
    end_std->glb_service = sqrt(1.0 * end_std->glb_service / i);
}

void compute_job_number_mean() {

    int i;
    end_mean->node = batch_stat[0].avg_node;
    end_mean->node_cloudlet = batch_stat[0].avg_node_cloudlet;
    end_mean->node_cloud = batch_stat[0].avg_node_cloud;
    for (i = 1; i < batch_number; i++) {
        end_mean->node = update_running_mean(end_mean->node, batch_stat[i].avg_node, i);
        end_std->node = update_running_sample_sum_sd(end_std->node, end_mean->node,
                                                     batch_stat[i].avg_node, i);

        end_mean->node_cloudlet = update_running_mean(end_mean->node_cloudlet, batch_stat[i].avg_node_cloudlet, i);
        end_std->node_cloudlet = update_running_sample_sum_sd(end_std->node_cloudlet, end_mean->node_cloudlet,
                                                              batch_stat[i].avg_node_cloudlet, i);

        end_mean->node_cloud = update_running_mean(end_mean->node_cloud, batch_stat[i].avg_node_cloud, i);
        end_std->node_cloud = update_running_sample_sum_sd(end_std->node_cloud, end_mean->node_cloud,
                                                           batch_stat[i].avg_node_cloud, i);
    }
    end_std->node = sqrt(1.0 * end_std->node / i);
    end_std->node_cloudlet = sqrt(1.0 * end_std->node_cloudlet / i);
    end_std->node_cloud = sqrt(1.0 * end_std->node_cloud / i);
}

void compute_throughput_mean() {
    int i = 0;
    double sum1 = 0, sum2 = 0, sum3 = 0;

    for (i = 0; i < batch_number; i++) {
        sum1 += batch_stat[i].gbl_throughput;
        sum2 += batch_stat[i].gbl_throughput_cloudlet;
        sum3 += batch_stat[i].gbl_throughput_cloud;
    }

    end_mean->gbl_throughput = 1.0 * sum1 / batch_number;
    end_mean->gbl_throughput_cloudlet = 1.0 * sum2 / batch_number;
    end_mean->gbl_throughput_cloud = 1.0 * sum3 / batch_number;
}

