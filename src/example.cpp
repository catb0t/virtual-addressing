#include <cstdio>
#include "virtaddr.hpp"

using namespace virtual_addressing;

int main (void) {
  auto a = lifetimes::giveth();

  static const triples::flags::flag_each_t fls[] = { triples::flags::FLAG_NOTATION, triples::flags::flag_each_t::FLAG_LAST_FLAG };

  a[0] = triples::lifetimes::first_triple( 3, 10, triples::flags::holder( fls ) );
  a[1] = nullptr;

  printf("%d, %lu\n", 3, attributes::metadata::getting::real_length( a ));
  printf("%d, %lu\n", 10, attributes::metadata::getting::virtual_length( a ));
  printf("%d, %d\n", true, attributes::metadata::getting::notation_flag( a ));

  std::free(a[0]);
  static const triples::flags::flag_each_t fls2[] = { triples::flags::flag_each_t::FLAG_LAST_FLAG };

  a[0] = triples::lifetimes::first_triple( 45, 100, triples::flags::holder( fls2 ) );

  printf("%d, %lu\n", 45, attributes::metadata::getting::real_length( a ));
  printf("%d, %lu\n", 100, attributes::metadata::getting::virtual_length( a ));
  printf("%d, %d\n", false, attributes::metadata::getting::notation_flag( a ));

  lifetimes::taketh(a);
}
