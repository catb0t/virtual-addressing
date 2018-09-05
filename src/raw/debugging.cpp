#include "../virtaddr.hpp"

namespace virtual_addressing {
  namespace debugging {
    void say (const char* const format, ...) {
      #if defined(VIRTADDR_DEBUG) && !defined(VIRTADDR_GOFAST)
        va_list va;
        va_start(va, format);
        std::printf("[VIRTADDR_DEBUG] ");
        std::vprintf(format, va);
        va_end(va);
        std::printf("%s", "\n");
        std::fflush(stdout);
      #endif
      (void) format;
    }
  }
}
