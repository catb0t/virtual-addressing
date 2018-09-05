// working with three-tuples
#include "../virtaddr.hpp"

namespace virtual_addressing {
  namespace triples {
    namespace attributes {
      namespace metadata {
        namespace getting {
          triple_atom_t  first (const triple_t trp) { return trp[0]; }
          triple_atom_t second (const triple_t trp) { return trp[1]; }
          triple_atom_t  third (const triple_t trp) { return trp[2]; }
        }

        namespace setting {
          triple_t  first (const triple_atom_t atom) {
            return triples::lifetimes::giveth(atom, 0, 0);
          }
          triple_t second (const triple_atom_t atom) {
            return triples::lifetimes::giveth(0, atom, 0);
          }
          triple_t  third (const triple_atom_t atom) {
            return triples::lifetimes::giveth(0, 0, atom);
          }
        }

        namespace deducing {
          triple_atom_t third (const triple_t trp) {
            const triple_atom_t thrd = metadata::getting::third(trp);
            return
              metadata::getting::second(trp) <= thrd
              ? thrd
              : 0;
          }

          /*
            returns 1 for slice, 0 for count
          */
          bool notation_guess (const triple_t trp) {
            return metadata::getting::second(trp) <= triples::attributes::metadata::
                #ifdef VIRTADDR_GOFAST
                  getting
                #else
                  deducing
                #endif
              ::third(trp);
          }
        }
      }

      namespace slices {
        /*
          convert a count-format triple to slice notation

          if the first argument was nullptr / NULL, then the preceding triple was the initial
            (metadata) triple in the list

          preceding is expected to be in slice notation, so that we know where we are
        */
        triple_t to_slice_notation (const triple_t preceding, const triple_t trp, const bool no_check /* = false */) {
          namespace get = triples::attributes::metadata::getting;
          namespace ded = triples::attributes::metadata::deducing;

          /* slice notation is exclusive of the upper bound so count -= 1*/
          /* const */ value_t real_count, value;

          { // don't mess up my function scope, bro
            const auto
                val = value = get::first(trp),
              count = get::second(trp),
                top = get::third(trp); // need the raw value here, not deduced

              real_count = utilities::subtract_unsigned(count, 1);
            #ifndef VIRTADDR_GOFAST
              if (!no_check && top > 0) {
                // it wasn't in count notation, and we are not going fast
                // maybe this return value is inconsistent? TODO
                return triples::lifetimes::giveth(val, 0, real_count);
              }
            #endif
            (void) val;
            (void) no_check; // for GOFAST
            (void) top;
          }

          // NOTE: early return here
          if (nullptr == preceding) {
            return triples::lifetimes::giveth(value, 0, real_count);
          }

          // logic to do the meaty conversion follows
          const auto
            new_bot = 1 + ded::third(preceding), // intentional deduce here
            new_top = new_bot + real_count;

          return lifetimes::giveth(value, new_bot, new_top);
        }
        /*
          convert a slice-format triple to counting notation
        */
        triple_t to_count_notation (const triple_t trp, const bool no_check /* = false */) {

          namespace get = triples::attributes::metadata::getting;
          namespace ded = triples::attributes::metadata::deducing;

          const auto
            val = get::first(trp),
            bot = get::second(trp),
            top = ded::third(trp);  // intentionally deduce here

          // going to believe the caller, that top > bot
          if (no_check) {
            return triples::lifetimes::giveth(val, top - bot);
          // disbelieve the caller and check ourselves unless we are going fast
          } else {
            return triples::lifetimes::giveth(
              val,
            #ifdef VIRTADDR_GOFAST // this is what happens when you try go fast
              top - bot
            #else
              utilities::subtract_unsigned(top, bot)
            #endif
            );
          }

        }
      }
    }

    namespace flags {
      /*
        this is a constructor!
      */
      flag_holder_t holder (void) {
        return flag_holder_t {};
      }

      // also a constructor! are you paying attention?
      flag_holder_t holder (const flag_each_t* fl) {

        flag_holder_t total = {};

        for (size_t i = 0; flag_each_t::FLAG_LAST_FLAG != fl[i] ; i++) {
          total |= flag_holder_t(1U << fl[i]);
        }

        return total;
      }

      // if the haystack shifted right by needle bits shares a bit with 1, then the flag exists
      bool has (const flag_holder_t haystack, const flag_each_t needle) {
        return (bool) (1U & (haystack >> needle));
      }

    }

    namespace lifetimes {
      static triple_t _impl_copy (const void* const triple);

      /*
        the leading triple in any virtaddr_t
        the arguments will be stored in the same order as they are given here
      */
      triple_t first_triple (const value_t count_triples, const value_t virtual_length, const triples::flags::flag_holder_t fls) {
        const triple_atom_t stack_data[ TRIPLE_LENGTH ] = {
          count_triples,
          virtual_length,
          value_t(fls)
        };

        return triples::lifetimes::copy(stack_data);
      }

      /*
        make any triple
      */
      triple_t giveth (void) {
        return triples::lifetimes::giveth(0, 0);
      }

      triple_t giveth (const value_t value, const value_t zeroes) {
        const triple_atom_t stack_data[ TRIPLE_LENGTH ] = {value, zeroes, 0 };
        return triples::lifetimes::copy(stack_data);
      }

      triple_t giveth (const value_t value, const index_t bottom, const index_t top) {
        const triple_atom_t stack_data[ TRIPLE_LENGTH ] = {value, bottom, top};
        return triples::lifetimes::copy(stack_data);
      }

      triple_t copy (const triple_t triple) {
        return triples::lifetimes::_impl_copy(triple);
      }

      triple_t copy (const triple_atom_t* const triple) {
        return triples::lifetimes::_impl_copy(triple);
      }

      void taketh (const triple_t trp) {
        std::free(trp);
      }
      void taketh (triple_t* const trps, const index_t length) {
        for (size_t i = 0; i < length; i++) {
          taketh(trps[i]);
        }
        std::free(trps);
      }
      void taketh (const index_t argc, const triple_t trp, ...) {
        va_list va;
        va_start(va, trp);

        for (size_t i = 0; i < argc; i++) {
          auto t = static_cast<triple_t> (va_arg(va, triple_t));
          triples::lifetimes::taketh(t);
        }
        va_end(va);
      }

      /* STATIC LOCAL IMPLEMENTATIONS FOLLOW */

      static triple_t _impl_copy (const void* const triple) {
        return (triple_t)
        std::memcpy(
          alloc(triple_atom_t, TRIPLE_LENGTH),
          triple,
          nbytes(triple_atom_t, TRIPLE_LENGTH)
        );
      }
    }
  }
}
