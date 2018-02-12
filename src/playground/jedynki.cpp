//
// Created by kowal on 12.02.18.
//

#include <iostream>
#include <unordered_set>

using namespace std;

int main() {
    int n;
    scanf("%d", &n);

    int tab[n];
    for(int i=0; i<n; i++) {
        scanf("%d", &tab[i]);
    }

    bool used_exps[1000000];
    int used_exps_count = 0;
    for (int i = 0; i < 1000000; ++i) {
        used_exps[i] = false;
    }

    for(int i=0; i<n; i++) {
        int new_exp = tab[i];
        while(used_exps[new_exp]) {
            used_exps[new_exp] = false;
            new_exp++;
            used_exps_count--;
        }
        used_exps[new_exp] = true;
        used_exps_count++;

        printf("%d\n", used_exps_count);
    }

    return 0;
}