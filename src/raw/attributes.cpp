// metadata and properties of vaddr arrays
#include "../virtaddr.hpp"

namespace virtual_addressing {
  namespace attributes {
    namespace metadata {
      bool    is_range_notation (const virtual_addressing::virtaddr_t va) {
        return (bool) va[0][2];
      }
      virtual_addressing::index_t virtual_length (const virtual_addressing::virtaddr_t va) {
        return va[0][1];
      }
      virtual_addressing::index_t real_length (const virtual_addressing::virtaddr_t va) {
        return va[0][0];
      }

      virtual_addressing::index_t virtual_length (const virtual_addressing::triple_atom_t* const * const va) {
        return va[0][1];
      }
      virtual_addressing::index_t real_length (const virtual_addressing::triple_atom_t* const * const va) {
        return va[0][0];
      }
    }
    namespace ranges {
      virtual_addressing::virtaddr_t to_range_notation (const virtual_addressing::virtaddr_t) {
        return {};
      }
      virtual_addressing::virtaddr_t to_count_notation (const virtual_addressing::virtaddr_t) {
        return {};
      }
    }
  }
}