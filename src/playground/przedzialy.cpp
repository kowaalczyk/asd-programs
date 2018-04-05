//
// Created by kowal on 15.02.18.
//

#include <vector>
#include <cstdio>
#include <algorithm>
#include <climits>

using namespace std;

vector<pair<int, int>> v;

int main() {
    size_t n;
    scanf("%zu", &n);

    v.reserve(n);
    for(size_t i=0; i<n; i++) {
        int a, b;
        scanf("%d %d", &a, &b);
        v.emplace_back(a, b);
    }

    sort(v.begin(), v.end());

    int ans = INT_MAX;
    for(auto it = v.begin(); it != v.end(); it++) {
        auto nxt = it+1;
        if(nxt == v.end())
            break;
        ans = max(0, min(ans, (*nxt).first - (*it).second));
    }
    printf("%d\n", ans);
    return 0;
}