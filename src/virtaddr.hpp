#ifndef VA_VIRTADDR_H
#define VA_VIRTADDR_H

#include <cstdint>

#ifndef alloc
  #define alloc(type, size) malloc((sizeof (type)) * (size))
#endif

#ifndef set_out_param
  #define set_out_param(name, value) do { if ( NULL != (name) ){ *name = value; } } while(0)
#endif

namespace virtual_addressing {
  typedef std::uint64_t value_t;
  typedef std::size_t   index_t;

  // a triple is 2 or 3 uint64_ts
  typedef value_t* triple_t;

  /*
    a vaddr array is a NULL-terminated list of triples
    remember, this is of type triple_t* aka uint64_t** !!
  */
  typedef triple_t* virtaddr_t;

  /*struct virtaddr_t {
    raw_virtaddr_t data;

    virtaddr_t (const bool use_range_notation);
    virtaddr_t (const virtual_addressing::index_t length, const bool use_range_notation);
    virtaddr_t (const virtual_addressing::value_t* source, const virtual_addressing::index_t length, const bool use_range_notation);

  };*/

  namespace lifetimes {
    virtaddr_t make_new (const bool use_range_notation);
    virtaddr_t make_new (const virtual_addressing::index_t length, const bool use_range_notation);
    virtaddr_t make_new (const virtual_addressing::value_t* source, const virtual_addressing::index_t length, const bool use_range_notation);
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

  namespace attributes {
    namespace metadata {
                          bool    is_range_notation (const virtual_addressing::virtaddr_t);
      virtual_addressing::index_t    virtual_length (const virtual_addressing::virtaddr_t);
      virtual_addressing::index_t       real_length (const virtual_addressing::virtaddr_t);
    }
    namespace ranges {
      virtual_addressing::virtaddr_t to_range_notation (const virtual_addressing::virtaddr_t);
      virtual_addressing::virtaddr_t to_count_notation (const virtual_addressing::virtaddr_t);
    }
  }

}

#endif /* end of include guard: VA_VIRTADDR_H */
