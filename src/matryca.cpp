// problem description (pl): https://szkopul.edu.pl/c/laboratorium-z-asd-20172018/p/mat/

#include <iostream>
using namespace std;

int main() {
  string s;
  getline(cin, s);
  unsigned long N = s.length();

  char last_requested = s[0];
  unsigned long min_distance = N;
  unsigned long current_distance = 0;

  for(unsigned long i=0; i<N; i++) {
    char current_color = s[i];
    current_distance++;

    if(current_color != last_requested && current_color != '*') {
      if(current_distance < min_distance) {
        min_distance = current_distance;
      }
      last_requested = current_color;
    }
    if(current_color == last_requested) {
      current_distance = 0;
    }
  }

  cout << N - min_distance + 1;

  return 0;
  //Jezeli mamy we wzorze AB (obok siebie), to matryca musi byc dlugosci wzoru
  //1 gwiazdka ==> matryca krotsza o 1 od dlugosci wzoru (itd.)
  //ale sa przypadki szczegolne: same A, same B, same *
  //odp: n-m+1
}
