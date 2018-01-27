#include "virtaddr.h"


int main (void) {
  virtaddr_t a = alloc(triple_t, 2);
  a[0] = lifetimes_make_first_triple(3, 10, false);
  a[1] = NULL;

  printf("%d %zu\n", 3, attributes_len_triples( a ));
  printf("%d %zu\n", 10, attributes_len_virtual( a ));

  free(a[0]);
  free(a[1]);
  free(a);
}
