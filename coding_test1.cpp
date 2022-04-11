#include <iostream>

using namespace std;

int main() {
    int age, year;
    char cnum[20], sex;
    cin >> cnum;
    
    if (cnum[7] == '1' || cnum[7] == '2') {
        year = 1900 + (((cnum[0]-48)*10) + (cnum[1]-48));
    }
    else { year = 2000 + ((cnum[0]-48)*10 + (cnum[1]-48)); }
    age = 2022-year+1;

    if (cnum[7] == '1' || cnum[7] == '3') {
        sex = 'M';
    }
    else { sex = 'W'; }
    cout << age << "  " << sex << endl;
    return 0;
}