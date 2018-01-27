#ifndef VA_VIRTADDR_H
#define VA_VIRTADDR_H

#include <cstdint>
#include <cstdarg>
#include <cstddef>
#include <cstdlib>
#include <cstring>

#ifndef alloc
  #define alloc(type, size) (malloc(( nbytes(type, size) )))
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

    namespace triples {
      virtual_addressing::triple_t first_triple (const value_t count_triples, const value_t virtual_length, const bool range_notation);

      virtual_addressing::triple_t triple (const value_t value, const value_t zeroes);
      virtual_addressing::triple_t triple (const virtual_addressing::value_t value, const virtual_addressing::index_t bottom, const virtual_addressing::index_t top);
    }

    virtual_addressing::virtaddr_t giveth (const bool use_range_notation);
    virtual_addressing::virtaddr_t giveth (const virtual_addressing::index_t length, const bool use_range_notation);
    virtual_addressing::virtaddr_t giveth (const virtual_addressing::value_t* source, const virtual_addressing::index_t length, const bool use_range_notation);

    void taketh (const virtual_addressing::virtaddr_t vaddr);
    void taketh (const virtual_addressing::virtaddr_t* vaddr, const virtual_addressing::index_t length);
    void taketh (const virtual_addressing::index_t argc, const virtual_addressing::virtaddr_t n_vaddr, ...);
  }

  namespace ctypes {
    namespace in {}
    namespace out {}
  }

  namespace locations {
    namespace searching {
      virtual_addressing::index_t linear_search (const virtual_addressing::virtaddr_t, const virtual_addressing::value_t value);
      virtual_addressing::index_t binary_search (const virtual_addressing::virtaddr_t, const virtual_addressing::value_t value);
    }
    namespace indexing {
      virtual_addressing::value_t    get (const virtual_addressing::virtaddr_t, const virtual_addressing::index_t index);
      virtual_addressing::virtaddr_t set (const virtual_addressing::virtaddr_t, const virtual_addressing::index_t index, const virtual_addressing::value_t value);
    }
  }

  namespace mutations {
    namespace modifying {}
    namespace transforming {}
  }

  namespace attributes {
    namespace metadata {
      bool is_range_notation (const virtual_addressing::virtaddr_t);

      virtual_addressing::index_t virtual_length (const virtual_addressing::virtaddr_t);
      virtual_addressing::index_t virtual_length (const virtual_addressing::triple_atom_t* const * const va);

      virtual_addressing::index_t real_length (const virtual_addressing::virtaddr_t);
      virtual_addressing::index_t real_length (const virtual_addressing::triple_atom_t* const * const va);
    }

    namespace ranges {
      virtual_addressing::virtaddr_t to_range_notation (const virtual_addressing::virtaddr_t);
      virtual_addressing::virtaddr_t to_count_notation (const virtual_addressing::virtaddr_t);
    }
  }

  namespace visualisations {
    namespace in {}
    namespace out {}
  }

}

#endif /* end of include guard: VA_VIRTADDR_H */
