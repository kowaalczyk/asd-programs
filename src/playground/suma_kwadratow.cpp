#include <iostream>
#include <set>
#include <cassert>


using namespace std;

typedef pair<uint32_t, uint64_t> elem_data_t;
set<elem_data_t> s;

int main() {
    int n;
    scanf("%d", &n);
    uint64_t ans = 0;
    for(int i=0; i<n; i++) {
        int val;
        scanf("%d", &val);

        auto ret_data = s.emplace(val, 0);
        auto pos_iter = ret_data.first;
        auto prev_iter = pos_iter;
        if(pos_iter != s.begin()) {
            prev_iter--;
            // pos iter at inserted element, prev_iter at previous one
            assert((*pos_iter).first > (*prev_iter).first);
            uint64_t new_second = ((*pos_iter).first - (*prev_iter).first)*((*pos_iter).first - (*prev_iter).first);
            s.erase(pos_iter);
            ret_data = s.emplace(val, new_second);
            ans += new_second;
            prev_iter = ret_data.first;
            pos_iter = ret_data.first;
            pos_iter++;
            if(pos_iter != s.end()) {
                // pos iter at element after inserted, prev iter at inserted
                assert((*pos_iter).first > (*prev_iter).first);
                ans -= (*pos_iter).second;
                uint64_t new_second = ((*pos_iter).first - (*prev_iter).first)*((*pos_iter).first - (*prev_iter).first);
                uint32_t tmp_first = (*pos_iter).first;
                s.erase(pos_iter);
                s.emplace(tmp_first, new_second);
                ans += new_second;
            }
        } else {
            pos_iter++;
            if(pos_iter != s.end()) {
                // pos iter at element after inserted, prev iter at inserted, but this time previous value is not subtracted
                assert((*pos_iter).first > (*prev_iter).first);
                uint64_t new_second = ((*pos_iter).first - (*prev_iter).first)*((*pos_iter).first - (*prev_iter).first);
                uint32_t tmp_first = (*pos_iter).first;
                s.erase(pos_iter);
                s.emplace(tmp_first, new_second);
                ans += new_second;
            }
        }
        if(i > 0) {
            printf("%lu\n", ans);
        }
    }

    return 0;
}
