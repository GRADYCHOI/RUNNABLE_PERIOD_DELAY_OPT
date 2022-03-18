#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>
#include <stdbool.h>

/* support macros for finding maximum values */
#define MAX2(a, b)          ((a) > (b) ? (a) : (b))
#define MAX3(a, b, c)       (MAX2(MAX2((a), (b)), (c)))
#define MAX4(a, b, c, d)    (MAX2(MAX2(MAX2((a), (b)), (c)), (d)))
#define MAX5(a, b, c, d, e) (MAX2(MAX2(MAX2(MAX2((a), (b)), (c)), (d)), (e)))

/* max number of runnables */
#define N_MAX    10

/* default options */
#define TIMING   0      /* whether to show timing information */
#define UB       1.0    /* utilization bound */
#define ALPHA    0.01   /* alpha constant for control cost function */
#define BETA     0.01   /* beta constant for control cost function */
#define P_MIN    10.0   /* minimum period for exhaustive search */
#define P_MAX    1000.0 /* maximum period for exhaustive search */
#define P_STEP   10.0   /* step for exhaustive search */

struct options {
    int timing;
    double ub;
    double alpha;
    double beta;
    double p_min;
    double p_max;
    double p_step;
};

/* 
 * EXH: exhaustive method
 * OUR: our method
 */
enum cmd {EXH, OUR};

/* DAG type */
enum dag_type {A, B, C, D, E, F};

/* function prototypes */
static int parse_options(int argc, char *argv[], int *cmd, int *dag_type, double e[], struct options *o);
static int run_ours(int dag_type, double e[], struct options *o);
static int run_exhaustive(int dag_type, double e[], struct options *o);
static int get_n(int dag_type);
static double east(int dag_type, double e[]);
static double jconv(int dag_type, double p[], double e[]);
static double util(double p[], double e[], int n);
static void show_result(const char *method, double j, double u, double p[], int n, long secs, long usecs, int timing);
static void make_simulator(double j, double u, double p[], double e[], int n, long secs, long usecs); 
static double jmax(int n);
static void timediff(struct timeval *start, struct timeval *end, long *secs, long *usecs);
static void usage(const char *prog);

int main(int argc, char *argv[])
{
    double e[N_MAX + 1];
    int cmd, dag_type;
    int i, rc;
    struct options o;

    o.timing = 0;
    o.ub = UB;
    o.alpha = ALPHA;
    o.beta = BETA;
    o.p_min = P_MIN;
    o.p_max = P_MAX;
    o.p_step = P_STEP;

    rc = parse_options(argc, argv, &cmd, &dag_type, e, &o);
    if (rc < 0) {
        return EXIT_FAILURE;
    }
    if (cmd == EXH) {
        rc = run_exhaustive(dag_type, e, &o);
    }
    else if (cmd == OUR) {
        rc = run_ours(dag_type, e, &o);
    }
    else {
        fprintf(stderr, "Invalid command number: %d\n", cmd);
    }
    if (rc < 0) {
        return EXIT_FAILURE;
    }
    else {
        return EXIT_SUCCESS;
    }
    /* unreachable */
}

static int parse_options(int argc, char *argv[], int *cmd, int *dag_type, double e[], struct options *o)
{
    int i, n, opt;
	while ((opt = getopt(argc, argv, "tm:M:s:a:b:u:")) != -1) {
        switch (opt) {
            case 't':
                o->timing = 1;
                break;
            case 'm':
                o->p_min = atof(optarg);
                break;
            case 'M':
                o->p_max = atof(optarg);
                break;
            case 's':
                o->p_step = atof(optarg);
                break;
            case 'a':
                o->alpha = atof(optarg);
                break;
            case 'b':
                o->beta = atof(optarg);
                break;
            case 'u':
                o->ub = atof(optarg);
                break;
            default:
                usage(argv[0]);
                return -1;
        }
    }
    if (argc < optind + 2) {
        fprintf(stderr, "Failed to parse the command line option\n");
        usage(argv[0]);
        return -1;
    }
    if (strcmp(argv[optind], "exh") == 0 || strcmp(argv[optind], "EXH") == 0) {
        *cmd = EXH;
    }
    else if (strcmp(argv[optind], "our") == 0 || strcmp(argv[optind], "OUR") == 0) {
        *cmd = OUR;
    }
    else {
        fprintf(stderr, "Invalid command: %s\n", argv[optind]);
        usage(argv[0]);
        return -1;
    }
    optind++;
    if (strcmp(argv[optind], "a") == 0 || strcmp(argv[optind], "A") == 0) {
        *dag_type = A;
    }
    else if (strcmp(argv[optind], "b") == 0 || strcmp(argv[optind], "B") == 0) {
        *dag_type = B;
    }
    else if (strcmp(argv[optind], "c") == 0 || strcmp(argv[optind], "C") == 0) {
        *dag_type = C;
    }
    else if (strcmp(argv[optind], "d") == 0 || strcmp(argv[optind], "D") == 0) {
        *dag_type = D;
    }
    else {
        fprintf(stderr, "Invalid DAG type: %s\n", argv[optind]);
        usage(argv[0]);
        return -1;

    }
    optind++;
    n = get_n(*dag_type);
    if (n < 0) {
        fprintf(stderr, "Cannot identify n from the DAG type %s\n", argv[optind - 1]);
        usage(argv[0]);
        return -1;
    }
    if (n > argc - optind) {
        fprintf(stderr, "Requires %d execution times instead of %d\n", n, argc - optind);
        usage(argv[0]);
        return -1;
    }
    else if (n < argc - optind) {
        fprintf(stderr, "Requires %d execution times instead of %d\n", n, argc - optind);
        usage(argv[0]);
        return -1;
    }
    for (i = 1; i <= n; i++) {
        e[i] = atof(argv[optind++]);
    }
    return 0;
}

