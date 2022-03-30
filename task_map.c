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

struct task {
    int dag;
    int priority;
    int ex_time;
    int period;
};

enum dag_type {A, B, C, D};

static int parse_options(int argc, char *argv[], int *dag_type, int e[], struct model *m);
static int run_mapping(int *dag_type, int e[], struct model *m, struct task *t);
static int get_n(int dag_type);

int main(int argc, char *argv[]) {

    int e[N_MAX];
    int dag_type;
    struct model m;
    struct task t;
    int map;

    map = parse_options(argc, argv, &dag_type, e, &m);

    if (map == 0) {
        //map = run_mapping(dag_type, e, &m, &task);
    }
    else if (map <0) {
        return EXIT_FAILURE;
    }
}

static int parse_options(int argc, char *argv[], int *dag_type, int e[], struct model *m) {
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
    for (i = 1 ; i <= n ; i++) {
        e[i] = atof(argv[optind++]);
        printf("e%d : %d\n", i, e[i]);
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