//
// Created by Ovidiu Daniel Barba on 30/08/2017.
//

#include <stdio.h>
#include "../empirical_test_randomness.h"


int main(int argc, char **argv){
    int k = 6;
    long n = 7200;
    double alpha = 0.05;

    run_up_test(k,alpha,n);


}