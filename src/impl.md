* **lifetimes** controlling lifetimes of vaddrs
  * **construct** creating new (empty-ish) vaddrs
  * **destruct** demolishing vaddr arrays
* **ctypes** raw C pointers to and from virtual addressing
  * **in** transforming C types to vaddrs
  * **out** transforming vaddrs to C types
* **locations** indexing and searching
  * **searching** get index, given value constraints
  * **indexing** get value, given index
* **mutations**
  * **modifying** by-value inserting, deleting, extending, changing
  * **transforming** sorting, reversing, mapping, filtering, reducing
* **attributes** convert / interpret properties of vaddrs
  * **ranges** conversions to / from range notation
  * **metadata** get/set metadata, flags etc
* **visualisations** string representations of vaddrs
  * **in** creating vaddrs from string / human data
  * **out** displaying vaddrs as human-readable

a vaddr (virtual address object) is a two-dimensional array. its elements are "triples", which are 2 or 3-element arrays of `uint64_t`.

when F (flags) has the lowest bit set, FEATURE_SLICE (explicit slicing notation) is not enabled.

the structure in this case is
```
A = [
  [L S F] - L: number of triples following S: spoofing virtual length F: flags
  [V Z X] - V: a non-zero value Z: the number of zeroes preceding V X: unused
  [V Z X] - ...
  ...
  NULL  - a null terminator, for simpler iteration.
]
```

this "counting" notation is the default. Z counts the real number of zeroes which follow the value V of the previous data triple, but which precede the value V in the current data triple.

in order to look up a (zero) value by virtual index, you must sum the number of virtual elements represented by all previous triples.  

when F (flags) has the lowest bit set, FEATURE_SLICE (explicit slicing notation) is enabled.

the structure in this case is
```
A = [
  [L S F] - L: number of triples following S: spoofing virtual length F: flags
  [V B T] - V: a non-zero value B: the bottom inclusive index of zero range T: the top exclusive index of zero range
  [V B T] - ...
  ...
  NULL  - a null terminator, for simpler iteration.
]
```
B describes the lower (bottom) inclusive bound of the slice. T describes the upper (top) exclusive bound of the slice.
The index named by B, and all indicies after it which are strictly less than T, are represented by the slice B T.
```
  0 0  = length: 0  indicies with zeroes: (none)
  0 1  = length: 1  indicies with zeroes: 0
  0 2  = length: 2  indicies with zeroes: 0, 1
  8 20 = length: 12 indicies with zeroes: 8, ..., 19

  8 2  = invalid slice, interepreted as count notation
```  


on x86-64, the memory used by either structure for N non-zero values is

```
( 8 * 3 * (N + 1) ) + 8
 (1) (2)      (3)    (4)
```
1. `sizeof (uint64_t)`
2. the length of each triple
3. the initial metadata triple element (containing `[L S R]`)
4. an extra pointer element for `NULL`

for 2 non-zero values, the result is 80 bytes.
2 values for 80 bytes seems rich but here if there are more than 10 zeroes around / between the non-zero values, then memory is saved.

if there were 2 000 non-zero values the memory used would be 48 032 bytes but if there are more than 6 004 zeroes around the non-zero elements, then memory will be saved.

the case in which there are many "empty" runs of zeroes and a non-zero value once in a while is exactly what is found in implementing hashtables through raw C arrays.
