// metadata and properties of vaddr arrays
#include "../virtaddr.hpp"

/*
  the project namespace
*/
namespace virtual_addressing {
  /*
    the file's namespace
  */
  namespace attributes {

    /*
      get, set and calculate metadata and attributes and flags
    */
    namespace metadata {

      /*
        get just believes what the array claims, instead of counting
      */
      namespace getting {

        /*
          get all the flags in the virtaddr
        */
        triple_flags::flag_holder_t flags (const virtaddr_t va) {
          return (triple_flags::flag_holder_t) va[0][2];
        }

        /*
          get 1 flag from the virtaddr
        */
        bool flag (const virtaddr_t va, triple_flags::flag_each_t fl) {
          return (bool) true & (getting::flags(va) >> fl);
        }

        /*
          function to get the notation flag

          returns true for slice, false for count
        */
        bool notation_flag (const virtaddr_t va) {
          return getting::flag(va, triple_flags::flag_each_t::FLAG_NOTATION);
        }

        /*
          the number of data triples in a vaddr array; also the number of non-zero data elements.

          the first_triple (metadata triple) is not counted, nor is the trailing nullptr / NULL.
        */
        index_t real_length (const virtaddr_t va)    { return va[0][0]; }

        /*
          the number of virtual elements this vaddr array represents; in other words the "real emulated size".

          this is equivalent to the number of data elements (the number of triples) plus the number of zeroes given by each triple.
        */
        index_t virtual_length (const virtaddr_t va) { return va[0][1]; }

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

        virtaddr_t notation_flag (const virtaddr_t va, const bool state) {
          virtaddr_t copy = virtual_addressing::lifetimes::copying::copy(va);

          copy[0][2] = state;
          return copy;
        }
      }

      namespace deducing {
        triple_flags::flag_holder_t flags (const virtaddr_t, const bool /* = false */) {
          return 0;
        }

        bool flag (const virtaddr_t, triple_flags::flag_each_t, const bool /* = false */) {
          return 0;
        }


        bool notation_flag (const virtaddr_t va, const bool force_calc /* = false */) {
          if (!force_calc && getting::real_length(va) < 1) {
            return getting::notation_flag(va);
          }
          // top > bottom for valid slice notation
          return (bool) (va[1][2] > va[1][1]);
        }

        index_t virtual_length (const virtaddr_t va, const bool force_calc /* = false */) {
          if (!force_calc && getting::real_length(va) < 1) {
            return getting::virtual_length(va);
          }

          const index_t count_triples = deducing::real_length(va);

          value_t sum = count_triples;

          if ( getting::notation_flag(va) ) {
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

    namespace slices {
      virtaddr_t to_slice_notation (const virtaddr_t, const bool /* = false */) { return {}; }
      virtaddr_t to_count_notation (const virtaddr_t, const bool /* = false */) { return {}; }
    }
  }
}