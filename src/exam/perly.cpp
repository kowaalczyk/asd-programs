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
val_t best_results[100000][4][2]; // calculated answers for [begin][last taken][taken three]
bool calculated[100000][4][2]; // check if answer is meaningful


val_t best_possible(size_t begin, size_t end, size_t last_taken, bool taken_three) {
    if(begin >= end) {
        return 0;
    }
    if(calculated[begin][last_taken][taken_three]) {
        return best_results[begin][last_taken][taken_three];
    }
    val_t ans = 0;
    if(last_taken >= 2 && taken_three) {
        ans = best_possible(begin + 1, end, 0, taken_three);
    } else {
        bool new_taken_3 = taken_three || (last_taken == 2);
        ans = max(
                pearls[begin] + best_possible(begin + 1, end, last_taken + 1, new_taken_3),
                best_possible(begin + 1, end, 0, taken_three)
        );
    }
    best_results[begin][last_taken][taken_three] = ans;
    calculated[begin][last_taken][taken_three] = true;
    return ans;
}

int main() {
    size_t n;
    scanf("%zu", &n);

    pearls.reserve(n);
    for(int i=0; i<n; i++) {
        val_t tmp;
        scanf("%lu", &tmp);

        pearls.push_back(tmp);
    }

    for(int i=0; i<100000; i++) {
        for(int j=0; j<4; j++) {
            for(int k=0; k<2; k++) {
                calculated[i][j][k] = false;
                best_results[i][j][k] = 0;
            }
        }
    }

    val_t ans = best_possible(0, n, 0, false);
    printf("%lu\n", ans);
    return 0;
}
