// this is just a helper printing variable sizes available on current machine

#include <stdio.h>

int main() {
  printf("Size of int: %d\n", sizeof(int));
  printf("Size of long long int: %d\n", sizeof(long long int));
  printf("Size of short int: %d\n", sizeof(short int));
  return 0;
}
