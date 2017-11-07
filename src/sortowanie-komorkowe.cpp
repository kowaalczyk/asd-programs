// problem description (pl): https://szkopul.edu.pl/c/laboratorium-z-asd-20172018/p/sor/

#include <iostream>

#define BIL 1000000000

using namespace std;

int main() {
    unsigned int tab[1000];
    unsigned int n;
    cin >> n;
    for(unsigned int i=0; i<n; i++) {
        cin>>tab[i];
    }

    //answer := 1 for 1-digit tab
    if(n==1) {
        cout << 1 << endl;
        return 0;
    }

    //l_cache[length-1][first_el_pos] := # of possible (length)-sequences starting with (first_el)
    unsigned int l_cache[1000][1000];
    //r_cache[length-1][last_el_pos] := # of possible (length)-sequences ending with (last_el)
    unsigned int r_cache[1000][1000];

    //calculate cache values for 2-sequences
    r_cache[1][0] = 0;
    l_cache[1][n-1] = 0;
    for(unsigned int i=0; i<n-1; i++) {
        if(tab[i] < tab[i+1]) {
            r_cache[1][i+1] = 1;
        } else {
            r_cache[1][i+1] = 0;
        }
        if(tab[i] < tab[i+1]) {
            l_cache[1][i] = 1;
        } else {
            l_cache[1][i] = 0;
        }
    }

    //dynamically calculate values for longer sequences
    for(unsigned int length_index = 2; length_index < n; length_index++) {
        for(unsigned int start = 0; start+length_index < n; start++) {
            unsigned int end = start + length_index;


            unsigned int current_r = 0;
            if(tab[end-1] < tab[end]) {
                current_r += r_cache[length_index-1][end-1] % BIL;
            }
            if(tab[start] < tab[end]) {
                current_r += l_cache[length_index-1][start] % BIL;
            }
            r_cache[length_index][end] = current_r % BIL;

            unsigned int current_l = 0;
            if(tab[start] < tab[start+1]) {
                current_l += l_cache[length_index-1][start+1] % BIL;
            }
            if(tab[start] < tab[end]) {
                current_l += r_cache[length_index-1][end] % BIL;
            }
            l_cache[length_index][start] = current_l % BIL;
        }
    }

    //answer := # of n-sequences beginning with tab[0] + # of n-sequences ending with tab[n-1]
    cout << (r_cache[n-1][n-1] % BIL + l_cache[n-1][0] % BIL) % BIL <<endl;

    return 0;
}