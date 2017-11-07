#include <iostream>

using namespace std;


//typedef int tree_val_t;
//typedef int tree_pos_t;
//
//static const tree_pos_t TREE_MAX_SIZE = 800008; //TODO: Double-check
//static const tree_pos_t N_MAX_SIZE = 100001;
static const int MAX_ALLOWED_REVENUE = 2000000000;
static const int MIN_ALLOWED_REVENUE = 0;
//
//tree_val_t min_t[TREE_MAX_SIZE];
//tree_val_t max_t[TREE_MAX_SIZE];
//tree_val_t delta[TREE_MAX_SIZE];
//bool calculated[TREE_MAX_SIZE];

// ------------------------------------------------------------------------------------------



int main() {
    int n, m;
    cin >> n;
    int rev[n+1]; // 0  is a guard (input data is indexed from 1)
    for(int i=1; i<=n; i++) {
        cin >> rev[i];
    }
    cin >> m;

    // TODO: Prepare state

    for(int I=0; I<m; I++) {
        int current_l, current_r, current_delta;
        cin >> current_l >> current_r >> current_delta;

        int ans = -1;

        bool too_much = false;
        for(int i=current_l; i<=current_r; i++) {
            if(rev[i] + current_delta > MAX_ALLOWED_REVENUE || rev[i] + current_delta < MIN_ALLOWED_REVENUE) {
                too_much = true;
                break;
            }
        }
        if(!too_much) {
            for(int i=current_l; i<=current_r; i++) {
                rev[i] += current_delta;
            }
            ans = 0;
            for(int i=2; i<=n; i++) {
                if(rev[i] > rev[i-1]) {
                    ans++;
                }
            }
        }
        cout << ans << endl;

//        if(delta > 0) {
//            if(get_max(current_l, current_r) + delta > MAX_ALLOWED_REVENUE) {
//                cout << -1 << endl;
//                continue;
//            }
//            set_max(current_l, current_r, delta);
//        } else if(delta < 0) {
//            if(get_min(current_l, current_r) + delta < MIN_ALLOWED_REVENUE) {
//                cout << -1 << endl;
//                continue;
//            }
//            set_min(current_l, current_r, delta); // update values on min path and direct neigh
//        } else {
//            // delta == 0
//        }
//        set_l; // update vabourslues in left edge and its direct neighbours
//        set_r; // update values in right edge and its direct neighbours
//
//        // return calculated new delta value based on ed
//        ans = set_positive_deltas;
//        cout << ans << endl;

    }

    return 0;
}