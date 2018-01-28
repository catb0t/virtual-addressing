// creating / deleting vaddrs
#include "../virtaddr.hpp"

namespace virtual_addressing {
  namespace lifetimes {

    virtaddr_t copy (const virtaddr_t va) {
      const index_t count_triples = attributes::metadata::deducing::real_length(va);
      virtaddr_t copy = static_cast<virtaddr_t> (std::memcpy(
        alloc(virtaddr_t, 1),
        va,
        nbytes(triple_t, count_triples)
      ));

      // copy sub-pointers here
      for (size_t i = 1; i < count_triples - 1; i++) {
        copy[i] = triples::lifetimes::copy(va[i]);
      }

      return copy;
    }

    // the KERNEL hath giveth,
    virtaddr_t giveth (void) {
      return lifetimes::giveth( triples::flags::flag_holder_t {} );
    }

    virtaddr_t giveth (const triples::flags::flag_holder_t fls) {

      static const index_t empty_triple_count = 2;

      virtaddr_t res = static_cast<virtaddr_t> alloc(triple_atom_t*, empty_triple_count);
      res[0] = triples::lifetimes::first_triple(0, 0, (value_t) fls);
      res[1] = nullptr;
      return res;
    }

    virtaddr_t giveth (const index_t length, const triples::flags::flag_holder_t fls) {

      virtaddr_t res = static_cast<virtaddr_t> alloc(triple_atom_t*, length);

      res[0] = triples::lifetimes::first_triple(0, 0, (value_t) fls);
      for (size_t i = 1; i < length - 1; i++) {
        // for 0s as arguments, the result is the same between make_triple candidates
        res[i] = triples::lifetimes::giveth(0, 0, 0);
      }
      res[ length - 1 ] = nullptr;
      return res;
    }

    virtaddr_t giveth (const value_t* source, const index_t length, const triples::flags::flag_holder_t fls) {
      (void) fls;
      (void) length;
      (void) source;
      return {};
    }

    // and the KERNEL hath taken away; blessed be the name of the KERNEL.
    /*
      every pointer passed to this function must have at least 1 free-able element
      i.e it must be a "valid" (even if zero length) virtaddr array
    */
    void taketh (const virtaddr_t vaddr) {
      const index_t count_triples = attributes::metadata::deducing::real_length(vaddr);

      if (count_triples > 0) {
        for (size_t i = 1; i < count_triples; i++) {
          std::free( vaddr[i] );
        }
      }
      std::free( vaddr[0] );
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