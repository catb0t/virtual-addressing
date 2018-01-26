#include "virtaddr.h"

int identity (fat_virtaddr_t* x, int a, ...);
int identity (fat_virtaddr_t* x, int a, ...) {
  // note: no va_start etc
  (void) a;
  return x->x;
}

int main (void) {
  fat_virtaddr_t* a = alloc(fat_virtaddr_t, 1);
  a->x = 9;
  a->identity = &identity;
  int y = I( a, identity, 0, 0 );
  printf("%d\n", y);
  return 0;
}
