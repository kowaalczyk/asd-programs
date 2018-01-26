//
// Created by kowal on 26.01.18.
//

#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>

//const int data_size = 1000000000;


using namespace std;

int main() {
    ios_base::sync_with_stdio(false);

    size_t n; cin >> n;
    vector<int_fast32_t> tab;
    tab.reserve(n);

    for(size_t i=0; i<n; i++) {
        int_fast32_t tmp;
        cin >> tmp;
        tab.push_back(tmp);
    }

    sort(tab.begin(), tab.end());

    size_t curr_begin = 0;
    size_t max_len = 0;
    for(size_t i=1; i<n; i++) {
        if(tab[i] - tab[i-1] > 1) {
            size_t curr_len = i - curr_begin;
            if (curr_len > max_len) {
                max_len = curr_len;
            }
            curr_begin = i;
        }
    }
    size_t curr_len = n - curr_begin;
    if (curr_len > max_len) {
        max_len = curr_len;
    }
    cout << max_len << endl;

    return 0;
}