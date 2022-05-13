#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>
#include <stdbool.h>

using namespace std;

/* default options */
#define N_MAX 20


enum dag_type {A, B, C, D};
//enum property {DRP, WP, DSP};
//enum execution_type {TE, DE};

static int parse_options(int argc, char *argv[], int *dag_type, int e[]);
static int run_mapping(int dag_type, int e[], int p[], long secs, long usecs);
static int get_n(int dag_type);
static int Delay_C(int e[], int p[], int dag_type);
static int Delay_2(int e[], int p[], int dag_type);
static int Delay_R(int e[], int p[], int dag_type);
int lcm(int a,int b);
static int Response_c(int i, int e[], int p[], int n, int priority[], int value);

int main(int argc, char *argv[]) {
    int number;
    int count = 1;
    bool DAG = true;
    cout << "Runnable count : ";
    cin >> number;

    vector<vector<int>> runnable(number+1, vector<int>{ });

    int e[number] = { };
    while(count <= number) {
        int k;
        int exe;
        cout << "Please write " << count <<" Runnable's Execution Time :";
        cin >> exe;
        e[count] = exe;
        cout << "Please write " << count <<" Runnable's DAG Number :";
        cin >> k;
        for (int i = 0; i < k; i++) {
            int d;
            cin >> d;
            runnable[count].insert(runnable[count].begin()+i, d);
        }
        cout << count << " Runnable's DAG write finish" << endl;
        count++;

    }
	for (int i = 1; i <= number; i++) {
        cout << i << " Runnable's Execution Time : " << e[i] << endl;
        cout << i << " Runnable DAG : ";
        for (auto v : runnable[i]) {
            cout << v << " ";
        }
        
        cout << endl;
	}

	return 0;
}