#include <cstdio>
#include "virtaddr.hpp"

int main (void) {
  virtual_addressing::virtaddr_t a = static_cast<virtual_addressing::virtaddr_t> alloc(virtual_addressing::triple_atom_t*, 2);
  a[0] = virtual_addressing::lifetimes::triple_building::first_triple(3, 10, false);
  a[1] = nullptr;

  printf("%d %zu\n", 3, virtual_addressing::attributes::metadata::getting::real_length( a));
  printf("%d %zu\n", 10, virtual_addressing::attributes::metadata::getting::virtual_length( a ));

  virtual_addressing::lifetimes::taketh(a);
}
