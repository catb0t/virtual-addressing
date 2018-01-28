// creating / deleting vaddrs
#include "../virtaddr.hpp"

namespace virtual_addressing {
  namespace lifetimes {

    namespace triple_building {

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

    namespace copying {
      virtaddr_t copy (const virtaddr_t va) {
        const index_t count_triples = virtual_addressing::attributes::metadata::deducing::real_length(va);
        virtaddr_t copy = static_cast<virtaddr_t> (std::memcpy(
          alloc(virtaddr_t, 1),
          va,
          nbytes(triple_t, count_triples)
        ));

        // copy sub-pointers here
        for (size_t i = 1; i < count_triples - 1; i++) {
          copy[i] = static_cast<triple_t> (std::memcpy(
            alloc(triple_t, 1),
            va[i],
            nbytes(value_t, TRIPLE_LENGTH))
          );
        }

        return copy;
      }
    }

    // the KERNEL hath giveth,
    virtaddr_t giveth (const bool use_range_notation) {

      static const index_t empty_triple_count = 2;

      virtaddr_t res = static_cast<virtaddr_t> alloc(triple_atom_t*, empty_triple_count);
      res[0] = lifetimes::triple_building::first_triple(0, 0, use_range_notation);
      res[1] = nullptr;
      return res;
    }

    virtaddr_t giveth (const index_t length, const bool use_range_notation) {

      virtaddr_t res = static_cast<virtaddr_t> alloc(triple_atom_t*, length);
      res[0] = lifetimes::triple_building::first_triple(0, 0, use_range_notation);
      for (size_t i = 1; i < length - 1; i++) {
        // for 0s as arguments, the result is the same between make_triple candidates
        res[i] = lifetimes::triple_building::triple(0, 0, 0);
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
      const index_t count_triples = virtual_addressing::attributes::metadata::deducing::real_length(vaddr);

      if (count_triples > 0) {
        for (size_t i = 0; i < count_triples; i++) {
          std::free( vaddr[i] );
        }
      }
      std::free( vaddr );
    }

    void taketh (const virtaddr_t* vaddr, const index_t length) {
      for (size_t i = 0; i < length; i++) {
        lifetimes::taketh(vaddr[i]);
      }
    }

    void taketh (const index_t argc, const virtaddr_t n_vaddr, ...) {
      va_list va;
      va_start(va, n_vaddr);

      for (size_t i = 0; i < argc; i++) {
        virtaddr_t virt = va_arg(va, virtaddr_t);
        lifetimes::taketh(virt);
      }

      va_end(va);
    }

  }
}