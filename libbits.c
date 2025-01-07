#include "libbits.h"


/* create new bitvector */
bitvector_t *
new_bitvector (size_t len)
{
  bitvector_t *bv = safe_malloc (sizeof (bitvector_t));
  bv->length = len;
  bv->bits = safe_malloc (WORD_COUNT (len) * sizeof (uint32_t));
  return (bv);
}

/* delete new bitvector */
void
delete_bitvector (bitvector_t *bv)
{
  free (bv->bits);
  free (bv);
  return;
}

/* set bit */
void
set_bit (bitvector_t *bv, size_t bit)
{
  assert (bit < bv->length);
  bv->bits[(bit >> WORD_BITS_SHIFT)] |=
    ((uint32_t)1 << (bit & WORD_BITS_MASK));
  return;
}

/* unset bit */
void
unset_bit (bitvector_t *bv, size_t bit)
{
  assert(bit < bv->length);
  bv->bits[(bit >> WORD_BITS_SHIFT)] &=
    ~((uint32_t)1 << (bit & WORD_BITS_MASK));
  return;
}

/* is_bit_set bit */
int
is_bit_set (bitvector_t *bv, size_t bit)
{
  assert(bit < bv->length);
  uint32_t bword = bv->bits[(bit >> WORD_BITS_SHIFT)];
  return (WORD_TEST(bword, (bit & WORD_BITS_MASK)));
}

