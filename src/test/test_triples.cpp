#include <criterion/criterion.h>
#include "../virtaddr.hpp"

namespace va = virtual_addressing;

Test(flags, all) {
  cr_assert_eq(
    0,
    va::triples::flags::holder()
  );

  static const va::triples::flags::flag_each_t fls[] = {
    va::triples::flags::flag_each_t::FLAG_NOTATION, va::triples::flags::flag_each_t::FLAG_LAST_FLAG
  };

  cr_assert_eq(
    1 << va::triples::flags::flag_each_t::FLAG_NOTATION,
    va::triples::flags::holder(fls)
  );

  static const va::triples::flags::flag_each_t fls2[] = {
    va::triples::flags::flag_each_t::FLAG_LAST_FLAG
  };

  cr_assert_eq(
    0,
    va::triples::flags::holder(fls2)
  );

  const auto flh = va::triples::flags::holder(fls);

  cr_assert(va::triples::flags::has(flh, va::triples::flags::flag_each_t::FLAG_NOTATION));
}
/*
Test(attributes, getting) {
  static const va::index_t tlen = 5;
  static const va::triple_atom_t count[tlen][3] = {
    {10000, 220000, 0},
    {1000, 22000, 0},
    {100, 2200, 0},
    {10, 220, 0},
    {1, 22, 0},
  };

  static const va::triple_atom_t slice[tlen][3] = {
    {10000, 220000, 1123530},
    {1000, 22000, 1123530},
    {100, 2200, 1123530},
    {10, 220, 1120},
    {1, 22, 110},
  };

  static const va::triple_atom_t also_count[tlen][3] = {
    {10000, 220000, 10},
    {1000, 22000, 10},
    {100, 2200, 10},
    {10, 220, 10},
    {1, 22, 10},
  };
}

Test(lifetimes, giveth) {

}

Test(lifetimes, taketh) {

}

Test(mutations, all) {

}
*/
