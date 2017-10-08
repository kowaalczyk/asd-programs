#include <iostream>
using namespace std;

int main() {
  int n, m;
  int price[1000000];
  int day_products[1000000];
  //

  long long sum = 0;

  cin >> n;
  for (int i=0; i<m; i++) {
    cin >> price[i];
    //TODO: Uwazac na granice przedzialow (moze chciec kupic wiecej produktow niz jest wszystkich)

    // partial_sums[i] = price[i];
    // if(i>0) {
    //   partial_sums[i] += partial_sums[i-1];
    // }
  }
  cin >> m;
  for (int i=0; i<m; i++) {
    cin >> day_products[i];
  }



  return 0;
}
