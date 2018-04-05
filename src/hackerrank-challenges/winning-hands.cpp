//
// Created by kowal on 16.02.18.
//

#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

#define MMAX 1000000
#define NMAX 30

int n, m, x;
int partial_ans[NMAX+1][MMAX]; // [i][k] := # of i-card hands that = k (mod m)
int times_used[NMAX+1][MMAX][NMAX]; // [i][k][l] := # of times card #l is used in partial_ans[i][k]
int cards[NMAX];
int ans = 0;

int main() {
    for(int i=0; i<=NMAX; i++) {
        for(int j=0; j<MMAX; j++) {
            partial_ans[i][j] = 0;
            for(int k=0; k<NMAX; k++) {
                times_used[i][j][k] = 0;
            }
        }
    }

    scanf("%d %d %d", &n, &m, &x);
    for(int i=0; i<n; i++) {
        int val;
        scanf("%d", &val);
        partial_ans[1][val%m]++;
        times_used[1][val%m][val]++;
        cards[i] = val;
    }

    for(int hs=2; hs<=n; hs++) {
        for(int val = 0; val < m; val++) {
            for(int a = 0; a < m; a++) {
                for(int cid = 0; cid < n; cid++) {
                    // for each card, each smaller set if a card was not used in the smaller set, create a set with that card
                    if(!times_used[hs-1][a][cid] && (cards[cid]*a)%m == val) {
                        partial_ans[hs][val] += (partial_ans[hs-1][a]);
                    }
                }
                for(int b = 0; b < m; b++) {

                    if((a*b)%m == val && times_used[hs-1][a] ^ times_used[1][b]) {
                        partial_ans[hs][val] += (partial_ans[hs-1][a] * partial_ans[1][b]);
                        times_used[hs][val] = times_used[hs-1][a] | times_used[1][b];
                    }
                }
            }
        }
    }
    for(int hs = 1; hs<=n; hs++) {
        ans += partial_ans[hs][x];
    }
    printf("%d\n", ans);
    return 0;
}