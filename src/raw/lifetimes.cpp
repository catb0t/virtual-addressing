// creating / deleting vaddrs
#include "../virtaddr.hpp"

namespace virtual_addressing {
  namespace lifetimes {

    namespace triples {

      /*
        the leading triple in any virtaddr_t
        the arguments will be stored in the same order as they are given here
      */
      triple_t first_triple (const value_t count_triples, const value_t virtual_length, const bool range_notation) {
        static const triple_atom_t stack_data[ TRIPLE_LENGTH ] = {count_triples, virtual_length, (value_t) range_notation};

        return
          (triple_t)
          std::memcpy(alloc(value_t, TRIPLE_LENGTH), stack_data, nbytes(value_t, TRIPLE_LENGTH));
      }

      /*
        make any triple
      */
      triple_t triple (const value_t value, const value_t zeroes) {
        static const triple_atom_t stack_data[ TRIPLE_LENGTH ] = {value, zeroes, 0};

        return
          (triple_t)
          std::memcpy(alloc(triple_atom_t, TRIPLE_LENGTH), stack_data, nbytes(triple_atom_t, TRIPLE_LENGTH));
      }

      triple_t triple (const value_t value, const index_t bottom, const index_t top) {
        static const triple_atom_t stack_data[ TRIPLE_LENGTH ] = {value, bottom, top};

        return
          (triple_t)
          std::memcpy(alloc(triple_atom_t, TRIPLE_LENGTH), stack_data, nbytes(triple_atom_t, TRIPLE_LENGTH));
      }
    }

    // the KERNEL hath giveth,
    virtaddr_t giveth (const bool use_range_notation) {

      static const index_t empty_triple_count = 2;

      virtaddr_t res = static_cast<virtaddr_t> alloc(triple_atom_t*, empty_triple_count);
      res[0] = virtual_addressing::lifetimes::triples::first_triple(0, 0, use_range_notation);
      res[1] = nullptr;
      return res;
    }

    virtaddr_t giveth (const index_t length, const bool use_range_notation) {

      virtaddr_t res = static_cast<virtaddr_t> alloc(triple_atom_t*, length);
      res[0] = virtual_addressing::lifetimes::triples::first_triple(0, 0, use_range_notation);
      for (size_t i = 1; i < length - 2; i++) {
        // for 0s as arguments, the result is the same between make_triple candidates
        res[i] = virtual_addressing::lifetimes::triples::triple(0, 0, 0);
      }
      res[ length - 1 ] = nullptr;
      return res;
    }

    virtaddr_t giveth (const value_t* source, const index_t length, const bool use_range_notation) {
      (void) use_range_notation;
      (void) length;
      (void) source;
      return {};
    }

    // and the KERNEL hath taken away; blessed be the name of the KERNEL.
    void taketh (const virtaddr_t vaddr) {
      const index_t count_triples = virtual_addressing::attributes::metadata::real_length(vaddr);

      if (count_triples > 0) {
        for (size_t i = 0; i < count_triples - 1; i++) {
          std::free( vaddr[i] );
        }
      }
      std::free( vaddr );
    }

    void taketh (const virtaddr_t* vaddr, const index_t length) {
      for (size_t i = 0; i < length; i++) {
        virtual_addressing::lifetimes::taketh(vaddr[i]);
      }
    }

    void taketh (const index_t argc, const virtaddr_t n_vaddr, ...) {
      va_list va;
      va_start(va, n_vaddr);

      for (size_t i = 0; i < argc; i++) {
        virtaddr_t virt = va_arg(va, virtaddr_t);
        virtual_addressing::lifetimes::taketh(virt);
      }

      va_end(va);
    }

  }
}