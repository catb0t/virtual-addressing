// metadata and properties of vaddr arrays
#include "../virtaddr.h"

size_t       attributes_len_triples (const virtaddr_t va) {
  return va[0][0];
}
size_t       attributes_len_virtual (const virtaddr_t va) {
  return va[0][1];
}
