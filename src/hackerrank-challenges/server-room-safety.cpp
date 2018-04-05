//
// Created by kowal on 16.02.18.
//

#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

int n;
vector<int> pos;
vector<int> height;

int main() {
    scanf("%i", &n);
    pos.reserve(static_cast<unsigned long>(n));
    height.reserve(static_cast<unsigned long>(n));
    for(int i=0; i<n; i++) {
        int tmp;
        scanf("%i", &tmp);
        pos.emplace_back(tmp);
    }
    for(int i=0; i<n; i++) {
        int tmp;
        scanf("%i", &tmp);
        height.emplace_back(tmp);
    }

    bool l_fall, r_fall;
    l_fall = false;
    r_fall = false;

    // ltr
    int i = 0;
    int j = pos[i] + height[i];
    i++;
    while(i<n && pos[i] <= j) {
        j = max(j, pos[i]+height[i]);
        i++;
    }
    if(i == n) {
        l_fall = true;
    }

    // rtl
    i = n-1;
    j = pos[i] - height[i];
    i--;
    while(i>=0 && pos[i] >= j) {
        j = min(j, pos[i]-height[i]);
        i--;
    }
    if(i == -1) {
        r_fall = true;
    }

    if(l_fall && r_fall && n>0) {
        printf("BOTH\n");
    } else if(l_fall && n>0) {
        printf("LEFT\n");
    } else if(r_fall && n>0) {
        printf("RIGHT\n");
    } else {
        printf("NONE\n");
    }

    // ltr
//    for(size_t i=1; i<n; i++) {
//        if(pos[i-1] + height[i-1] >= pos[i]) {
//            l_falls++;
//        }
//        if(pos[i] - height[i] <= pos[i-1]) {
//            r_falls++;
//        }
//    }
//    if(l_falls == n-1 && r_falls == n-1 && n>0) {
//        printf("BOTH\n");
//    } else if(l_falls == n-1 && n>0) {
//        printf("LEFT\n");
//    } else if(r_falls == n-1 && n>0) {
//        printf("RIGHT\n");
//    } else {
//        printf("NONE\n");
//    }
    return 0;
}
