//
// Created by kowal on 12.02.18.
//

#include <cstdio>
#include <set>
#include <tuple>
#include <algorithm>

using namespace std;

int main() {
    unsigned long n;
    scanf("%lu", &n);

    vector<tuple<int, int, int>> lines;
    lines.reserve(n);
    for(int i=0; i<n; i++) {
        int x, y1, y2;
        scanf("%d %d %d", &x, &y1, &y2);
        lines.emplace_back(x, y1, y2);
    }

    sort(lines.begin(), lines.end());

    unsigned long long ans = 0;
    for(auto line = lines.begin(); line != lines.end(); line++) {
        auto first_not_crossed_line = lower_bound(line, lines.end(), tuple<int, int, int>(
                get<0>(*line),
                get<2>(*line),
                get<2>(*line)
        ));

        ans = ans + distance(line, first_not_crossed_line) - 1;
    }

    printf("%llu\n", ans);
    return 0;
}
