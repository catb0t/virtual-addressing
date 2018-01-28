#include <criterion/criterion.h>
#include "../virtaddr.hpp"

using namespace virtual_addressing;

Test(attributes, getting) {

  auto a = lifetimes::giveth();

  static const triples::flags::flag_each_t fls[] = { triples::flags::FLAG_NOTATION, triples::flags::flag_each_t::FLAG_LAST_FLAG };

  a[0] = triples::lifetimes::first_triple( 3, 10, triples::flags::holder( fls ) );
  a[1] = nullptr;

  cr_assert_eq(3, attributes::metadata::getting::real_length( a ));
  cr_assert_eq(10, attributes::metadata::getting::virtual_length( a ));
  cr_assert_eq(true, attributes::metadata::getting::notation_flag( a ));

  std::free(a[0]);
  static const triples::flags::flag_each_t fls2[] = { triples::flags::flag_each_t::FLAG_LAST_FLAG };

  a[0] = triples::lifetimes::first_triple( 45, 100, triples::flags::holder( fls2 ) );

  cr_assert_eq(45, attributes::metadata::getting::real_length( a ));
  cr_assert_eq(100, attributes::metadata::getting::virtual_length( a ));
  cr_assert_eq(false, attributes::metadata::getting::notation_flag( a ));

  lifetimes::taketh(a);
}
