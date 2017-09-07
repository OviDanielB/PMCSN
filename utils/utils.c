//
// Created by Ovidiu Daniel Barba on 16/02/2017.
//

#import <math.h>

long fact(int x){
    int c;
    long fact = 1;

    if(x <= 0){
        return 1;
    }

    for(c = 1; c <= x; c++){
        fact = fact * c;
    }

    return fact;

}

int is_prime(long num)
/* ========================================
 * naive check of primality
 * returns 1 if num is prime, 0 otherwise
 * ========================================
 */
{
    if(num <= 1) return 0;
    if(num % 2 == 0 && num > 2) return 0; /* multiplier of 2 */
    for (int i = 3; i < floor(sqrt(num)) ; i++){
        if( num % i == 0) return 0; /* m is divisible by i so it's not prime */
    }
    return 1; /* is prime */
}

long gcd ( long a, long b )
/* ============================================
 * find the Greatest Common Divisor of a and b
 * ============================================
 */
{
    long c;
    while ( a != 0 ) {
        c = a; a = b%a;  b = c;
    }
    return b;
}
