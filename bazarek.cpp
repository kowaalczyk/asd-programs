#include <iostream>

using namespace std;

int main() {
   int n, m;
   int price[1000000];
   int products_to_buy;

  cin >> n;
  for (int i = 0; i < n; i++) {
    cin >> price[i];
  }

  cin >> m;
  for (int i = 0; i < m; i++) {
    cin >> products_to_buy;

     long long current_price = 0;
     int current_products = 0;
     int smallest_odd = 0;
     int smallest_even = 0;

    int j;
    for(j = n-1; j >= 0 && current_products < products_to_buy; j--) {
       int p = price[j];

      current_price += p;
      current_products++;

      if(p % 2) {
        smallest_odd = p;
      } else {
        smallest_even = p;
      }
    }

    if(!(current_price % 2)) {
       int biggest_odd = 0;
       int biggest_even = 0;

      for(int k=0; k<=j; k++) {
         int p = price[k];
        if(p % 2) {
          biggest_odd = p;
        } else {
          biggest_even = p;
        }
      }

      if(biggest_odd - smallest_even > biggest_even - smallest_odd) {
        if(biggest_odd != 0 && smallest_even != 0) {
          current_price += biggest_odd - smallest_even;
        }
      } else {
        if(biggest_even != 0 && smallest_odd != 0) {
          current_price += biggest_even - smallest_odd;
        }
      }
    }

    if(current_products == products_to_buy && current_price % 2) {
      cout<<current_price<<endl;
    } else {
      cout<<"-1"<<endl;
    }
  }

  return 0;
}
