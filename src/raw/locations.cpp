// finding values, given indicies
#include "../virtaddr.hpp"

namespace virtual_addressing {

  namespace locations {
    namespace searching {
      index_t linear_search (const virtaddr_t, const value_t value) {
        (void) value;
        return 0;
      }
      index_t binary_search (const virtaddr_t, const value_t value) {
        (void) value;
        return 0;
      }
    }
    namespace indexing {
      value_t get (const virtaddr_t, const index_t index) {
        (void) index;
        return 0;
      }
      virtaddr_t set (const virtaddr_t, const index_t index, const value_t value) {
        (void) index;
        (void) value;
        return {};
      }

      namespace implementation {
        value_t get_linear_search (const virtaddr_t va, const index_t) {
/*          const size_t vlen = virtual_addressing::attributes::metadata::virtual_length(va);

          if (get_index > vlen) {
            errno = EINVAL;
            set_out_param(ok, false);
            return 0;
          }

          const size_t len = sparse64_len_pairs(sps), rlen = 2 * len;

          size_t* const zero_ranges = sparse64_uncompress_zero_ranges(sps);

          // if the index is within a zero range the value is zero
          for (size_t i = 0; i < rlen; i += 2) {
            if ( is_in_range_64(get_index, zero_ranges[i], zero_ranges[i + 1]) ) {
              return 0;
            }
          }

          free(zero_ranges);

          size_t* const elts = _elements_data_64(sps);

          for (size_t i = 0; i < rlen; i += 2) {
            if (get_index == elts[i]) {
              const size_t val = elts[i + 1];
              free(elts);
              return val;
            }
          }

          free(elts);

          // no entry
          errno = ENOENT;

          set_out_param(ok, false);

          return 0;
*/
          (void) va;
          return 0;
        }
      }
    }
  }

}
