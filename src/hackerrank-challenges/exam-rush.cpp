//
// Created by kowal on 16.02.18.
//

#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

unsigned int n, h;
vector<unsigned int> v;

int main() {
    scanf("%d %d", &n, &h);
    v.reserve(n);
    for (unsigned int i = 0; i < n; ++i) {
        unsigned int tmp;
        scanf("%d", &tmp);
        v.emplace_back(tmp);
    }

    sort(v.begin(), v.end());
    unsigned int ans = 0;
    unsigned int used_h = 0;
    while(used_h + v[ans] < h) {
        used_h += v[ans];
        ans++;
    }
    printf("%d\n", ans);
    return 0;
}
