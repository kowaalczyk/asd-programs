//
// Created by kowal on 05.04.18.
//

#include <cstdio>
#include <vector>

using namespace std;

vector<int> v;
int suffix_skip[100000];
int preffix_skip[100000];

int main() {
    int n;
    scanf("%d", &n);
    v.reserve(static_cast<unsigned long>(n));

    int tmp;
    int current_prev = 0;
    int prev_el = -1;
    for(int i=0; i<n; i++) {
        scanf("%d", &tmp);
        v.emplace_back(tmp);
        if(v[i] == prev_el) {
            current_prev++;
            preffix_skip[i] = current_prev;
        } else {
            current_prev = 0;
            preffix_skip[i] = 0;
        }
        prev_el = v[i];
    }

    current_prev = 0;
    prev_el = -1;
    for(int i=n-1; i>=0; i--) {
        if(v[i] == prev_el) {
            current_prev++;
            suffix_skip[i] = current_prev;
        } else {
            current_prev = 0;
            suffix_skip[i] = 0;
        }
        prev_el = v[i];
    }

    bool found = false;
    for(int dist=n-1; dist>0; dist--) {
        for(int i=0; i+dist<n; i+=min(suffix_skip[i], preffix_skip[n-1])+1) {
            if(v[i] != v[i+dist]) {
                printf("%d\n", dist);
                found = true;
                break;
            }
        }
        if(found) {
            break;
        }
    }

    if(!found) {
        printf("BRAK\n");
    }

    return 0;
}