//
// Created by Ovidiu Daniel Barba on 07/09/2017.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "../rng_lib/rvgs.h"
#include "../rng_lib/rngs.h"
#include "../rng_lib/rvms.h"

#include "utils.h"
#include "../event_queue.h"

#ifndef PMCSN_CONSTANTS_H
#define PMCSN_CONSTANTS_H
#endif //PMCSN_CONSTANTS_H



#define CLASS_1_ARRIVAL_RATE            3.25    /* task/s */
#define CLASS_2_ARRIVAL_RATE            6.25    /* task/s */
#define CLASS_1_CLOUDLET_SERVICE_RATE   0.45    /* task/s */
#define CLASS_2_CLOUDLET_SERVICE_RATE   0.30    /* task/s */
#define CLASS_1_CLOUD_SERVICE_RATE      0.25    /* task/s */
#define CLASS_2_CLOUD_SERVICE_RATE      0.22    /* task/s */
#define SETUP_TIME_MEAN                 0.80    /* s */


/**
 * random number generator streams for each
 * event (class 1 arrival, class 2 cloud service, etc)
 */
#define ARRIVAL_CLASS_1_STREAM              0
#define ARRIVAL_CLASS_2_STREAM              1
#define CLOUDLET_SERVICE_CLASS_1_STREAM     2
#define CLOUDLET_SERVICE_CLASS_2_STREAM     3
#define CLOUD_SERVICE_CLASS_1_STREAM        4
#define CLOUD_SERVICE_CLASS_2_STREAM        5
#define SETUP_STREAM                        6


/**
 * simulation parameters:
 * start time, stop time, etc
 */
#define START         0.0              /* initial time                   */
#define STOP      20000.0              /* terminal (close the door) time */
#define INFINITO   (100.0 * STOP)      /* must be much larger than STOP  */
#define SEED      1234567
#define ALPHA       0.5
/**
 * event types
 */
#define EVENT_CLASS_1_ARRIVAL                           0
#define EVENT_CLASS_1_CLOUDLET_COMPLETION               1
#define EVENT_CLASS_1_CLOUD_COMPLETION                  2
#define EVENT_CLASS_2_ARRIVAL                           3
#define EVENT_CLASS_2_CLOUDLET_COMPLETION               4
#define EVENT_CLASS_2_CLOUD_COMPLETION                  5
#define EVENT_CLASS_2_SETUP                             6


#define ACCEPT_CLASS_1_ON_CLOUDLET                      0
#define SEND_CLASS_1_TO_CLOUD                           1
#define INTERRUPT_CLASS_2_ON_CLOUDLET_AND_SEND_TO_CLOUD 2
#define ACCEPT_CLASS_2_ON_CLOUDLET                      3
#define SEND_CLASS_2_TO_CLOUD                           4

/**
 * Batch means parameters
 */
long batch_number;  // #batches
double batch_time;  // time dedicated to one batch

int N;              // number of servers
int S;              // threshold value


