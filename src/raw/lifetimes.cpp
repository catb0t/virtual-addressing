// creating / deleting vaddrs
#include "../virtaddr.hpp"

namespace virtual_addressing {
  namespace lifetimes {
    virtaddr_t giveth (const bool use_range_notation) {
      (void) use_range_notation;
      return {};
    }
    virtaddr_t giveth (const virtual_addressing::index_t length, const bool use_range_notation) {
      (void) use_range_notation;
      (void) length;
      return {};
    }
    virtaddr_t giveth (const virtual_addressing::value_t* source, const virtual_addressing::index_t length, const bool use_range_notation) {
      (void) use_range_notation;
      (void) length;
      (void) source;
      return {};
    }

    void taketh (virtaddr_t vaddr) {
      (void) vaddr;
    }
    void taketh (virtaddr_t vaddr...) {
      (void) vaddr;
    }
  }
}