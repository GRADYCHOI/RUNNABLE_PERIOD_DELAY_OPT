#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>
#include <stdbool.h>


/* default options */
#define N_MAX 20


enum dag_type {A, B, C, D};
//enum property {DRP, WP, DSP};
//enum execution_type {TE, DE};

static int parse_options(int argc, char *argv[], int *dag_type, int e[]);
static int run_mapping(int dag_type, int e[], int p[], long secs, long usecs);
static int get_n(int dag_type);
static int Delay_C(int e[], int p[], int dag_type);
static int Delay_R(int e[], int p[], int dag_type);
int lcm(int a,int b);


int main(int argc, char *argv[]) {

    int e[argc-2];
    int p[argc-2];
    int dag_type;
    int map;
    int delay_c = 0, delay_r = 0;
    long secs, usecs;


    map = parse_options(argc, argv, &dag_type, e);

    if (map == 0) {
        map = run_mapping(dag_type, e, p, secs, usecs);
    }
    if (delay_c == 0) {
        delay_c = Delay_C(e, p, dag_type);
        delay_r = Delay_R(e, p, dag_type);  
    }
    else if (map <0) {
        return EXIT_FAILURE;
    }

}

static int parse_options(int argc, char *argv[], int *dag_type, int e[]) {
    //parse a parameters and setting an options.
    int i, n, opt;

    if (strcmp(argv[optind], "a") == 0 || strcmp(argv[optind], "A") == 0) {
        *dag_type = A;
        printf("DAG Type : %s\n", argv[optind]);
    }
    else if (strcmp(argv[optind], "b") == 0 || strcmp(argv[optind], "B") == 0) {
        *dag_type = B;
        printf("DAG Type : %s\n", argv[optind]);
    }
    else if (strcmp(argv[optind], "c") == 0 || strcmp(argv[optind], "C") == 0) {
        *dag_type = C;
        printf("DAG Type : %s\n", argv[optind]);
    }
    else if (strcmp(argv[optind], "d") == 0 || strcmp(argv[optind], "D") == 0) {
        *dag_type = D;
        printf("DAG Type : %s\n", argv[optind]);
    }
    else {
        fprintf(stderr, "DAG Type is not define");
        return -1;
    }
    optind++;
    n = get_n(*dag_type);
    if (n < 0) {
        fprintf(stderr, "DAG Type Error, %s is not define\n", argv[optind - 1]);
        return -1;
    }
    if (n > argc - optind) {
        fprintf(stderr, "%s Type requires %d execution tiems\n", argv[optind - 1], n);
        return -1;
    }
    else if (n < argc - optind) {
        fprintf(stderr, "%s Type requires %d execution tiems\n", argv[optind -1], n);
        return -1;
    }
    for (i = 1 ; i <= argc-2 ; i++) {
        e[i] = atof(argv[optind++]);
    }
    return 0;
}

static int get_n(int dag_type) { 
    //dag type convert to number.
    switch (dag_type) {
    case A:
        return 4;
    case B:
        return 5;
    case C:
        return 6;
    case D:
        return 7;
    default:
        return -1;
    }
}

static int run_mapping(int dag_type, int e[], int p[], long secs, long usecs) {
    //Runnable mapped roughest period Task.
    printf("-----------------------------------\n");
    printf("initial runnable mapping in tasks\n");

    int i, map;
    int n = get_n(dag_type);
    double Schedulability = 0;
    double utilization = 0;
    double total_execution_time = 0;
    double total_period = 0;
    double update_period = 0;
    bool opt_period = true;
    int max_period = 1;
    int max_execution = 0;

    for (i = 1 ; i <= n ; i++) {
        if (e[i] <= 5) {
            p[i] = 5;
        }
        else if ((e[i] > 5) && (e[i] <= 10)) {
            p[i] = 10;
        }
        else if ((e[i] > 10) && (e[i] <= 20)) {
            p[i] = 20;
        }
        else if ((e[i] > 20) && (e[i] <= 30)) {
            p[i] = 30;
        }
    }

    for (int i = 1; i <= n; i++) {
        utilization += (float)e[i]/p[i];        
    }
   
    printf("Utilization : %f      \n", utilization);
    printf("-----------------------------------\n");

    while (opt_period) {
       
        if (utilization > 0.67){ // 유틸리제이션이 0.67 이상이면 period 업데이트 시작. RM을 위해.
            int max_num = 0;
            double run_util = 0;
            int num[n];
            int array_num = 0;
            for (int i = 1 ; i <= n ; i++) { 
                if ((float)e[i]/p[i] > run_util){
                    run_util = (float)e[i]/p[i];
                    for (int j = 0 ; j <= n ; j++) {
                        num[j] = 0;
                    }
                    num[i] = 1;
                }
                else if ((float)e[i]/p[i] == run_util) {
                    num[i] = 1;
                }
            }
            //printf("largest runnable utilization rate : %f\n", run_util);
            for (int i = 0 ; i <= n ; i++) { // <--------------------jojue
                if (num[i] == 1){
                    p[i] += 5;
                }
            }
            
            utilization = 0;
            for (int k = 1 ; k <= n ; k++) {
                utilization += (float)e[k]/p[k];
            } 
            printf("Utilization : %f \n", utilization);
            Delay_C(e, p, dag_type);
            printf("-------------------------------------------\n");
        }
        else {
            printf("Utilization : %f , Optimazation Complet!\n", utilization);
            for (int j = 1 ; j <= n ; j++) {
                printf("%d task execution time : %dms,  period : %dms\n", j, e[j], p[j]);
            }
            opt_period = false;
        }
    }
    return 0;
}

