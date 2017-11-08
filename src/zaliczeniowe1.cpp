#include <iostream>

using namespace std;


typedef int tree_val_t;
typedef int tree_pos_t;

static const tree_pos_t TREE_MAX_SIZE = 800008; //TODO: Double-check
static const tree_pos_t N_MAX_SIZE = 100001;
static const int MAX_ALLOWED_REVENUE = 2000000000;
static const int MIN_ALLOWED_REVENUE = 0;

tree_val_t min_t[TREE_MAX_SIZE];
tree_val_t max_t[TREE_MAX_SIZE];
tree_val_t delta[TREE_MAX_SIZE];
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
    // calculate growth periods in O(n)

    for(int I=0; I<m; I++) {
        int current_l, current_r, current_delta;
        cin >> current_l >> current_r >> current_delta;

        int ans = -1;right delta

        bool too_much = false;
        for(int i=current_l; i<=current_r; i++) {
            // check if min-max are not violated

            if(rev[i] + current_delta > MAX_ALLOWED_REVENUE || rev[i] + current_delta < MIN_ALLOWED_REVENUE) {
                too_much = true;
                break;
            }
        }
        if(!too_much) {
            // calculate values for left and right edge and compare with previous growth periods #

            // TODO
            // calc left delta
            // calc new left edge: bottom-up to LCA with updating subtree deltas
            // compare new delta with old delta
            // update growth periods #
            // repeat for right edge -> TODO: Left and right edge have to run parallel to get to LCA in O(n)

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
        // return -1 if min-max violated

        cout << ans << endl;

    }

    return 0;
}