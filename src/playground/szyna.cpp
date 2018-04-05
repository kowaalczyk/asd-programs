//
// Created by kowal on 13.02.18.
//

#include <cstdio>
#include <vector>
#include <unordered_set>

using namespace std;


int main() {
    unsigned long n;
    scanf("%lu", &n);

    unordered_set<int> s;
    int ans = 0;
    for(unsigned int i=0; i<n; i++) {
        int tmp;
        scanf("%d", &tmp);
        if(s.find(tmp) != s.end()) {
            ans++;
            s.clear();
        } else {
            s.emplace(tmp);
        }
    }
    printf("%d\n", ans);

    return 0;
}
