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

/* structs */
struct model {
    int dag;
    int priority;
    int ex_time;
    int period;
};

struct runnable {
    char namne[20];
    int property;
    int execution_type;
    int execution_time;
};

struct task {
    int dag;
    int priority;
    int ex_time;
    int period;
    struct runnable runnable;
};

enum dag_type {A, B, C, D};
//enum property {DRP, WP, DSP};
//enum execution_type {TE, DE};

static int parse_options(int argc, char *argv[], int *dag_type, int e[]);
static int run_mapping(int dag_type, int e[], long secs, long usecs);
static int get_n(int dag_type);

int main(int argc, char *argv[]) {

    int e[argc-2];
    int dag_type;
    struct model m;
    struct task t;
    int map;
    long secs, usecs;

    map = parse_options(argc, argv, &dag_type, e);

    if (map == 0) {
        map = run_mapping(dag_type, e, secs, usecs);
    }
    else if (map <0) {
        return EXIT_FAILURE;
    }
}

static int parse_options(int argc, char *argv[], int *dag_type, int e[]) {
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
        printf("%d\n", e[i]);
    }
    return 0;
}



static int get_n(int dag_type) {
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

static int run_mapping(int dag_type, int e[], long secs, long usecs) {
    printf("runnable mapping in tasks\n");

    int task1 = 1;
    int task5 = 5;
    int task10 = 10;
    int task20 = 20;
    int i;
    int n = get_n(dag_type);
    printf("%d\n", n);
    int p[n];
    for (i = 1 ; i <= n ; i++) {
        if (e[i] <= task1) {
            p[i] = task1;
        }
        else if ((e[i] > task1) && (e[i] <= task5)) {
            p[i] = task5;
        }
        else if ((e[i] > task5) && (e[i] <= task10)) {
            p[i] = task10;
        }
        else if ((e[i] > task10) && (e[i] <= task20)) {
            p[i] = task20;
        }
        printf("runnable %d's period : %d\n", i, p[i]);

    }





}