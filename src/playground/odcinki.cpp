//
// Created by kowal on 12.02.18.
//

#include <cstdio>
#include <set>
#include <tuple>
#include <algorithm>

using namespace std;

int main() {
    int n;
    scanf("%d", &n);

    set<tuple<int, int, int>, less<tuple<int, int, int>>> lines;
    for(int i=0; i<n; i++) {
        int x, y1, y2;
        scanf("%d %d %d", &x, &y1, &y2);
        lines.emplace(x, y1, y2);
    }

    long ans = 0;
    for(auto line = lines.begin(); line != lines.end(); line++) {
        auto first_not_cressed_line = lines.lower_bound(tuple<int, int, int>(
                get<0>(*line),
                get<2>(*line),
                get<2>(*line)
        ));

        ans = ans + distance(line, first_not_cressed_line) - 1;
    }

    printf("%li\n", ans);
    return 0;
}
