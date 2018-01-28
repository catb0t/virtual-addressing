#include <criterion/criterion.h>
#include "../virtaddr.hpp"

namespace virt_addr = virtual_addressing;

Test(metadata, getting) {

  auto a = virt_addr::lifetimes::giveth();

  static const virt_addr::triples::flags::flag_each_t fls[] = { virt_addr::triples::flags::FLAG_NOTATION, virt_addr::triples::flags::flag_each_t::FLAG_LAST_FLAG };

  a[0] = virt_addr::triples::lifetimes::first_triple( 3, 10, virt_addr::triples::flags::holder( fls ) );
  a[1] = nullptr;

  cr_assert_eq(3, virt_addr::attributes::metadata::getting::real_length( a ));
  cr_assert_eq(10, virt_addr::attributes::metadata::getting::virtual_length( a ));
  cr_assert_eq(true, virt_addr::attributes::metadata::getting::notation_flag( a ));

  std::free(a[0]);
  static const virt_addr::triples::flags::flag_each_t fls2[] = { virt_addr::triples::flags::flag_each_t::FLAG_LAST_FLAG };

  a[0] = virt_addr::triples::lifetimes::first_triple( 45, 100, virt_addr::triples::flags::holder( fls2 ) );

  cr_assert_eq(45, virt_addr::attributes::metadata::getting::real_length( a ));
  cr_assert_eq(100, virt_addr::attributes::metadata::getting::virtual_length( a ));
  cr_assert_eq(false, virt_addr::attributes::metadata::getting::notation_flag( a ));

  virt_addr::lifetimes::taketh(a);
}

Test(metadata, setting) {

}

Test(metadata, deducing) {

}

Test(slices, to_slice) {

}

Test(slices, to_count) {

}
