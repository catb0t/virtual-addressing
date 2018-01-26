// finding values, given indicies
#include "../virtaddr.hpp"

namespace virtual_addressing {

  namespace locations {
    namespace searching {
      virtual_addressing::index_t linear_search (const virtual_addressing::virtaddr_t, const virtual_addressing::value_t value) {
        (void) value;
        return 0;
      }
      virtual_addressing::index_t binary_search (const virtual_addressing::virtaddr_t, const virtual_addressing::value_t value) {
        (void) value;
        return 0;
      }
    }
    namespace indexing {
      virtual_addressing::value_t    get (const virtual_addressing::virtaddr_t, const virtual_addressing::index_t index) {
        (void) index;
        return 0;
      }
      virtual_addressing::virtaddr_t set (const virtual_addressing::virtaddr_t, const virtual_addressing::index_t index, const virtual_addressing::value_t value) {
        (void) index;
        (void) value;
        return {};
      }
    }
  }

}
