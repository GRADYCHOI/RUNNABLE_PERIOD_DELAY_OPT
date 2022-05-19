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

int critical_path(int e[], vector<vector<int>> run, int num);
int dfs(int delay_time, int count, int num, vector<vector<int>> run, int e[]);

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
    cout << "Find Critical Path" << endl;
    critical_path(e, runnable, number);

	return 0;
}

int critical_path(int e[], vector<vector<int>> run, int num) {

    int count = 1; 
    int delay_time = 0;
    cout << e[count];
    cout << e[run[count][count-1]];  
    dfs(delay_time, count, num, run, e);
    
    return delay_time;
}

int dfs(int delay_time, int count, int num, vector<vector<int>> run, int e[]) {
    if (count > num) {
        printf("SDFSDF");
        return delay_time;// count 말고 프랙티스하게 경로이동을 잡는게 좋을듯, 예외처리도
    //끝까지 말고 경로 끝나면으로.
    //각 패스의 딜레이를 배열에 넣고 가장 오래걸리는거 비교, 그러려면 해당 딜레이의 dag도 알아야함
    }
    else {
        printf("DDD");
        delay_time += e[count];
        cout << delay_time;
        for (int i = 0; i < num; i++) {
            cout << run[count][i];
            count++;
            dfs(delay_time, count, num, run, e);
        }
    }
    return 0;
}
