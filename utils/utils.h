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
FILE *open_job_resp_times_file();
FILE *open_indep_test_results_file(long n);
FILE *open_transient_file();
FILE *open_resps_thts_file();

void write_time_resp(FILE *fp, double time, double rt, double);
void write_s_min_resp_time(int,int,double);
void write_s_resp_time_throu(FILE *fp,int S, double, double, double, double, double , double , double , double, double);
void write_job_resp_time(FILE *fp, double time);
FILE *open_convergence_file();
void write_convergence_file(int, int, double);
void write_resps_thts_file(FILE *, int, double,double,double,double,double,double,double,double,double,double,double,double,double,double,double,double);