//
// Created by kowal on 26.01.18.
//

#include <cstdio>
#include <queue>
#include <unordered_map>

using namespace std;

typedef size_t height_t;

priority_queue<height_t> Q;
unordered_map<size_t, size_t> count;

int main() {
    size_t n;
    scanf("%zu", &n);

    size_t uniq_count = 0;

    for (size_t i=0; i<n; i++) {
        height_t tmp;
        scanf("%lu", &tmp);

        if(count[tmp] == 0) {
            uniq_count++;
            Q.push(tmp);
        }
        count[tmp]++;
    }

    size_t m;
    scanf("%zu", &m);
    for(size_t i=0; i<m; i++) {
        size_t k;
        scanf("%zu", &k);

        size_t done = 0;
        while(done < k) {
            size_t currently_removed = 0;

            // remove as much biggest stacks as possible from queue
            height_t max_stack = Q.top();
            if(count[max_stack] > (k-done)) {
                currently_removed += (k-done);
                count[max_stack] -= currently_removed;
            } else {
                currently_removed += count[max_stack];
                count[max_stack] = 0;
            }
            done += currently_removed;

            // if no more stacks of that size are present, remove size from the queue
            if(count[max_stack] == 0) {
                uniq_count--;
                Q.pop();
            }

            // insert smaller half to queue and update count of different sizes
            height_t smaller_part = max_stack/2;
            if(count[smaller_part] == 0) {
                Q.emplace(smaller_part);
                uniq_count++;
            }
            count[smaller_part]+=currently_removed;

            // same for the bigger half
            height_t bigger_part = max_stack - smaller_part;
            if(count[bigger_part] == 0) {
                Q.emplace(bigger_part);
                uniq_count++;
            }
            count[bigger_part]+=currently_removed;
        }
        printf("%zu\n", uniq_count);
    }

    return 0;
}
