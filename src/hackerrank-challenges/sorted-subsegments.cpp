//
// Created by kowal on 16.02.18.
//

#include <vector>
#include <algorithm>
#include <cstdio> // using scanf/printf for speed

using namespace std;


unsigned int q, k;
unsigned long n;
int range_min, range_max;
vector<int> v;


int main() {
    scanf("%lu %d %d", &n, &q, &k);
    v.reserve(n);
    for(unsigned int i=0; i<n; i++) {
        int tmp;
        scanf("%d", &tmp);
        v.emplace_back(tmp);
    }
    for(unsigned int i=0; i<q; i++) {
        int l, r;
        scanf("%d %d", &l, &r);
        sort(v.begin()+l, v.begin()+r+1);
    }

    printf("%d\n", v[k]);
    return 0;
}