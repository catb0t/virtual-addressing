#ifndef VA_VIRTADDR_H
#define VA_VIRTADDR_H

#include <cstdint>
#include <cstdarg>
#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <cstring>

#ifndef alloc
  #define alloc(type, size) (std::malloc(( nbytes(type, size) )))
#endif

#ifndef set_out_param
  #define set_out_param(name, value) do { if ( NULL != (name) ){ *name = value; } } while(0)
#endif

#ifndef nbytes
  #define nbytes(type, size) ((sizeof (type)) * (size))
#endif

#define TRIPLE_LENGTH 3

namespace virtual_addressing {
  typedef std::uint64_t value_t;
  typedef std::size_t   index_t;

  //const virtual_addressing::index_t triple_length = 3;

  // just a more useful name
  typedef value_t triple_atom_t;

  // a triple is just 3 atom_ts
  typedef triple_atom_t* triple_t;

  /*
    a vaddr array is a NULL-terminated list of triples
    remember, this is of type triple_t* aka uint64_t** !!

    equivalent to triple_t*
  */
  typedef triple_atom_t** virtaddr_t;

  namespace debugging {
    void say (const char* const,...);
    void see (const char* const, const triple_t);
    void see (const char* const, const virtaddr_t);
  }

  namespace triples {
    namespace flags {
      typedef enum /* en_flag_each_t */ {
        /*
          set = slice; unset = count

          whether this vaddr array is using slice or count notation.

          see file `impl.md`.

          in the case of count notation (the default),
            the first element of each triple is a non-zero value.
            the second element of each triple is a maybe-zero number of zeroes which virtually precede the first element.
            the third and final element of each triple is ignored (it may be zero or junk).

          in the case of slice notation,
            the first element of each triple is a non-zero value. (as above)
            the second element of each triple is the inclusive lower bound of the zero slice which virtually precedes the value.
            the and final third element of each triple is the exclusive upper bound of the zero slice.

        */
        FLAG_NOTATION = 0,

        /*
          the terminating flag in the enum; not used / usable as a real value
        */
        FLAG_LAST_FLAG
      } flag_each_t;

      typedef value_t flag_holder_t;

      flag_holder_t holder (void);
      flag_holder_t holder (const flag_each_t* fl);

      bool has (const flag_holder_t, const flag_each_t);
    }

    namespace attributes {

      namespace metadata {
        namespace getting {
          triple_atom_t  first (const triple_t);
          triple_atom_t second (const triple_t);
          triple_atom_t  third (const triple_t);
        }

        namespace setting {
          triple_t  first (const triple_atom_t);
          triple_t second (const triple_atom_t);
          triple_t  third (const triple_atom_t);

        }

        namespace deducing {
          triple_atom_t third (const triple_t);
        }
      }

      namespace slices {
        virtaddr_t to_slice_notation (const virtaddr_t, const bool = false);
        virtaddr_t to_count_notation (const virtaddr_t, const bool = false);
      }
    }

    namespace lifetimes {
      triple_t first_triple (const value_t, const value_t, const triples::flags::flag_holder_t);

      triple_t giveth (void);
      triple_t giveth (const value_t, const value_t);
      triple_t giveth (const value_t, const value_t, const value_t);

      triple_t copy (const triple_t);
      triple_t copy (const triple_atom_t* const);

      void taketh (const triple_t);
      void taketh (triple_t* const, const index_t);
      void taketh (const index_t, const triple_t, ...);
    }
    namespace mutations {}
  }

  namespace attributes {
    namespace metadata {
      namespace getting {
        triples::flags::flag_holder_t flags (const virtaddr_t);
        bool                         flag (const virtaddr_t, triples::flags::flag_each_t);

        bool notation_flag (const virtaddr_t);

        index_t virtual_length (const virtaddr_t);
        index_t    real_length (const virtaddr_t);
      }

      namespace setting {
        virtaddr_t flags (const virtaddr_t, triples::flags::flag_holder_t);
        virtaddr_t  flag (const virtaddr_t, triples::flags::flag_each_t, triples::flags::flag_each_t);

        virtaddr_t notation_flag (const virtaddr_t, const bool);

        virtaddr_t virtual_length (const virtaddr_t, const value_t);
        virtaddr_t    real_length (const virtaddr_t, const value_t);
      }


      namespace deducing {
        triples::flags::flag_holder_t flags (const virtaddr_t, const bool = false);
        bool                         flag (const virtaddr_t, triples::flags::flag_each_t, const bool = false);

        bool notation_flag (const virtaddr_t, const bool = false);

        index_t virtual_length (const virtaddr_t, const bool = false);
        index_t    real_length (const virtaddr_t);
      }

    }

    namespace slices {
      virtaddr_t to_slice_notation (const virtaddr_t, const bool = false);
      virtaddr_t to_count_notation (const virtaddr_t, const bool = false);
    }
  }

  namespace ctypes {
    namespace in {}
    namespace out {}
  }

  namespace lifetimes {
    virtaddr_t giveth (void);
    virtaddr_t giveth (const triples::flags::flag_holder_t);
    virtaddr_t giveth (const index_t , const triples::flags::flag_holder_t);
    virtaddr_t giveth (const value_t*, const index_t, const triples::flags::flag_holder_t);

    virtaddr_t copy (const virtaddr_t);

    void taketh (const virtaddr_t);
    void taketh (const virtaddr_t*, const index_t);
    void taketh (const index_t, const virtaddr_t, ...);
  }

  namespace locations {
    namespace searching {
      index_t linear_search (const virtaddr_t, const value_t);
      index_t binary_search (const virtaddr_t, const value_t);
    }
    namespace indexing {
      value_t    get (const virtaddr_t, const index_t);
      virtaddr_t set (const virtaddr_t, const index_t, const value_t);

      namespace implementation {
        value_t get_linear_search (const virtaddr_t, const index_t);
      }
    }
  }

  namespace mutations {
    namespace modifying {}
    namespace transforming {}
  }

  namespace visualisations {
    namespace in {}
    namespace out {
      char* repr (const virtaddr_t);
    }
  }

}

#endif /* end of include guard: VA_VIRTADDR_H */
