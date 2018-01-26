#include <iostream>
#include <tuple>
#include <set>


using namespace std;

typedef pair<int, int64_t > elem_data_t;

int main() {
    // ios_base::sync_with_stdio(false);
    int n;
    cin >> n;

    // structure to hold elements
    int64_t  sum = 0;
    set<elem_data_t> pos; // position, inserted value, (value-prev_value)^2

    // guards at the beginning
    pos.emplace(0, 0);

    for (int i = 0; i < n; i++) {
        int k;
        cin >> k;

        auto ins_pair = pos.emplace(k, 0); // k is unique so we will have it in a good order
        auto iter = ins_pair.first; // set::iterator

        // base case
        auto curr_iter = iter;
        auto prev_iter = iter;
        prev_iter--;
        auto next_iter = iter;
        next_iter++;
        auto past_next_iter = next_iter;
        past_next_iter++;

        elem_data_t prev_element = *(prev_iter);
        elem_data_t curr_element = *(iter);
        elem_data_t next_element = *(next_iter);

        int64_t  decrease_by = 0;
        int64_t increase_by = 0;

        if (prev_iter != pos.begin()) {
            // for a first not-guard element, square should always be 0

            // update square for current value by removing and adding item
            get<1>(curr_element) = (get<0>(curr_element) - get<0>(prev_element))
                                   * (get<0>(curr_element) - get<0>(prev_element));
            curr_iter = pos.erase(curr_iter);
            pos.insert(curr_iter, curr_element);

            increase_by += get<1>(curr_element);
        }

        if (next_iter != pos.end()) {
            // updating square only for existing elements

            decrease_by = get<1>(next_element); // calculate previous square to subtract later

            // update new square for next element by removing and adding item
            get<1>(next_element) = (get<0>(next_element) - get<0>(curr_element))
                                 * (get<0>(next_element) - get<0>(curr_element));
            next_iter = pos.erase(next_iter);
            pos.insert(next_iter, next_element);

            increase_by += get<1>(next_element);
        }

        // update and print result
        sum = sum + increase_by - decrease_by ;
        if(i>0){
            cout << sizeof(int64_t) << endl;
        }
    }

    return 0;
}
