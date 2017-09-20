//
// Created by Ovidiu Daniel Barba on 16/02/2017.
//

#import <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int LOG = 1; /* if 1 log is enabled, 0 is disabled */


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

void log_debug(char *msg){
    if(LOG)
        printf("%s \n",msg);
}

void write_time_resp(FILE *fp, double time, double rt, double jobs){
    fprintf(fp,"%f; %f; %f\n", time , rt, jobs);

}

FILE *open_transient_file(){
    FILE *fp = fopen("../simulation_results/transient.csv","a+");
    if(fp == NULL){
        fprintf(stderr,"Opening transient results file problem \n");
        exit(EXIT_FAILURE);
    }
    return fp;
}


FILE * open_results_file(){

    FILE *fp = fopen("../simulation_results/res.csv","a+");
    if(fp == NULL){
        fprintf(stderr,"Opening simulation results file problem \n");
        exit(EXIT_FAILURE);
    }
    return fp;
}

FILE * open_resps_thts_file(){

    FILE *fp = fopen("../simulation_results/resps_thts.csv","a+");
    if(fp == NULL){
        fprintf(stderr,"Opening simulation results file problem \n");
        exit(EXIT_FAILURE);
    }
    return fp;
}

FILE *open_indep_test_results_file(long n){
    char buffer [50];
    char file_path[300];
    strcpy(file_path, "../simulation_results/ind_test_res");
    sprintf (buffer, "_n_%ld.csv" , n );

    strcat(file_path, buffer);
    printf(file_path);

    FILE *fp = fopen(file_path,"w+");
    if(fp == NULL){
        fprintf(stderr,"Opening independence results file problem \n");
        exit(EXIT_FAILURE);
    }
    return fp;
}

FILE *open_job_resp_times_file(){
    FILE *fp = fopen("./simulation_results/job_resp_times.csv","w+");
    if(fp == NULL){
        fprintf(stderr,"Opening job service times file problem \n");
        exit(EXIT_FAILURE);
    }
    return fp;
}

void write_s_resp_time_throu(FILE *fp, int S, double resp_time, double interval_r, double throughput, double interval_t,
                             double res0, double res1, double res2, double res3, double res4) {
    fprintf(fp, "%d; %f; %f; %f; %f; %f; %f; %f; %f; %f\n", S, resp_time, interval_r, throughput, interval_t, res0, res1, res2, res3, res4);
}

void write_s_min_resp_time(int S, int N, double resp){
    FILE *fp = fopen("../simulation_results/s_min_resp_time.csv","a+");
    if(fp == NULL){
        fprintf(stderr,"Opening s_min_resp_time.csv file problem \n");
        exit(EXIT_FAILURE);
    }

    fprintf(fp,"%d; %d; %f\n",S,N,resp);

    fclose(fp);
}


void write_job_resp_time(FILE *fp, double time){
    fprintf(fp,"%f\n",time);

}

void write_resps_thts_file(FILE *fp, int S, double res0, double res1, double res2, double res3, double res4, double res5, double res6,
                           double res7, double res8, double res9, double res10, double res11, double res12, double res13,
                           double res14, double res15){
    fprintf(fp,"%d;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f\n",S,
            res0,res1,res2,res3,res4,res5,res6,res7,res8,res9,res10,res11,
            res12,res13,res14,res15);

}

void write_convergence_file(int seed, int task_number, double time) {
    FILE *fp = fopen("./simulation_results/tasks_convergence.csv","a+");
    if(fp == NULL){
        fprintf(stderr,"Opening tasks_convergence.csv file problem \n");
        exit(EXIT_FAILURE);
    }

    fprintf(fp,"%d; %d; %f\n",seed,task_number,time);

    fclose(fp);
}



FILE *open_convergence_file() {
    FILE *fp = fopen("./simulation_results/tasks_convergence.csv","w+");
    if(fp == NULL){
        fprintf(stderr,"Opening convergence file problem \n");
        exit(EXIT_FAILURE);
    }
    return fp;
}

