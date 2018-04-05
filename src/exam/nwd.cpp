//
// Created by kowal on 17.02.18.
//

#include <set>
#include <vector>
#include <cstdio>

using namespace std;

int log_2(int n) {
    int ans = 0;
    int ans_val = 1;
    while (ans_val < n) {
        ans_val = ans_val << 1;
        ans++;
    }
    return ans;
}

int nwd(int a, int b) {
    if(a==1 || b==1) {
        return 1;
    }
    if(a==0) {
        return b;
    }
    if(b==0) {
        return a;
    }
    if(a > b) {
        return nwd(a%b, b);
    } else {
        return nwd(b%a, a);
    }
}

int n;
set<int> s; // val, nwd_without_val
int current_nwd = 0;

int main() {
    scanf("%d", &n);
    getchar(); // '\n'
    for(int i=0; i<n; i++) {
        int val;
        char op;
        op = static_cast<char>(getchar());
        getchar(); // ' '
        scanf("%d", &val);
        getchar(); // \n

        if(op == '+') {
            if(s.empty()) {
                current_nwd = val;
            } else {
                int nwd_without_val = current_nwd;
                current_nwd = nwd(nwd_without_val, val);
            }
            s.emplace(val);
        } else {
            s.erase(val);
            current_nwd = s.begin() == s.end() ? 1 : *(s.begin());
            for(auto it = s.begin(); it != s.end(); it++) {
                current_nwd = nwd(current_nwd, *it);
                if(current_nwd == 1) {
                    break;
                }
            }
        }
        printf("%d\n", current_nwd);
    }
    return 0;
}