static int Delay_C(int e[], int p[], int dag_type) {
    
    int n = get_n(dag_type);
    int count = 0;
    int cycle_count = 0;
    int sub_delay_time = 0;
    int sysnc_count = 0;
    int worst_delay = 0;
    int priority[n];
    int temp;

    for (int i = 1; i < n; i++) {
        for (int j = 1+1; j <= n; j++) {
            if (e[i] > e[j]) {
                priority[i-1] = e[j];
            }
        }
    }
    for (int i = 0; i <= n; i++) {
        printf("%d\n", priority[i]);
    }

    bool period1 = false;
    bool period2 = false;
    bool period3 = false;
    bool period4 = false;
    bool period5 = false;
    bool period6 = false;

    bool depend1 = false;
    bool depend2 = false;
    bool depend3 = false;
    bool depend4 = false;
    bool depend5 = false;
    bool depend6 = false;


    while (count < 750){
	    //printf("  %d ms  \n", count);
	    if (count % p[1] == 0) { 
		    period1 = true;
	    }
	    if (count % p[2] == 0) { 
		    period2 = true;
	    }
	    if (count % p[3] == 0) {
		    period3 = true; 
	    }
	    if (count % p[4] == 0) {
		    period4 = true; 
	    }
	    //printf(" %d %d %d %d\n", period1, period2, period3, period4);

	    if (sysnc_count == count) { 
		    if (period1 == 1) {
                //printf("1");
			    if (depend1 == false) {
				    depend1 = true;
				    cycle_count = 0 + e[1];
				    sysnc_count = sysnc_count + e[1];
				    period1 = false;
				    //printf("just run 1\n");
				    count ++;
			    }
			    else if ((depend1 == true) && (depend2 == true)) {
				    cycle_count = cycle_count + e[1];
				    sysnc_count = sysnc_count + e[1];
				    sub_delay_time += e[1]; // sub delay timer start
				    period1 = false;
				    //printf("twice run 1\n");
				    count ++;
			    }
			    else if ((depend1 == true) && (depend2 == false)) {
				    depend1 = true;
				    cycle_count = 0 + e[1];
				    sysnc_count = sysnc_count + e[1];
				    period1 = false;
				    //printf("renew run 1\n");
				    count ++;
			    }
		    }

		    else if (period2 == 1) {
			    if ((depend2 == false) && (depend1 == true)) {
				    depend2 = true;
				    cycle_count = cycle_count + e[2];
				    sysnc_count = sysnc_count + e[2];
				    period2 = false;
				    //printf("run 2\n");
				    count ++;
				    if (sub_delay_time) {
					    sub_delay_time += e[2];
				    }
			    }
			    else if (depend2 == true) {
				    cycle_count += e[2];
				    sysnc_count += e[2];
				    period2 = false;
				    //printf("run 2 again\n");
				    count ++;
				    if (sub_delay_time) {
					    sub_delay_time += e[2];
				    }
			    }
			    else {
				    cycle_count += e[2];
				    sysnc_count += e[2];
				    period2 = false;
				    count ++;
			    }
		    }

		    else if (period3 == 1) {
			    if ((depend3 == false) && (depend2 == true)) {
				    depend3 = true;
				    cycle_count = cycle_count + e[3];
				    sysnc_count = sysnc_count + e[3];
				    period3 = false;
				    //printf("run 3\n");
				    count ++;
				    if (sub_delay_time) {
					    sub_delay_time += e[3];
				    }
			    }
			    else if (depend3 == true) {
				    cycle_count += e[3];
				    sysnc_count += e[3];
				    period3 = false;
				    //printf("run 3 again\n");
				    count ++;
				    if (sub_delay_time) {
					    sub_delay_time += e[3];
				    }
			    }
			    else {
				    cycle_count += e[3];
				    sysnc_count += e[3];
				    period3 = false;
				    count ++;
			    }
		    }

		    else if (period4 == 1) {
			    if ((depend4 == false) && (depend3 == true)) {
				    cycle_count = cycle_count + e[4];
				    sysnc_count = sysnc_count + e[4];
				    period4 = false;
				    //printf("run 4\n");
				    if (worst_delay < cycle_count) {
					    worst_delay = cycle_count;
				    }
				    //printf("End-to-End Delay : %d\n", cycle_count);
				    depend1 = false;
				    depend2 = false;
				    depend3 = false;
				    cycle_count = 0;
				    if (sub_delay_time =! 0) {
					    depend1 = true;
					    cycle_count = sub_delay_time + e[4];
					    sub_delay_time = 0;
				    }
				    count ++;
			    }
			    else {
				    cycle_count += e[4];
				    sysnc_count += e[4];
				    period4 = false;
				    count ++;
			    }

		    }

		    else {
			    cycle_count ++;
			    sysnc_count ++;
			    count ++;
			    //printf("delay 1ms\n");
		    }
	    }
	    else if (sysnc_count != count) {
		    count ++;
	    }
    }
    printf("Simulation E2E Delay : %d ms\n", worst_delay);

    return 0;
}


static int Delay_R(int e[], int p[], int dag_type) {
    int sum = 0;
    int n = get_n(dag_type);
    for (int i = 1; i <= n; i++) {
        sum += p[i]*2;
    }
    printf("Period X 2 E2E Delay : %dms\n", sum);
    return sum;
}

int lcm(int a,int b) {
    int i,j;
    int temp; //공배수를 저장할 변수
    j = (a>b)?a:b; //둘중 큰수를 구함
    for(i=j;;i++) {
        if(i%a==0 && i%b==0){
            temp = i;
            break;
        }
    }
    return temp;
}