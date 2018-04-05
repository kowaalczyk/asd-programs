//
// Created by kowal on 15.02.18.
//

#include <vector>
#include <cstdio>

using namespace std;

int tab[1000000];
// 0 - raw series, 1, 2, 3 - # of following 1s/2s/3s, 4 - # of following <2,3> - series

int main() {
    int n;
    scanf("%d", &n);
    for(int i=0; i<n; i++) {
        scanf("%d", &tab[i]);
    }
    long long num[8] = {0, 0, 0, 0, 0, 0, 0, 0}; // placeholder, 1:1s, 2:2s, 3:3s, 4:<1,2>s, 5:<1,3>s, 6:<2,3>s, 7:<1,2,3>s
    for(int i=n-1; i>=0; i--) {
        switch (tab[i]) {
            case 1:
                num[1]++;
                num[4] += num[2]; // <1,2>
                num[5] += num[3]; // <1,3>
                num[7] += num[6]; // <1,2,3>
                break;
            case 2:
                num[2]++;
                num[6] += num[3]; // <2,3>
                break;
            case 3:
                num[3]++;
                break;
            default:
                // not possible
                break;
        }
    }
    long long sum = 0;
    for(int i=0; i<8; i++) {
        sum += num[i];
    }
    printf("%lli\n", sum);
    return 0;
}
