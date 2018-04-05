//
// Created by kowal on 15.02.18.
//

#include <set>
#include <cstdio>
#include <unordered_set>

using namespace std;


int d;
unsigned long n;
unordered_set<int> used;
unordered_set<int> diff; // contains elements that are different by d


int main() {
    scanf("%d %d", &n, &d);
    for(int i=0; i<n; i++) {
        int op, val;
        scanf("%d %d", &op, &val);
        if(op == 1) {
            used.emplace(val);
            if(used.find(val-d) != used.end()) {
                diff.emplace(val);
                diff.emplace(val-d);
            }
            if(used.find(val+d) != used.end()) {
                diff.emplace(val);
                diff.emplace(val+d);
            }
        } else { // op == -1
            used.erase(val);
            diff.erase(val);
            if(diff.find(val-d) != diff.end() && diff.find(val-2*d) == diff.end()) {
                diff.erase(val-d);
            }
            if(diff.find(val+d) != diff.end() && diff.find(val+2*d) == diff.end()) {
                diff.erase(val + d);
            }
        }
        if(diff.empty()) {
            printf("NIE\n");
        } else {
            printf("TAK\n");
        }
    }
    return 0;
}