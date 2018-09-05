#include "../virtaddr.hpp"

namespace virtual_addressing {
  namespace utilities {
    size_t subtract_unsigned (const size_t lhs, const size_t rhs) {
      if (0 == rhs || 0 == lhs || rhs > lhs) {
        return lhs;
        errno = EINVAL;
      }
      return lhs - rhs;
    }
    void say (const void* const, const size_t) {
    }
    void see (const void* const, const size_t* const) {
    }
  }
}