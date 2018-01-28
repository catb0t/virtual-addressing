// working with three-tuples
#include "../virtaddr.hpp"

namespace virtual_addressing {
  namespace triples {
    namespace attributes {

    }

    namespace flags {
      flag_holder_t holder (const flag_each_t* fl) {

        flag_holder_t total = 0;

        for (size_t i = 0; flag_each_t::FLAG_LAST_FLAG != fl[i] ; i++) {
          total |= 1U << fl[i];
        }

        std::printf("total: %lu\n", total);
        return total;
      }

      bool has (const flag_holder_t haystack, const flag_each_t needle) {
        return (bool) (1U & (haystack >> needle));
      }

    }

    namespace lifetimes {
      static triple_t _impl_copy (const void* const triple);

      /*
        the leading triple in any virtaddr_t
        the arguments will be stored in the same order as they are given here
      */
      triple_t first_triple (const value_t count_triples, const value_t virtual_length, const triples::flags::flag_holder_t fls) {
        const triple_atom_t stack_data[ TRIPLE_LENGTH ] = {count_triples, virtual_length, (value_t) fls};

        return triples::lifetimes::copy(stack_data);
      }

      /*
        make any triple
      */
      triple_t triple (const value_t value, const value_t zeroes) {
        const triple_atom_t stack_data[ TRIPLE_LENGTH ] = {value, zeroes, 0};

        return triples::lifetimes::copy(stack_data);
      }

      triple_t triple (const value_t value, const index_t bottom, const index_t top) {
        const triple_atom_t stack_data[ TRIPLE_LENGTH ] = {value, bottom, top};

        return triples::lifetimes::copy(stack_data);
      }

      triple_t copy (const triple_t triple) {
        return triples::lifetimes::_impl_copy(triple);
      }

      triple_t copy (const triple_atom_t* const triple) {
        return triples::lifetimes::_impl_copy(triple);
      }

      static triple_t _impl_copy (const void* const triple) {
        return (triple_t)
        std::memcpy(
          alloc(triple_atom_t, TRIPLE_LENGTH),
          triple,
          nbytes(triple_atom_t, TRIPLE_LENGTH)
        );
      }
    }
  }
}
