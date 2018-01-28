#ifndef VA_VIRTADDR_H
#define VA_VIRTADDR_H

#include <cstdint>
#include <cstdarg>
#include <cstddef>
#include <cstdlib>
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

  /*struct virtaddr_t {
    raw_virtaddr_t data;

    virtaddr_t (const bool use_range_notation);
    virtaddr_t (const virtual_addressing::index_t length, const bool use_range_notation);
    virtaddr_t (const virtual_addressing::value_t* source, const virtual_addressing::index_t length, const bool use_range_notation);

  };*/

  namespace lifetimes {

    namespace triple_building {
      triple_t first_triple (const value_t count_triples, const value_t virtual_length, const bool range_notation);

      triple_t triple (const value_t value, const value_t zeroes);
      triple_t triple (const value_t value, const index_t bottom, const index_t top);
    }

    namespace copying {
      virtaddr_t copy (const virtaddr_t va);
    }

    virtaddr_t giveth (const bool use_range_notation);
    virtaddr_t giveth (const index_t length, const bool use_range_notation);
    virtaddr_t giveth (const value_t* source, const index_t length, const bool use_range_notation);

    void taketh (const virtaddr_t vaddr);
    void taketh (const virtaddr_t* vaddr, const index_t length);
    void taketh (const index_t argc, const virtaddr_t n_vaddr, ...);
  }

  namespace ctypes {
    namespace in {}
    namespace out {}
  }

  namespace locations {
    namespace searching {
      index_t linear_search (const virtaddr_t, const value_t value);
      index_t binary_search (const virtaddr_t, const value_t value);
    }
    namespace indexing {
      value_t    get (const virtaddr_t, const index_t index);
      virtaddr_t set (const virtaddr_t, const index_t index, const value_t value);

      namespace implementation {
        value_t get_linear_search (const virtaddr_t va, const index_t);
      }
    }
  }

  namespace mutations {
    namespace modifying {}
    namespace transforming {}
  }

  namespace attributes {
    namespace metadata {
      namespace getting {
        bool is_range_notation (const virtaddr_t);
        index_t virtual_length (const virtaddr_t);
        index_t    real_length (const virtaddr_t);
      }

      namespace setting {
        virtaddr_t is_range_notation (const virtaddr_t, const bool);
        virtaddr_t    virtual_length (const virtaddr_t, const value_t);
        virtaddr_t       real_length (const virtaddr_t, const value_t);
      }


      namespace deducing {
        bool is_range_notation (const virtaddr_t);
        index_t virtual_length (const virtaddr_t);
        index_t    real_length (const virtaddr_t);
      }

    }

    namespace ranges {
      virtaddr_t to_range_notation (const virtaddr_t);
      virtaddr_t to_count_notation (const virtaddr_t);
    }
  }

  namespace visualisations {
    namespace in {}
    namespace out {}
  }

}

#endif /* end of include guard: VA_VIRTADDR_H */
