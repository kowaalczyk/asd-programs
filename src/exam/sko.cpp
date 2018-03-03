//
// Created by kowal on 17.02.18.
//

#include <vector>
#include <cstdio>
#include <algorithm>
#include <unordered_set>
using namespace std;

int n, m;

vector<tuple<int, int, int>> E; // weight, node1, node2
vector<bool> used;
vector<int> partial_ans;
vector<bool> partial_possible;

pair<int, bool> min_following_edges(int i, int used_count) {
    while(i<m && (used[get<1>(E[i])] || used[get<2>(E[i])])) {
        i++;
    }
    if(i>=m) {
        return pair<int, bool>(0, used_count>=n);
    }
    // lightest usable edge
    if(partial_ans[i] != 0) {
        return pair<int, bool>(partial_ans[i], partial_possible[i]);
    }

    auto edge = E[i];
    auto min_without_edge_ret = min_following_edges(i + 1, used_count);

    used[get<1>(edge)] = true;
    used[get<2>(edge)] = true;
    auto min_with_edge_ret = min_following_edges(i + 1, used_count + 2);
    used[get<1>(edge)] = false;
    used[get<2>(edge)] = false;

    if(min_with_edge_ret.second && min_without_edge_ret.second) {
        partial_ans[i] = min(min_with_edge_ret.first + get<0>(edge), min_without_edge_ret.first);
        partial_possible[i] = true;
        return pair<int, bool>(min(min_with_edge_ret.first + get<0>(edge), min_without_edge_ret.first), true);
    } else if(min_with_edge_ret.second) {
        partial_ans[i] = min_with_edge_ret.first + get<0>(edge);
        partial_possible[i] = true;
        return pair<int, bool>(min_with_edge_ret.first + get<0>(edge), true);
    } else if(min_without_edge_ret.second) {
        partial_ans[i] = min_without_edge_ret.first;
        partial_possible[i] = true;
        return pair<int, bool>(min_without_edge_ret.first, true);
    } else {
        partial_ans[i] = -1;
        partial_possible[i] = false;
        return pair<int, bool>(0, false);
    }
}


int main() {
    scanf("%d %d", &n, &m);
    E.reserve(static_cast<unsigned long>(m));
    used.reserve(static_cast<unsigned long>(m));
    partial_ans.reserve(static_cast<unsigned long>(m));
    partial_possible.reserve(static_cast<unsigned long>(m));

    for(int i=0; i<m; i++) {
        int u, v, c;
        scanf("%d %d %d", &u, &v, &c);
        E.emplace_back(c, min(u, v), max(u, v));
        used[u] = false;
        used[v] = false;
        partial_ans[m] = 0;
    }

    sort(E.begin(), E.end());
    auto ans_ret = min_following_edges(0, 0);
    if(ans_ret.second) {
        printf("%d\n", ans_ret.first);
    } else {
        printf("-1\n");
    }

    return 0;
}
