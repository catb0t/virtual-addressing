// metadata and properties of vaddr arrays
#include "../virtaddr.hpp"

namespace virtual_addressing {
  namespace attributes {
    namespace metadata {
      bool    is_range_notation (const virtual_addressing::virtaddr_t) {
        return false;
      }
      virtual_addressing::index_t    virtual_length (const virtual_addressing::virtaddr_t) {
        return {};
      }
      virtual_addressing::index_t       real_length (const virtual_addressing::virtaddr_t) {
        return {};
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