static int run_ours(int dag_type, double e[], struct options *o)
{
    struct timeval start, end;
    long secs, usecs;
    int i, n;
    double e_ast;
    double p[N_MAX + 1];
    double p_ast;

    double alpha = o->alpha;
    double beta = o->beta;

    gettimeofday(&start, NULL);
    n = get_n(dag_type);
    e_ast = east(dag_type, e);

    p[1] = e[1] + sqrt((n -2) * e[1] * e_ast) + sqrt((alpha + beta) * e[1] * e[n] / beta); 
    p_ast = p[1] * sqrt((n - 2) * e_ast / e[1]);
    for (i = 2; i <= n - 1; i++) {
        p[i] = p_ast * (e[i] / e_ast);
    }
    p[n] = p[1] * sqrt(beta * e[n] / ((alpha + beta) * e[1]));
    for (i = 1 ; i <= n ; i++) {
	    if ((p[i] / 1) > 0) {
		    p[i] = (int)p[i]/1 + 1;
	    }
    }
    gettimeofday(&end, NULL);
    timediff(&start, &end, &secs, &usecs);

    show_result("OURS", jconv(dag_type, p, e), util(p, e, n), p, n, secs, usecs, o->timing);
    make_simulator(jconv(dag_type, p, e), util(p, e, n), p, e, n, secs, usecs);
    
    return 0;
}

static int run_exhaustive(int dag_type, double e[], struct options *o)
{
    struct timeval start, end;
    long secs, usecs;
    int i, n;
    double p[N_MAX + 1];
    double opt_p[N_MAX + 1];
    double min_j, min_j_u;
    double u, opt_u;
    double j;

    double ub = o->ub;
    double alpha = o->alpha;
    double beta = o->beta;
    double p_min = o->p_min;
    double p_max = o->p_max;
    double p_step = o->p_step;

    gettimeofday(&start, NULL);
    n = get_n(dag_type);
    min_j = jmax(n);
    for (i = 1; i <= n; i++) {
        p[i] = p_min;
    }
    for (;;) {
        int end_flag = 0;
        /* loop body begins here */
        u = util(p, e, n);
        if (u > ub) {
            goto loop;
        }
        j = jconv(dag_type, p, e);
        if (j < min_j) {
            min_j = j;
            min_j_u = u;
            memcpy(opt_p, p, sizeof(p));
        }
        /* loop body ends here */
loop:
        for (i = n; i >= 0; i--) {
            if (i == 0) {
                end_flag = 1;
                break;
            }
            p[i] += p_step;
            if (p[i] > p_max) {
                p[i] = p_min;
            }
            else {
                break;
            }
        }
        if (end_flag) {
            break;
        }
    }
    gettimeofday(&end, NULL);
    timediff(&start, &end, &secs, &usecs);
    show_result("EXHAUSTIVE", min_j, min_j_u, opt_p, n, secs, usecs, o->timing); 

    return 0;
}

static int get_n(int dag_type)
{
    switch (dag_type) {
    case A:
        return 4;
    case B:
        return 5;
    case C:
        return 5;
    case D:
        return 6;
    default:
        return -1;
    }
}

