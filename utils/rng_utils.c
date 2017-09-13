//
// Created by Ovidiu Daniel Barba on 16/02/2017.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "utils.h"



long g(long *x, long *a,long *m)
/* =================================================================
 * If m = aq+r is prime, r < q, and x ∈ Xm, then g(x) = axmodm
 * can be evaluated as follows without producing any intermediate
 * or final values larger than m − 1 in magnitude.
 * @return: g(x,a,m) = a*x mod m without overflow , 0 if m not prime
 * =================================================================
 */
{
    long q, r , t;

    if(! is_prime(*m)){
        fprintf(stderr,"Modulus m = %ld should be prime!!", *m);
        return 0;
    }

    q = (long) floor(*m / *a);
    r = (*m) % (*a);

   // printf(" G ==> %s \n", (*m == *a * q + r) ? "TRUE" :  "FALSE");
    t = *a * (*x % q) - r * ( (long) floor(*x / q) );

    //printf("G :   q = %ld, r= %ld, t = %ld , a*xmodm = %ld \n", q,r,t, *a * *x % *m);
    if(t > 0){
        return t;
    } else {
        return t + *m;
    }


}



int full_period_check(long *a, long *m)
/* =================================================================
 * This algorithm  can be used to determine
 * if a is a full-period multiplier relative to the prime modulus m
 * @return: 1 if a is a fp multiplier of m, 0 otherwise
 * =================================================================
 */
{
    long p = 1;
    long x = *a;
    long tmp;

    if(! is_prime(*m)){
        fprintf(stderr,"Modulus m = %ld should be positive! \n", *m);
        exit(EXIT_FAILURE);
    }

    while (x != 1) { /* if m not prime, loop may continue forever */
        p++;
        tmp = x;
        x = g(&tmp, a, m); /* avoid possible overflow  */
        //printf("a = %ld , m = %ld, g(x) = %ld, count = %ld\n", *a,*m, x,p);
        printf("completed %f percent \r", p * 1.0 / *m * 100.0);
    }

    if(p == *m - 1) return 1; /* a is a full period multiplier for m */

    return 0; /* a is not a full period multipliter for m */
}


int find_succ_full_period(long *a, long *m, long *fp_arr)
/* =================================================================
 * Given the prime modulus m and ANY full-period multiplier a,
 * the following algorithm generates all the full-period multipliers
 * relative to m
 * @fp_arr: returns pointer to array of fp multipliers of m
 * @return: 0 if not succeded, 1 otherwise
 * =================================================================
 */
{
    if( ! (is_prime(*m) && full_period_check(a,m)) ){
        fprintf(stderr, "Conditions for a = %ld and/or m = %ld not met. "
                "m should be prime and a a full-period multiplier of m!", *a, *m);
        return 1;
    }

    long i = 1, x = *a;

    while (x != 1){
        if( (gcd(i, *m - 1) == 1) && (*m % x < *m / x)){ /* x is a full-period multiplier of m */
            //  insert into list
            printf("x = %ld\n",x);
        }
        i++;
        x = g(&x,a,m); /* avoid possible overflow */
    }

    return 0;

}

long find_first_fp_mc(long *m)
/* =================================================================
 * This algorithm  can be used to determine
 * the first full-period (fp), modulus-compatible (mc) multiplier of
 * modulus m (prime)
 * @return: the first fp-mc multiplier of m, 0 if none found
 * =================================================================
 */
{
    long a, fp_mc_a;

    /* all mc multipliers a are usually small to m   a^2 < m -->  a < sqrt(m) */
    for(a = 2; a < floor(sqrt(*m)) ; a++){

        if(full_period_check(&a,m) == 1){
            fp_mc_a = a;
            return fp_mc_a;
        }
    }

    /* no fp-mc multiplier found */
    return 0;

}