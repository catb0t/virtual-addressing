// creating new (empty-ish) vaddrs
#include "../virtaddr.h"

virtaddr_t lifetimes_ctor_blank (const bool use_range_notation) {
  const triple_atom_t stack_data[TRIPLE_LENGTH] = {0, 0, use_range_notation};

  virtaddr_t res = alloc(triple_atom_t*, 1);
  res[0] = memcpy(alloc(triple_atom_t, TRIPLE_LENGTH), stack_data, nbytes(value_t, TRIPLE_LENGTH));
  res[1] = NULL;
  return res;
}

triple_t lifetimes_make_first_triple (const value_t count_triples, const value_t virtual_length, const bool use_range_notation) {
  const triple_atom_t stack_data[ TRIPLE_LENGTH ] = {count_triples, virtual_length, (value_t) use_range_notation};

  return
    (triple_t)
    memcpy(alloc(value_t, TRIPLE_LENGTH), stack_data, nbytes(value_t, TRIPLE_LENGTH));
}