static double east(int dag_type, double e[])
{
    switch (dag_type) {
    case A:
        return e[2] + e[3];
    case B:
        return MAX2(e[2] + e[3], e[2] + e[4]);
    case C:
        return MAX2(e[4], e[2] + e[3]);
    case D:
        return MAX3(e[2] + e[3], e[2] + e[5], e[4] + e[5]);
    default:
        return 0;
    }
}

static double jconv(int dag_type, double p[], double e[])
{
    switch (dag_type) {
    case A:
        return (2 * ALPHA * p[4] + 2 * BETA * (p[1] + p[2] + p[3] + p[4]));
    case B:
        return MAX2(2 * ALPHA * p[5] + 2 * BETA * (p[1] + p[2] + p[3] + p[5]),
                    2 * ALPHA * p[5] + 2 * BETA * (p[1] + p[2] + p[4] + p[5]));
    case C:
        return MAX2(2 * ALPHA * p[5] + 2 * BETA * (p[1] + p[4] + p[5]),
                    2 * ALPHA * p[5] + 2 * BETA * (p[1] + + p[2] + p[3] + p[5]));
    case D:
        return MAX3(2 * ALPHA * p[6] + 2 * BETA * (p[1] + p[2] + p[3] + p[6]),
                    2 * ALPHA * p[6] + 2 * BETA * (p[1] + p[2] + p[5] + p[6]),
                    2 * ALPHA * p[6] + 2 * BETA * (p[1] + p[4] + p[5] + p[6]));
    default:
        return 0;
    }
}

static double util(double p[], double e[], int n)
{
    int i;
    double u = 0.0;

    switch (n) {
    case 4:
        return (e[1]/p[1] + e[2]/p[2] + e[3]/p[3] + e[4]/p[4]);
    case 5:
        return (e[1]/p[1] + e[2]/p[2] + e[3]/p[3] + e[4]/p[4] + e[5]/p[5]);
    case 6:
        return (e[1]/p[1] + e[2]/p[2] + e[3]/p[3] + e[4]/p[4] + e[5]/p[5] + e[6]/p[6]);
    case 7:
        return (e[1]/p[1] + e[2]/p[2] + e[3]/p[3] + e[4]/p[4] + e[5]/p[5] + e[6]/p[6] + e[7]/p[7]);
    case 8:
        return (e[1]/p[1] + e[2]/p[2] + e[3]/p[3] + e[4]/p[4] + e[5]/p[5] + e[6]/p[6] + e[7]/p[7] + e[8]/p[8]);
    default:
        for (i = 1; i <= n; i++) {
            u += (e[i] / p[i]);
        }
        return u;
    }
    /* unreachable */
}

