//
// Created by kowal on 12.02.18.
//

#include <cstdio>
#include <set>
#include <vector>

using namespace std;


int main() {
    set<int> nums;
    int n, d;

    bool found = false;
    scanf("%d %d", &n, &d);
    for(int i=0; i<n; i++) {
        int tmp;
        scanf("%d", &tmp);

        auto possible_ans = nums.find(tmp+d);
        if(possible_ans != nums.end()) {
            printf("%d %d\n", *possible_ans, tmp);
            found = true;
            break;
        }
        possible_ans = nums.find(tmp-d);
        if(possible_ans != nums.end()) {
            printf("%d %d\n", tmp, *possible_ans);
            found = true;
            break;
        }
        nums.emplace(tmp);
    }
    if(!found) {
        printf("NIE\n");
    }

    return 0;
}
