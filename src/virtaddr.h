#ifndef VA_VIRTADDR_H
#define VA_VIRTADDR_H

#include <inttypes.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef alloc
  #define alloc(type, size) malloc((sizeof (type)) * (size))
#endif

#ifndef set_out_param
  #define set_out_param(name, value) if ( NULL != (name) ){ *name = value; }
#endif

#ifndef I
  #define I(obj, mth, args, ...) obj->mth(obj, args, __VA_ARGS__)
#endif

// a triple is 2 or 3 uint64_ts
typedef uint64_t* triple_t;

/*
  a vaddr array is a NULL-terminated list of triples
  remember, this is of type triple_t* aka uint64_t** !!
*/
typedef triple_t* virtaddr_t;

/*
  controlling lifetimes of virtaddr_ts
*/
virtaddr_t lifetimes_ctor_blank (const bool);
virtaddr_t       lifetimes_ctor (const size_t, const bool);
virtaddr_t  lifetimes_ctor_from (const uint64_t* const, const size_t, const bool);
void             lifetimes_dtor (const virtaddr_t);
void           lifetimes_dtor_n (const size_t, const virtaddr_t, ...);

typedef struct st_lifetimes_ns lifetimes_ns;
typedef struct st_lifetimes_ns {
  virtaddr_t (*ctor_blank) (const bool);
  virtaddr_t       (*ctor) (const size_t, const bool);
  virtaddr_t  (*ctor_from) (const uint64_t * const, const size_t, const bool);

  void   (*dtor) (virtaddr_t const);
  void (*dtor_n) (const size_t len, virtaddr_t const, ...);
} lifetimes_ns;


/*
  reading and inferring metadata and attributes of virtaddr_ts
*/
size_t       attributes_len_triples (const virtaddr_t);
size_t       attributes_len_virtual (const virtaddr_t);
bool   attributes_is_range_notation (const virtaddr_t);

typedef struct st_attributes_ns {
  size_t       (*len_triples) (const virtaddr_t);
  size_t       (*len_virtual) (const virtaddr_t);
  bool   (*is_range_notation) (const virtaddr_t);
} attributes_ns;

/*
  locating values and indicies
*/

/* \/ \/ NON-STATIC FUNCTION STRUCT \/ \/ */

//typedef struct st_fat_virtaddr_t fat_virtaddr_t;
//
//typedef struct st_fat_virtaddr_t {
//
//  /* member structs */
//  attributes_ns attributes;
//  lifetimes_ns lifetimes;
//
//  /* member functions */
//  int (*identity) (fat_virtaddr_t*, int, ...);
//
//  /* data */
//  int x;
//  virtaddr_t data;
//
//} fat_virtaddr_t;

#endif /* end of include guard: VA_VIRTADDR_H */
