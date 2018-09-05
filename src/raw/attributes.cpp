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
        triples::flags::flag_holder_t flags (const virtaddr_t va) {
          return
            (triples::flags::flag_holder_t)
            triples::attributes::metadata::getting::third(va[0]);
        }

        /*
          get 1 flag from the virtaddr
        */
        bool flag (const virtaddr_t va, triples::flags::flag_each_t fl) {
          return triples::flags::has( getting::flags(va), fl );
        }

        /*
          function to get the notation flag

          returns true for slice, false for count
        */
        bool notation_flag (const virtaddr_t va) {
          return getting::flag(va, triples::flags::flag_each_t::FLAG_NOTATION);
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
          virtaddr_t copy = lifetimes::copy(va); // 1

          copy[0][0] = new_real_length;
          return copy;
        }

        virtaddr_t virtual_length (const virtaddr_t va, const value_t new_virtual_length) {
          virtaddr_t copy = lifetimes::copy(va); // 1

          copy[0][1] = new_virtual_length;
          return copy;
        }

        virtaddr_t notation_flag (const virtaddr_t va, const bool state) {
          virtaddr_t copy = lifetimes::copy(va); // 1

          copy[0][2] = state;
          return copy;
        }
      }

      namespace deducing {
        triples::flags::flag_holder_t flags (const virtaddr_t va, const bool force_calc /* = false */) {

          #ifdef VIRTADDR_GOFAST
            /* go fast */
            if (!force_calc) {
              return getting::flags(va);
            }
          #endif
          (void) force_calc;

          namespace tf = triples::flags;

          // ?
          (void) va;

          return {};
        }

        bool flag (const virtaddr_t va, triples::flags::flag_each_t test_for, const bool force_calc /* = false */) {

          #ifdef VIRTADDR_GOFAST
            /* go fast */
            if (!force_calc) {
              return getting::flag(va, test_for);
            }
          #endif
          (void) force_calc;

          switch ( test_for ) {
            case triples::flags::FLAG_NOTATION: {
              return notation_flag(va);
              break;
            }
            case triples::flags::FLAG_LAST_FLAG: {
              // best diagnostic currently
              //throw new triples::flags::flag_each_t;
              return false;
              break;
            }
            default: {
              // fallthrough
            }
          }
          return false;
        }

        bool notation_flag (const virtaddr_t va, const bool force_calc /* = false */) {
          #ifdef VIRTADDR_GOFAST
            /*
              we are not being forced to calculate it and it can't be deduced
              this is actually minorly nonsensical, we'll work on it
            */
            if (!force_calc /*&& getting::real_length(va) < 1*/) {
              return getting::notation_flag(va);
            }
          #endif
          (void) force_calc;

          const auto va_len = metadata::
            #ifdef VIRTADDR_GOFAST
              getting
            #else
              deducing
            #endif
          ::real_length(va);

          // initial value doesn't matter
          bool is_slice = true;
          // top > bottom for valid slice notation

          for (size_t i = 1; i < va_len; i++) {
            is_slice = triples::attributes::metadata::deducing::notation_guess(va[i]);
          }
          return is_slice;
        }

        index_t virtual_length (const virtaddr_t va, const bool force_calc /* = false */) {
          #ifndef VIRTADDR_GOFAST
            if (!force_calc && getting::real_length(va) < 1) {
              return getting::notation_flag(va);
            }
          #else
            (void) force_calc;
          #endif

          const index_t count_triples = deducing::real_length(va);

          value_t sum = count_triples;

          if ( getting::notation_flag(va) ) {
            for (size_t i = 1; i < count_triples; i++) {
              sum += triples::attributes::metadata::getting::first(va[i]);
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
          // don't add 1 because we are not counting triple #0
          while (nullptr != va[ len ]) {
            len++;
          }
          return len;
        }
      }
    }

    namespace slices {
      /*
        the input will (hopefully) have its notation flag unset (= count)
      */
      virtaddr_t to_slice_notation (const virtaddr_t va, const bool no_check /* = false */) {
        /*
          if we are checking and the flag is set, return a copy unless we are going fast
        */
        #ifndef VIRTADDR_GOFAST
          if (!no_check && true == metadata::getting::notation_flag(va)) {
            return lifetimes::copy(va); // 1, ~1
          }
        #else
          (void) no_check;
        #endif

        const auto va_new = lifetimes::copy(va); // 1

        /*
          when we are not going fast we can calculate the length
          when we are going fast we will just believe it blindly
        */
        const auto va_len = metadata::
          #ifdef VIRTADDR_GOFAST
            getting
          #else
            deducing
          #endif
        ::real_length(va);

        for (size_t i = 1; i < va_len; i++) {
          const auto last =
            1 == i        // this semaphore allocates nothing, you see
            ? nullptr     // semaphore to tell to_slice_notation that this is the first triple
            : va[i - 1];  // else use preceding triple
          va_new[i] = triples::attributes::slices::to_slice_notation(last, va[i]);
        }

        assert(va_new[va_len] == nullptr);

        return va_new; // ~1
      }
      /*
        the input will hopefully have its notation flag unset (= slice)
      */
      virtaddr_t to_count_notation (const virtaddr_t va, const bool no_check /* = false */) {
        /*
          if we are checking and the flag is unset, return a copy unless we are going fast
        */
        #ifndef VIRTADDR_GOFAST
          if (!no_check && false == metadata::getting::notation_flag(va)) {
            return lifetimes::copy(va);
          }
        #else
          (void) no_check;
        #endif

        const auto va_new = lifetimes::copy(va); // 1

        const auto va_len = metadata::
          #ifdef VIRTADDR_GOFAST
            getting
          #else
            deducing
          #endif
          ::real_length(va);

        for (size_t i = 1; i < va_len; i++) {
          va_new[i] = triples::attributes::slices::to_count_notation(va[i] /* give flag here? */);
        }

        // nothing to be done here -- as long as this passes
        assert(va_new[va_len] == nullptr);

        return va_new;
      }
    }
  }
}