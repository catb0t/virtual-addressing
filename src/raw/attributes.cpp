// metadata and properties of vaddr arrays
#include "../virtaddr.hpp"

namespace virtual_addressing {
  namespace attributes {

    namespace metadata {

      namespace getting {
        index_t real_length (const virtaddr_t va)    { return va[0][0]; }

        index_t virtual_length (const virtaddr_t va) { return va[0][1]; }

        bool is_range_notation (const virtaddr_t va) { return (bool) va[0][2]; }
      }

      /* naive ! results must be freed */
      namespace setting {
        virtaddr_t real_length (const virtaddr_t va, const value_t new_real_length)    {
          virtaddr_t copy = virtual_addressing::lifetimes::copying::copy(va);

          copy[0][0] = new_real_length;
          return copy;
        }

        virtaddr_t virtual_length (const virtaddr_t va, const value_t new_virtual_length) {
          virtaddr_t copy = virtual_addressing::lifetimes::copying::copy(va);

          copy[0][1] = new_virtual_length;
          return copy;
        }

        virtaddr_t is_range_notation (const virtaddr_t va, const bool state) {
          virtaddr_t copy = virtual_addressing::lifetimes::copying::copy(va);

          copy[0][2] = state;
          return copy;
        }
      }

      namespace deducing {
        bool is_range_notation (const virtaddr_t va) {
          if (getting::real_length(va) < 1) {
            return getting::is_range_notation(va);
          }
          // top > bottom for valid range notation
          return (bool) (va[1][2] > va[1][1]);
        }

        index_t virtual_length (const virtaddr_t va) {
          if (getting::real_length(va) < 1) {
            return getting::virtual_length(va);
          }

          const index_t count_triples = deducing::real_length(va);

          value_t sum = count_triples;

          if ( getting::is_range_notation(va) ) {
            for (size_t i = 1; i < count_triples; i++) {
              sum += va[i][1];
            }
          } else {
            for (size_t i = 1; i < count_triples; i++) {
              sum += va[i][2] - va[i][1];
            }
          }

          return sum;
        }

        /*
          does not count leading first_triple nor trailing NULL!
        */
        index_t real_length (const virtaddr_t va) {
          index_t len = 0;

          // saves us adding 1 at the end
          while (nullptr != va[ len ]) {
            len++;
          }

          return len;
        }
      }
    }

    namespace ranges {
      virtaddr_t to_range_notation (const virtaddr_t) { return {}; }
      virtaddr_t to_count_notation (const virtaddr_t) { return {}; }
    }
  }
}