//
// Created by Ovidiu Daniel Barba on 16/02/2017.
//

#ifndef PMCSN_UTILS_H
#define PMCSN_UTILS_H

#endif //PMCSN_UTILS_H


int is_prime(long num);
long gcd(long a,long b);
long fact(int x);

void log_debug(char * msg);

FILE *open_results_file();

void write_s_resp_time_throu(FILE *fp,int S, double resp_time, double throughput, double , double , double , double, double);