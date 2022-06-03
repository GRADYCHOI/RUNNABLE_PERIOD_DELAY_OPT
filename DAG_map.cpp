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

int dfs(int delay_time, int count, int num, vector<vector<int>> run, int e[]);
int critical_path(int num, vector<vector<int>> run, int e[]);
// 전역변수
int gb_count = 0; // critical path 벡터에 추가하기위한 파라미터
vector<int> delay(10, 0); // 각 path의 실행 delay 
vector<int> path(1); // 
vector<vector<int>> critical_path_dag(10, vector<int>(5,0));

int main(int argc, char *argv[]) {
    int number;
    int count = 1;
    bool DAG = true;
    cout << "Runnable count : ";
    cin >> number;

    vector<vector<int>> runnable(number+1, vector<int> (number, 0));

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
            //runnable[count].insert(runnable[count].begin()+i, d);  // insert 말고 바꾸는거로 하는게 좋을듯.
            runnable[count][i] = d;//이거로 가자
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
    critical_path(number, runnable, e);
    int critical_delay = delay[0];
    int critical_dag = 0;

    for (int i = 0; i < 10; i++) {
        if (delay[i] > critical_delay) { 
            critical_delay = delay[i];
            critical_dag = i;
        }        
    }
    printf("critical delay = %dms, critical path num : %d\ncritical path : ", critical_delay, critical_dag);
    for (int i = 1; i <= 5; i++) {
        if (critical_path_dag[critical_dag][i] != 0) printf("%d -> ", critical_path_dag[critical_dag][i]);
    }
    printf("\n");
    
	return 0;
}

int critical_path(int num, vector<vector<int>> run, int e[]) { 
    int critical_delay = 0, critical_count = 1, next_dag = 1;
    int b = dfs(critical_delay, critical_count, num, run, e);
    return 0;
}

int dfs(int delay_time, int count, int num, vector<vector<int>> run, int e[]) {
    if (count < num) {
        printf("current runnable : %d\n", count);
        path.push_back(count);
        delay_time += e[count];
        cout << "delay = " << delay_time <<endl;
        for (int i = 0; i < num; i++) {
            if (run[count][i] == 0) continue;

            else {
                cout << count << "'s dag : " << run[count][i] << endl;
                cout << delay_time << endl;
                delay_time = dfs(delay_time, run[count][i], num, run, e);  
            }
        }
        printf("\n");
    }
    else {
        printf("last runnable : %d\n", count);
        path.push_back(count);
        delay_time += e[count];
        copy(path.begin(), path.end(), critical_path_dag[gb_count].begin());
        cout << "delay = " << delay_time <<endl;
        delay[gb_count] = delay_time;
        gb_count ++;
    }
    path.pop_back();
    delay_time -= e[count];
    return delay_time;
}