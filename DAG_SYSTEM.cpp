#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>
#include <stdbool.h>
#include <iostream>
#include <vector>

using namespace std;

/* default options */
#define N_MAX 20


enum dag_type {A, B, C, D};
//enum property {DRP, WP, DSP};
//enum execution_type {TE, DE};

static int DAG_parse(int dag_type, int e[], vector<vector<int> run(number, vector<int> {}));
static int parse_options(int argc, char *argv[], int *dag_type, int e[]);
static int run_mapping(int dag_type, int e[], int p[], long secs, long usecs);
static int get_n(int dag_type);
static int Delay_C(int e[], int p[], int dag_type);
static int Delay_2(int e[], int p[], int dag_type);
static int Delay_R(int e[], int p[], int dag_type);
int lcm(int a,int b);
static int Response_c(int i, int e[], int p[], int n, int priority[], int value);


int main(int argc, char *argv[]) {

    int e[argc-2];
    int p[argc-2];
    int dag_type;
    int map;
    int DAG;
    int delay_c = 0, delay_2 = 0, delay_r = 0;
    long secs, usecs;
    int n = get_n(dag_type)
    vector<vector<int>> runnable(n, vector<int> {});


    map = parse_options(argc, argv, &dag_type, e);
    DAG = DAG_parse(dag_type, e, runnable);

    if (map == 0) {
        map = run_mapping(dag_type, e, p, secs, usecs);
    }
    if (delay_c == 0) {
        delay_2 = Delay_2(e, p, dag_type);  
        delay_r = Delay_R(e, p, dag_type);
        delay_c = Delay_C(e, p, dag_type);
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

static int DAG_parse(int dag_type, int e[], vector<vector<int> run(number, vector<int> {})) {




}