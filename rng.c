//
// Created by Ovidiu Daniel Barba on 16/02/2017.
//

#include <stdio.h>
#include <stdlib.h>

#include "utils.h"



int full_period_check(long a, long m)
/* =================================================================
 * This algorithm  can be used to determine
 * if a is a full-period multiplier relative to the prime modulus m
 * =================================================================
 */
{
    long p = 1;
    long x = a;

    if(! is_prime(m)){
        fprintf(stderr,"Modulus m = %ld should be positive! \n", m);
        exit(EXIT_FAILURE);
    }

    while (x != 1) { /* if m not prime, loop may continue forever */
        p++;
        x = (a * x) % m;
        printf("%ld\n",x);
    }

    if(p == m-1) return 1; /* a is a full period multiplier for m */

    return 0; /* a is not a full period multipliter for m */
}