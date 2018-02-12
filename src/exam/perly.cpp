//
// Created by kowal on 27.01.18.
//

#include <cstdio>
#include <set>
#include <vector>
#include <algorithm>

using namespace std;

typedef uint64_t val_t;

vector<val_t> pearls;
val_t best_results[4][100000];
bool calculated[4][2][1000000];

val_t best_possible(size_t begin,
                    size_t end,
                    size_t last_taken,
                    bool taken_three)
{
    if(calculated[last_taken][begin]) {
        return best_results[last_taken][begin];
    }

    if(begin > end) {
        return 0;
    }

    val_t ans = 0;
    if(last_taken >= 3) {
        ans = best_possible(begin+1, end, 0, true);
        if(best_results[last_taken][begin] == -1) {
            best_results[last_taken][begin] = ans;
            calculated[last_taken][taken_three][begin] = true;
        }
        return ans;
    } else {
        if(last_taken == 2) {
            if(taken_three) {
                ans = best_possible(begin+1, end, 0, true);
                if(best_results[last_taken][begin] == -1) {
                    best_results[last_taken][begin] = ans;
                    calculated[last_taken][taken_three][begin] = true;
                }
                return ans;
            } else {
                val_t with_3 = best_possible(begin+1, end, 3, true) + pearls[begin];
                val_t without_3 = best_possible(begin+1, end, 0, false);

                ans = max(with_3, without_3);
                if(best_results[last_taken][begin] == -1) {
                    best_results[last_taken][begin] = ans;
                    calculated[last_taken][taken_three][begin] = true;
                }
                return ans;
            }
        } else {
            // taken <= 2
            val_t with_begin = best_possible(begin+1, end, last_taken+1, taken_three) + pearls[begin];
            val_t without_begin = best_possible(begin+1, end, 0, taken_three);

            ans = max(with_begin, without_begin);
            if(best_results[last_taken][begin] == -1) {
                best_results[last_taken][begin] = ans;
                calculated[last_taken][taken_three][begin] = true;
            }
            return ans;
        }
    }
}

int main() {
    size_t n;
    scanf("%zu", &n);

    pearls.reserve(n);
    for(int i=0; i<n; i++) {
        val_t tmp;
        scanf("%lu", &tmp);

        pearls.push_back(tmp);
        for(int j=0; j<4; j++) {
            best_results[j][i] = 0;
            calculated[j][0][i] = false;
            calculated[j][1][i] = false;
        }
    }

    val_t ans = best_possible(0, n-1, 0, false);

    printf("%lu\n", ans);
    return 0;
}
