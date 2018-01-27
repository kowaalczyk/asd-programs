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

vector<tuple<val_t, size_t, size_t>> pearl_sets; // value, start pos, end pos

val_t best_possible(vector<tuple<val_t, size_t, size_t>>::const_iterator begin,
                    vector<tuple<val_t, size_t, size_t>>::const_iterator end,
                    bool taken_three,
                    vector<bool> used)
{
    val_t ans = 0;

    for(auto ps = begin; ps != end; ps++) {
        int insertable = true;
        size_t int_len = get<2>(*ps) - get<1>(*ps) + 1;

        for(val_t i = get<1>(*ps)-1; i <= get<2>(*ps)+1; i++) {
            if(used[i]) {
                insertable = false;
                break;
            }
        }
        if(insertable && int_len == 3 && taken_three) {
            insertable = false;
        }
        if(insertable) {
            if(int_len > 1) {
                // calculate max without inserted element
                val_t max_not_inserted = best_possible(ps+1, end, taken_three, used);

                // calculate max with inserted element
                if(int_len == 3) {
                    taken_three = true;
                }
                for(val_t i = get<1>(*ps); i <= get<2>(*ps); i++) {
                    used[i] = true;
                }

                val_t max_inserted = best_possible(ps+1, end, taken_three, move(used));

                // make best possible decision and return recursive result
                if(max_inserted + get<0>(*ps) >= max_not_inserted) {
                    return ans + max_inserted + get<0>(*ps);
                } else {
                    return ans + max_not_inserted;
                }
            } else {
                ans += get<0>(*ps);
                for(val_t i = get<1>(*ps); i <= get<2>(*ps); i++) {
                    used[i] = true;
                }
            }
        }
    }
    return ans;
}

int main() {
    size_t n;
    scanf("%zu", &n);

    pearl_sets.resize(3*n);
    val_t prev_val = 0;
    val_t prev_val_2 = 0;
    for (size_t i=1; i<=n; i++) { // positions indexed from 1 to n
        val_t tmp;
        scanf("%lu", &tmp);

        pearl_sets.emplace_back(tmp, i, i);
        if(i > 1) {
            pearl_sets.emplace_back(tmp+prev_val, i-1, i);
        }
        if(i > 2) {
            pearl_sets.emplace_back(tmp+prev_val+prev_val_2, i-2, i);
        }
        prev_val_2 = prev_val;
        prev_val = tmp;
    }

    sort(pearl_sets.begin(), pearl_sets.end(), std::greater<tuple<val_t, size_t, size_t>>());

    val_t ans = 0;
    bool taken_three = false;
    vector<bool> used;
    used.resize(n+2);

    ans = best_possible(pearl_sets.begin(), pearl_sets.end(), false, move(used));

    printf("%lu\n", ans);
    return 0;
}
