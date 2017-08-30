//
// Created by Ovidiu Daniel Barba on 23/02/2017.
//
#include <stdio.h>


#include "../rng.h"

#define TEST_G_N 1000
#define PASS "PASSED"
#define NOT_PASS "NOT PASSED"

/* global variables */
long a = 48271; // modulus compatible
long m = 2147483647; // prime 2^31 - 1
long small_m = 13; /* prime */
long med_m = 401; /* prime */


int test_find_first_fp_mc(){
    long test_a, test_a1;

    test_a = find_first_fp_mc(&small_m);
    test_a1 = find_first_fp_mc(&med_m);

    /* 2 is the first fp_mc multiplier of m = 13  and 3 for m = 401 */
    if(test_a == 2 && test_a1 == 3){
        return 1;
    } else {
        return 0;
    }
}

int test_g(){

    int count = 0;
    int testPassed = 0;

    for( long x = 1; x <= TEST_G_N; x++){

        // g(x,a,m) should calculate a*x mod m
        if((a*x % m) == g(&x,&a,&m)){
            count++;
        }
    }

    if(count == TEST_G_N){
        testPassed = 1;
    }

    return testPassed;

}

int test_succ_full_period_modulus_compatible_multi(){

    int sm_a = 7;
    find_succ_full_period(&sm_a,&m,NULL);
}

int main(int argc, char **argv){

    long fp_mc_mult[5] = {7, 16807, 41214, 25697, 63295};

    printf("BEGINNING TEST SUITE\n");
    int i;
    for(i = 0; i < 5; i++){

        printf("TEST a = %ld is modulus compatible,full period multiplier to m = %ld : %s", fp_mc_mult[i], m, (full_period_check(&fp_mc_mult[i], &m)) ? PASS : NOT_PASS);

    }

    printf("TEST G(x,a,m) : %s \n", (test_g() == 1) ? PASS : NOT_PASS);
    printf("TEST find_first_fc_mc(m) : %s \n", (test_find_first_fp_mc() == 1) ? PASS : NOT_PASS);

}