static void show_result(const char *method, double j, double u, double p[], int n, long secs, long usecs, int timing) 
{
    int i;

    printf("method : %-10s    Control Cost Function :  %-6.2f Util : %-6.2f \n", method, j, u * 100);
    printf("period : ");
    for (i = 1; i <= n; i++) {
        if (i != n) {
            printf("%-3.2f ", p[i]);
        }
        else {
            printf("%-3.2f\n", p[i]);
        }
    }
    if (timing) {
        printf("      detection Time : %ld.%06ld\n", secs, usecs);
    }
    else {
        printf("\n");
    }
    return;
}
// make period simulator 
static void make_simulator(double j, double u, double p[], double e[], int n, long secs, long usecs) 
{
    int i;
    double al_period;
    double al_execution;
    int count = 0;
    int cycle = 0;
    int k = 0;
    int cycle_count = 0;
    bool period1 = true;
    bool period2 = true;
    bool period3 = true;
    bool period4 = true;
    int sysnc_count = 0;

    bool depend1; // priority
    bool depend2;
    bool depend3;
    bool depend4;

    for (i = 1; i <= n+1; i++) {
	al_execution += e[i];
	//runnable -> period
	//runnable -> exevution time
	//runnable -> dag dependency

    }
    printf("Total Execution Time : %-3.2f \n", al_execution);
    printf("        Total period : %-3.2f \n", al_period);
    
    while (count < 100){
	    printf("now time : %d ms       sysnc_time : %d    \n", count, sysnc_count);
	    if (count % (int)p[1] == 0) { 
		    period1 = true;
	    }
	    if (count % (int)p[2] == 0) { 
		    period2 = true;
	    }
	    if (count % (int)p[3] == 0) {
		    period3 = true; 
	    }
	    if (count % (int)p[4] == 0) {
		    period4 = true; 
	    }
	    printf(" %d %d %d %d\n", period1, period2, period3, period4);

	    if (sysnc_count == count) { 
		    printf("sysnc\n");
		    if (period1 == 1) {
			    cycle_count = cycle_count + e[1];
			    sysnc_count = sysnc_count + e[1];
			    period1 = false;
			    printf("run 1\n");
			    count ++;
			    continue;
		    }
		    else if (period2 == 1) {
			    cycle_count = cycle_count + e[2];
			    sysnc_count = sysnc_count + e[2];
			    period2 = false;
			    printf("run 2\n");
			    count ++;
			    continue;
		    }
		    else if (period3 == 1) {
			    cycle_count = cycle_count + e[3];
			    sysnc_count = sysnc_count + e[3];
			    period3 = false;
			    printf("run 3\n");
			    count ++;
			    continue;
		    }
		    else if (period4 == 1) {
			    cycle_count = cycle_count + e[4];
			    sysnc_count = sysnc_count + e[4];
			    period4 = false;
			    printf("run 4\n");
			    printf("End-to-End Delay : %d\n", cycle_count);
			    cycle_count = 0;
			    count ++;
			    continue;
		    }

		    if ((period1 == 0) && (period2 == 0)  && (period3 == 0) && (period4 == 0)) {
			    cycle_count ++;
			    sysnc_count ++;
			    printf("delay 1ms\n");
		    }
	    }
	    count ++;
    }

//
//    while (count < 100){
//	    for(int j = 1; j<= 4; j++){
//		    if (j % 4 == 1) {
//			    count += e[1];
//			    cycle_count += e[1];
////			    else {
////				    count, cycle_count =+ (p[1]*cycle - count);				    
////				    
////				    printf("waiting %f ms\n", (p[1]*cycle - count));
////				    count += e[1];
////				    cycle_count += e[1];
////			    }
//
//			    printf("run 1\n");
//		    }
//		    else if (j % 4 == 2) {
//			    count += e[2];
//			    cycle_count += e[2];
////			    else {
////				    count, cycle_count =+ (p[2]*cycle - count);
////				    printf("waiting %f ms\n", (p[2]*cycle - count));
////				    count += e[2];
////				    cycle_count += e[2];
////			    }
//				    
//			    printf("run 2\n");
//		    }
//		    else if (j % 4 == 3) {
//			    count += e[3];
//			    cycle_count += e[3];
////			    else {
////				    count, cycle =+ (p[3]*cycle - count);
////				    printf("waiting %f ms\n", (p[3]*cycle - count));
////				    count += e[3];
////				    cycle_count += e[3];
////			    }
//			    printf("run 3\n");
//		    }
//		    else if (j % 4 == 0) {
//			    count += e[4];
//			    cycle_count += e[4];
////			    else {
////				    count, cycle_count =+ (p[4]*cycle - count);
////				    printf("waiting %f ms\n", (p[4]*cycle - count));
////				    count += e[4];
////				    cycle_count += e[4];
////			    }
//			    printf("run 4\n");
//			    printf("End-to-End Delay : %d \n", cycle_count);
//			    cycle_count = 0;
//			    cycle++;
//		    }
//	    }
//	    printf("Now Time : %d \n", count);
//	    
//    }
//
    
    return;
}
static double jmax(int n)
{
    return (2 * ALPHA * P_MAX + 2 * BETA * n * P_MAX);
}

static void timediff(struct timeval *start, struct timeval *end, long *secs, long *usecs)
{
    *secs = (end->tv_sec - start->tv_sec); //avoid overflow by subtracting first
    *usecs = (end->tv_usec - start->tv_usec);
    if (*usecs < 0) {
        *secs -= 1;
        *usecs = 1000000 + *usecs;
    }
    return; 
}

static void usage(const char *prog)
{
    fprintf(stderr, "usage: %s [OPTIONS] algorithm dag e1 e2 ... en\n", prog);
    fprintf(stderr, "       -t: show timing information\n");
    fprintf(stderr, "       -m <minimum period>\n");
    fprintf(stderr, "       -M <maximum period>\n");
    fprintf(stderr, "       -s <period step>\n");
    fprintf(stderr, "       -a <alpha>\n");
    fprintf(stderr, "       -b <beta>\n");
    fprintf(stderr, "       -u <utilization bound>\n");
    fprintf(stderr, "       algorithm: [exh|our]\n");
    fprintf(stderr, "       dag: DAG type [a|b|c|d]\n");
    fprintf(stderr, "       e1 e2 ... en: execution times for each runnable\n");
    return;
}
