// creating new (empty-ish) vaddrs
#include "../virtaddr.h"

virtaddr_t lifetimes_ctor_blank (const bool use_range_notation) {
  const triple_atom_t stack_data[TRIPLE_LENGTH] = {0, 0, use_range_notation};

  virtaddr_t res = alloc(triple_atom_t*, 1);
  res[0] = memcpy(alloc(triple_atom_t, TRIPLE_LENGTH), stack_data, TRIPLE_LENGTH);
  res[1] = NULL;
  return res;
}
