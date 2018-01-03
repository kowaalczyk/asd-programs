#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>

using namespace std;

const int N_MAX = 300000;
const int N_LOG_MAX = 20; // more than enough

string word = "";
int order[N_LOG_MAX][N_MAX]; /// order[subword_length][subword_start_pos] := i <==> subword is i-th largest among its length


/// returns floor of log_2(x)
inline int log_floor(int x) {
    int ans = 0;
    while (x >>= 1) ++ans;
    return ans;
}

/// returns 2^x
inline int pow_2(int x) {
    return (x > 0 ? 2<<(x-1) : 1);
}

/// populates order array for faster comparison of word substrings, requires word to be loaded.
void process_word(int word_size) {
    int size_log = log_floor(word_size);

    // level 0
    for(int j=0; j<word_size; j++) {
        order[0][j] = word[j]; // casting char to int
    }

    // further levels
    for(int i=1; i<=size_log; i++) { // TODO: \gt or \geq ?
        vector<tuple<int, int, int>> prev_order;
        int prev_length = pow_2(i-1);
        for(int j=0; j<word_size; j++) {
            prev_order.emplace_back(order[i-1][j], order[i-1][j+prev_length], j);
        }
        sort(prev_order.begin(), prev_order.end());

        int current_order_position = 0; // position of currently accessed word =
        for(int j=0; j<word_size; j++) {
            int order_index = get<2>(prev_order.at((unsigned int)j));
            if(j>0) {
                if(get<0>(prev_order.at((unsigned int)(j))) == get<0>(prev_order.at((unsigned int)(j-1))) && get<1>(prev_order.at((unsigned int)(j))) == get<1>(prev_order.at((unsigned int)(j-1)))) {
                    // new order position is same as for previous element because they are equal
                } else {
                    current_order_position++;
                }

            }
            order[i][order_index] = current_order_position;
        }
    }
}

/// compares substring word[a..b] with word[c..d], returns '<', '>' or '='
char lex_compare(int a, int b, int c, int d) {
    int len_left = b-a+1;
    int len_right = d-c+1;

    int left_logfloor = log_floor(len_left);
    int right_logfloor = log_floor(len_right);
    int min_level = min(left_logfloor, right_logfloor);
    int min_level_length = pow_2(min_level);
    int min_level_cmp = order[min_level][a] - order[min_level][c];

    if(min_level_cmp < 0) {
        return '<';
    } else if(min_level_cmp > 0) {
        return '>';
    } else if(min_level_length == len_left && min_level_length == len_right) {
        // cmp==0 and both sides are equal on full length
        return '=';
    } else if(min_level_length == len_right) {
        // cmp==0 and compared on full right length
        return '>';
    } else if(min_level_length == len_left) {
        // cmp==0 and compared on full left length
        return '<';
    }
    // need to compare further
    return lex_compare(a+min_level_length, b, c+min_level_length, d);
}


int main() {
    ios_base::sync_with_stdio(false);
    // init order array
    for (auto &i : order) {
        for (int &j : i) {
            j = 0;
        }
    }
    // load word
    int n, m;
    cin >> n >> m;
    cin >> word;
    // prepare word for queries
    process_word(n);
    // load queries and print answers
    for(int i=0; i<m; i++) {
        int a, b, c, d;
        cin >> a >> b >> c >> d;
         cout << lex_compare(a-1, b-1, c-1, d-1) << endl;
    }
    return 0;
}