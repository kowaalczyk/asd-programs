// problem description (pl): https://szkopul.edu.pl/c/laboratorium-z-asd-20172018/p/baz/

#include <iostream>

using namespace std;

int main() {
  ios::sync_with_stdio(false);

  int n, m;
  int price[1000000];
  int products_to_buy;

  int largest_even_before[1000000];
  int largest_odd_before[1000000];
  int smallest_even_not_before[1000000];
  int smallest_odd_not_before[1000000];
  long long prefix_sum[1000000];

  cin >> n;

  //reading prices and caching largest befores and prefix sums
  int remembered_odd = 0;
  int remembered_even = 0;
  for (int i = 0; i < n; i++) {
    int p;
    cin >> p;
    price[i] = p;

    prefix_sum[i] = p;
    if(i>0) {
      prefix_sum[i] += prefix_sum[i-1];
    }

    largest_even_before[i] = remembered_even;
    largest_odd_before[i] = remembered_odd;

    if(p % 2) {
      remembered_odd = p;
    } else {
      remembered_even = p;
    }
  }

  //caching smallest afters
  remembered_odd = 0;
  remembered_even = 0;
  for(int i = n-1; i>=0; i--) {
    int p = price[i];
    if(p % 2) {
      remembered_odd = p;
    } else {
      remembered_even = p;
    }
    smallest_odd_not_before[i] = remembered_odd;
    smallest_even_not_before[i] = remembered_even;
  }

  //calculating answers for each number of products in constant time
  cin >> m;
  for (int i = 0; i < m; i++) {
    cin >> products_to_buy;

    long long price_sum = 0;
    bool unable = false;
    if(products_to_buy > n) {
      unable = true;
    } else {
      int x = n - products_to_buy;

      price_sum = x>0 ? prefix_sum[n - 1] - prefix_sum[x - 1] : prefix_sum[n - 1];
      if(!(price_sum % 2)) {
        //hotfix:
        if(largest_odd_before[x] && smallest_even_not_before[x]) {
          if(largest_even_before[x] && smallest_odd_not_before[x]) {
            //both options are defined - compare and use the optimal one
            if(largest_odd_before[x] - smallest_even_not_before[x] > largest_even_before[x] - smallest_odd_not_before[x]) {
              price_sum -= smallest_even_not_before[x];
              price_sum += largest_odd_before[x];
            } else {
              price_sum -= smallest_odd_not_before[x];
              price_sum += largest_even_before[x];
            }
          } else {
            //use odd before, even after
            price_sum -= smallest_even_not_before[x];
            price_sum += largest_odd_before[x];
          }
        } else {
          if(largest_even_before[x] && smallest_odd_not_before[x]) {
            //use even before, odd after
            price_sum -= smallest_odd_not_before[x];
            price_sum += largest_even_before[x];
          } else {
            //none are defined - can't change price_sum
          }
        }
      }
    }

    if(unable || !(price_sum % 2)) {
      cout<<-1<<endl;
    } else {
      cout<<price_sum<<endl;
    }
  }

  return 0;
}
