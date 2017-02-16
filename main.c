#include <stdio.h>

#include "utils.h"
#include "rng.h"

int main() {
    printf(" %d %d \n", is_prime(231), full_period_check(21, 213));
    return 0;
}