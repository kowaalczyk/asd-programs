//
// Created by kowal on 17.02.18.
//

#include <cstdio>
#include <climits>
#include <array>
#include <vector>
#include <tuple>
#include <algorithm>
#include <cassert>

using namespace std;

int n, m;

int edges[100001][3]; // [i][1] = weight of edge i--(i-1), [i][2] = weight of edge i--(i-2)
vector<tuple<int, int>> min_with; // [i]<j> = minimal matching [0..i] with connection i--(i-j)
vector<tuple<int, int>> min_without; // [i]<j> = minimal matching [0..i]\[i-j], after i is calculated

int main() {
    scanf("%d %d", &n, &m);
    min_with.reserve(static_cast<unsigned long>(n+1));
    min_without.reserve(static_cast<unsigned long>(n+1));

    for(int i=0; i<=n; i++) {
        min_with.emplace_back(INT_MAX, INT_MAX);
        min_without.emplace_back(INT_MAX, INT_MAX);
    }

    for(int i=0; i<m; i++) {
        int u, v, c;
        scanf("%d %d %d", &u, &v, &c);
        int bigger = max(u, v);
        int smaller = min(u, v);
        if(smaller+1 == bigger) {
            edges[bigger][1] = c;
        } else {
            assert(smaller+2 == bigger);
            edges[bigger][2] = c;
        }
    }

    if(n%2 == 1) {
        printf("-1\n");
        exit(0);
    }

    min_with[2] = tuple<int, int>(edges[2][1], INT_MAX);
    min_without[3] = tuple<int, int>(edges[2][1], edges[3][2]);

    for(int i=4; i<=n; i++) {
        // match with (i-1)
        int min_connect_1 = min(get<0>(min_without[i-1]) + edges[i][1], INT_MAX);
        // match with (i-2)
        int min_connect_2 = min(get<1>(min_without[i-1]) + edges[i][2], INT_MAX);
        // MAX_INT = no match possible
        min_with[i] = tuple<int, int>(min_connect_1, min_connect_2);

        // min matching of [0..i-1] with loaded i
        int min_without_1 = min(get<0>(min_with[i-1]), get<1>(min_with[i-1]));
        // min matching of [0..i]\[i-1] with loaded i
        int min_without_2 = min(get<0>(min_without[i-2]) + edges[i][2], INT_MAX);

        min_without[i] = tuple<int, int>(min_without_1, min_without_2);
        // MAX_INT = no match possible
    }

    int min_ans = min(get<0>(min_with[n]), get<1>(min_with[n]));
    int ans = min_ans == INT_MAX ? -1 : min_ans;
    printf("%d\n", ans);

    return 0;
}
