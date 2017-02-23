//
// Created by Ovidiu Daniel Barba on 23/02/2017.
//
#include <stdio.h>

#include "../rng.h"

#define TEST_G_N 1000

int test_g(){
    long a = 48271; // modulus compatible
    long m = 2147483647; // prime 2^31 - 1

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

int main(int argc, char **argv){

    printf("BEGINNING TEST SUITE\n");

    printf("TEST G(x,a,m) : %s \n", (test_g() == 1) ? "PASSED" : "NOT PASSED");

}