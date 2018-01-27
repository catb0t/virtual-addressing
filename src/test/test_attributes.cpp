#include <criterion/criterion.h>
#include "../virtaddr.hpp"

Test(attributes, count_triples) {

  virtual_addressing::virtaddr_t a = static_cast<virtual_addressing::virtaddr_t> alloc(virtual_addressing::triple_atom_t*, 2);
  a[0] = virtual_addressing::lifetimes::triples::first_triple(3, 10, false);
  a[1] = nullptr;

  cr_assert_eq(3, virtual_addressing::attributes::metadata::real_length( a));
  cr_assert_eq(10, virtual_addressing::attributes::metadata::virtual_length( a ));
}
