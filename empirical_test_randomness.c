//
// Created by Ovidiu Daniel Barba on 30/08/2017.
//

#include <stdlib.h>
#include <stdio.h>

#include "rng_lib/rngs.h"
#include "utils.h"
#include "rng_lib/rvms.h"




/**
 * return the upper bound critical value v2*
 * @param k number of bins
 * @param alpha determines level of cofidence
 * @return
 */
double upper_critical_value(int k, double alpha){
    return idfChisquare(k - 1, 1 - alpha / 2);
}

/**
 * return the lower bound critical value v1*
 * @param k number of bins
 * @param alpha determines level of cofidence
 * @return
 */
double lower_critical_value(int k, double alpha){
    return idfChisquare(k - 1, alpha / 2);
}

/**
 * calculate the chi square statistic given the
 * observed o and expected n * p[x]
 * @param k number of bins
 * @param n number of run-ups
 * @param o observed
 * @param p expected / n
 * @return chi square statistic
 */
double chi_square_stat(int k,long n, long *o, double *p){

    int x;
    double chi = 0.0;

    for(x = 1; x <= k ; x++){
        chi = chi + (o[x] - n * p[x]) * (o[x] - n * p[x]) / (n * p[x]);
    }

    return chi;
}


/**
 * calculates array of probability of a run-up of length x
 * using p[x] = x / (x + 1)!
 * @param k length of array
 * @return probability array
 */
double *prob(int k){

    if(k <= 0){
        fprintf(stderr,"K = %d must be greater than 0! \n", k);
        exit(EXIT_FAILURE);
    }

    int x;
    double *p;

    p = malloc((k + 1) * sizeof(double));
    if(p == NULL){
        fprintf(stderr, "Malloc Error! \n");
        exit(EXIT_FAILURE);
    }

    for(x = 1; x <= k; x++){
        p[x] = x * 1.0 / fact(x + 1);
        printf("%f\n", p[x]);
    }

    return p;

}

/**
 * fills bins with number of occurences of
 * randomly generated numbers using the
 * RUN-UP TEST OF INDEPENDENCE
 * @param k number of bins
 * @param n number of tries
 * @return array of bin counts (length k + 1)
 */
long *fill_bins(int k,long n){

    /* bean index */
    int x;
    long i;
    /* pointer to array of bin counts, indexed by x */
    long *o;

    double u,t;

    /* k + 1 because index x starts at 1 */
    o = malloc((k + 1) * sizeof(double));
    if(o == NULL){
        fprintf(stderr, "Malloc Error");
    }

    /* initialize bin counts to 0 */
    for(x = 1; x <= k; x++){
        o[x] = 0;
    }

    for(i = 0; i < n; i++){
        x = 1;
        u = Random();
        t = Random();
        while(t > u){
            x++;
            u = t;
            t = Random();
        }

        if(x > k){
            x = k;
        }
        o[x]++;
    }

    for(x = 1; x <= k; x++){
        printf("BIN %d with value %ld \n", x, o[x]);
    }

    return o;
}


/**
 * execute a run-up test of indipendence
 * with relative params
 * @param k number of bins
 * @param alpha determines confidence level
 * @param n number of run-ups
 * @return
 */
int run_up_test(int k, double alpha, long n){
    long *o;
    double *p;

    double chi, v1, v2;

    printf("---------- RUN-UP TEST STARTING --------\n");
    printf("Params: {k : %d, n : %ld, alpha: %f} \n", k, n, alpha);

    o = fill_bins(k, n);
    p = prob(k);
    chi = chi_square_stat(k,n,o,p);

    printf("Chi-Square Statistic : %f \n", chi);

    v1 = lower_critical_value(k, alpha);
    v2 = upper_critical_value(k,alpha);

    printf("Critical values: v1 = %f and v2 = %f \n", v1, v2 );

    printf("TEST %s! \n", (chi >= v1 && chi <= v2) ? "PASSED" : "FAILED");
    printf("---------- TEST FINISHED ---------\n");
    return 0;

}