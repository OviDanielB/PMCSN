//
// Created by Ovidiu Daniel Barba on 30/08/2017.
//

#include <stdio.h>
#include "../randomness_test/empirical_test_randomness.h"
#include "../utils/utils.h"

#define NUM_STREAMS 256


int main(int argc, char **argv){
    int k = 6, curr_stream;
    long n = 100000;
    double alpha = 0.05, v1, v2, curr_chi;
    FILE *fp;

    v2 = upper_critical_value(k, alpha);
    v1 = lower_critical_value(k, alpha);

    fp = open_indep_test_results_file( n );
    fprintf(fp,"%f\n",v1);
    fprintf(fp,"%f\n",v2);

    for(curr_stream = 0; curr_stream < NUM_STREAMS; curr_stream++) {
        curr_chi = run_up_test(k, alpha, n, curr_stream);
        fprintf(fp,"%f\n", curr_chi);
    }

    fclose(fp);


}