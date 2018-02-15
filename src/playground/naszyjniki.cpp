//
// Created by kowal on 15.02.18.
//

#include <cstdio>
#include <vector>
#include <algorithm>
#include <climits>
#include <set>

using namespace std;


vector<pair<int, int>> s_pos; // pair<s, position of s>


int main() {
    int n, l;
    scanf("%d %d", &n, &l);

    s_pos.reserve((size_t)(n*l));
    for(int i=0; i<n; i++) {
        for(int pos=0; pos<l; pos++) {
            int s;
            scanf("%d", &s);
            s_pos.emplace_back(s, pos);
        }
    }

    sort(s_pos.begin(), s_pos.end());
    auto new_end = unique(s_pos.begin(), s_pos.end());

    int max_dist = 0;
    auto begin_it = s_pos.begin();
    while(begin_it != new_end) {
        auto end_it = begin_it+1;
        int prev_pos = (*begin_it).second;
        int dist = 0;
        while(end_it != new_end && (*end_it).first == (*begin_it).first) {
            int current_pos = (*end_it).second;
            if(current_pos - prev_pos == 1) {
                dist++;
            } else {
                max_dist = max(dist, max_dist);
                dist = 0;
            }
            prev_pos = current_pos;
            end_it++;
        }
        max_dist = max(dist, max_dist);
        begin_it = end_it;
    }
    printf("%i\n", max_dist+1);
    return 0;
}
