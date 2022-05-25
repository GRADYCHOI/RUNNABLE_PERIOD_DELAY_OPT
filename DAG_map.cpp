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
    
	return 0;
}

int critical_path(int num, vector<vector<int>> run, int e[]) {
    vector<int> delay;
    
    int critical_delay = 0, critical_count = 1, next_dag = 1;
    int b = dfs(critical_delay, critical_count, num, run, e);
    delay.push_back(b);

    for (auto d : delay) {
        cout << d << " ";
    }
    cout << endl;

    return 0;
}

int dfs(int delay_time, int count, int num, vector<vector<int>> run, int e[]) {
    //count++;
    //if (count > num) {
    //    printf("SDFSDF\n\n");
        //run.clear
    //    return delay_time;// count 말고 프랙티스하게 경로이동을 잡는게 좋을듯, 예외처리도
    //끝까지 말고 경로 끝나면으로.
    //각 패스의 딜레이를 배열에 넣고 가장 오래걸리는거 비교, 그러려면 해당 딜레이의 dag도 알아야함
    //}
    if (count < num) { // i값을 놓는 것도 필요함 ex) next_dag = i 이런식으로
        printf("current runnable : %d\n", count);
        delay_time += e[count];
        cout << "delay = " << delay_time <<endl;
        for (int i = 0; i < num; i++) {
            if (run[count][i] == 0) continue;

            else {
                cout << count << "'s dag : " << run[count][i] << endl;
                dfs(delay_time, run[count][i], num, run, e); // 여기가 문제인듯
            }
        }
        printf("\n");
    }
    else {
        printf("last runnable : %d\n", count);
        delay_time += e[count];
        cout << "delay = " << delay_time <<endl;
        return delay_time;
    }
    return delay_time;
}