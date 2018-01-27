//
// Created by kowal on 27.01.18.
//

#include <cstdio>
#include <set>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <cstring>

using namespace std;

typedef uint64_t val_t;

vector<pair<val_t, size_t>> pearls; // value, pos

int main() {
    size_t n;
    scanf("%zu", &n);

    pearls.resize(n+1);
    for (size_t i=1; i<=n; i++) { // indexing positions from 1
        val_t tmp;
        scanf("%lu", &tmp);

        pearls.emplace_back(tmp, i);
    }

    sort(pearls.begin(), pearls.end(), std::greater<pair<val_t, size_t >>());

    val_t ans = 0;
    bool taken_three = false;
    unordered_set<size_t> used_positions;
    for (auto p : pearls) {
        if (used_positions.count(p.second-1)) {
            // already taken an element at the previous position
            if (used_positions.count(p.second-2)) {
                // 3rd element in a row
                if (!taken_three && used_positions.count(p.second-3) && !used_positions.count(p.second+1)) {
                    ans += p.first;
                    used_positions.emplace(p.second);
                    taken_three = true;
                }

            } else if (used_positions.count(p.second+1)) {
                // one element before and at least one after

                if (used_positions.count(p.second+2)) {
                    // unable to insert - would be 4 elements in a row
                } else {
                    // exactly one before and exactly one after
                    if (!taken_three) {
                        ans += p.first;
                        used_positions.emplace(p.second);
                        taken_three = true;
                    }
                }
            } else {
                // exactly one before, no after
                ans += p.first;
                used_positions.emplace(p.second);
            }
        } else if (used_positions.count(p.second+1)) {
            // none before, one or more after
            if (used_positions.count(p.second+2)) {
                // none before, two or more after
                if (!taken_three && !used_positions.count(p.second+3)) {
                    ans += p.first;
                    used_positions.emplace(p.second);
                    taken_three = true;
                }
            } else {
                // no before, exactly one after
                ans += p.first;
                used_positions.emplace(p.second);
            }
        } else {
            // standalone element
            ans += p.first;
            used_positions.emplace(p.second);
        }
    }

    printf("%lu\n", ans);
    return 0;
}