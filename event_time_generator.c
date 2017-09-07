
/**
 * All calls to Exponential have (1 / rate) as the parameter
 * since the Exponential function has the mean as the parameter
 */

#include "utils/constants.h"


/**
 * get next arrival time of class 1 tasks
 * @return time
 */
double getArrivalClass1(){

    // TODO arrival distribution may not be Exponential!!

    static double arrival = START;
    SelectStream(ARRIVAL_CLASS_1_STREAM);
    arrival += Exponential( 1 / CLASS_1_ARRIVAL_RATE);
    return arrival;
}


/**
 * get next arrival time of class 2 tasks
 * @return time
 */
double getArrivalClass2(){

    // TODO arrival distribution may not be Exponential!!

    static double arrival = START;
    SelectStream(ARRIVAL_CLASS_2_STREAM);
    arrival += Exponential( 1 / CLASS_2_ARRIVAL_RATE);
    return arrival;

}


/**
 * get class 1 service time on the cloudlet
 * @return service time
 */
double getServiceClass1Cloudlet(){

    SelectStream(CLOUDLET_SERVICE_CLASS_1_STREAM);
    return Exponential(1 / CLASS_1_CLOUDLET_SERVICE_RATE);

}

/**
 * get class 2 service time on the cloudlet
 * @return service time
 */
double getServiceClass2Cloudlet(){

    SelectStream(CLOUDLET_SERVICE_CLASS_2_STREAM);
    return Exponential(1 / CLASS_2_CLOUDLET_SERVICE_RATE);
}

/**
 * get class 1 service time on the cloud
 * @return service time
 */
double getServiceClass1Cloud(){

    SelectStream(CLOUD_SERVICE_CLASS_1_STREAM);
    return Exponential(1 / CLASS_1_CLOUD_SERVICE_RATE);
}


/**
 * get class 2 service time on the cloud
 * @return service time
 */
double getServiceClass2Cloud(){

    SelectStream(CLOUD_SERVICE_CLASS_2_STREAM);
    return Exponential(1 / CLASS_2_CLOUD_SERVICE_RATE);
}


/**
 * get the setup time of a class 2 task after being
 * interrupted from the cloudlet,moved to the cloud
 * and restarted
 * @return
 */
double getSetup(){

    SelectStream(SETUP_STREAM);
    return Exponential(SETUP_TIME_MEAN);

}