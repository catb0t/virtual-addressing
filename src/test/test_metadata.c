#include <criterion/criterion.h>
#include "../virtaddr.h"

Test(metadata, count) {
  virtaddr_t a = alloc(triple_t, 2);
  a[0] = lifetimes_make_first_triple(3, 10, false);
  a[1] = NULL;

  printf("%zu\n", attributes_len_virtual( a ));

  cr_assert_eq(3, attributes_len_triples( a ));
  cr_assert_eq(10, attributes_len_virtual( a ));
}
