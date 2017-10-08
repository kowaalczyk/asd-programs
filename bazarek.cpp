#include <iostream>

using namespace std;

int main() {
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

      price_sum = prefix_sum[n - 1] - prefix_sum[x - 1];
      if(!(price_sum % 2)) {
        if(largest_odd_before[x] - smallest_even_not_before[x] > largest_even_before[x] - smallest_odd_not_before[x]) {
          if(largest_odd_before[x] != 0 && smallest_even_not_before[x] != 0) {
            price_sum += largest_odd_before[x] - smallest_even_not_before[x];
          }
        } else {
          if(largest_even_before[x] != 0 && smallest_odd_not_before[x] != 0) {
            price_sum += largest_even_before[x] - smallest_odd_not_before[x];
          }
        }
      }
    }

    if(unable || !(price_sum % 2)) {
      cout<<-1<<endl;
    } else {
      cout<<price_sum<<endl;
    }

    //prev version
//     long long current_price = 0;
//     int current_products = 0;
//     int smallest_odd = 0;
//     int smallest_even = 0;
//
//    int j;
//    for(j = n-1; j >= 0 && current_products < products_to_buy; j--) {
//       int p = price[j];
//
//      current_price += p;
//      current_products++;
//
//      if(p % 2) {
//        smallest_odd = p;
//      } else {
//        smallest_even = p;
//      }
//    }
//
//    if(!(current_price % 2)) {
//       int biggest_odd = 0;
//       int biggest_even = 0;
//
//      for(int k=0; k<=j; k++) {
//         int p = price[k];
//        if(p % 2) {
//          biggest_odd = p;
//        } else {
//          biggest_even = p;
//        }
//      }
//
//      if(biggest_odd - smallest_even > biggest_even - smallest_odd) {
//        if(biggest_odd != 0 && smallest_even != 0) {
//          current_price += biggest_odd - smallest_even;
//        }
//      } else {
//        if(biggest_even != 0 && smallest_odd != 0) {
//          current_price += biggest_even - smallest_odd;
//        }
//      }
//    }
//
//    if(current_products == products_to_buy && current_price % 2) {
//      cout<<current_price<<endl;
//    } else {
//      cout<<"-1"<<endl;
//    }
  }

  return 0;
}
