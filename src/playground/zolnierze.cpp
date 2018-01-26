//
// Created by kowal on 26.01.18.
//

#include <cstdio>
#include <utility>
#include <set>

using namespace std;

set<int> z;

int main() {
    int n;
    scanf("%d", &n);

    z.emplace(-1);
    for(int i=1; i<=n; i++) {
        z.emplace(i);
    }

    for(int i=0; i<n; i++) {
        int ex;
        scanf("%d", &ex);

        set<int>::iterator er_pos = z.find(ex);
        auto right = next(er_pos) == z.end() ? -1 : *(next(er_pos));
        printf("%d %d\n", *(prev(er_pos)), right);
        z.erase(ex);
    }

    return 0;
}