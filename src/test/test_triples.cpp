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
}

Test(attributes, getting) {

}

Test(lifetimes, giveth) {

}

Test(lifetimes, taketh) {

}

Test(mutations, all) {

}
