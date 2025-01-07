/*** Utility Types ***/
#ifndef _LIBBITS_H_
#define _LIBBITS_H_

#ifdef __cplusplus
extern "C" {
#endif /** cplusplus **/

#include "libutil.h"

/* For 32 bits */
#define WORD_BITS_SHIFT 5
/* For 64 bits
#define WORD_BITS_SHIFT 6 */

#define WORD_BITS (sizeof(uint32_t) * 8)
#define WORD_BITS_MASK (WORD_BITS - 1)
#define WORD_MULT(bit)  ((bit + (WORD_BITS_MASK)) & ~(WORD_BITS_MASK))
#define WORD_TEST(bword, bit) ((bword >> bit) & 1)
#define WORD_COUNT(bit) (WORD_MULT(bit) >> WORD_BITS_SHIFT)


typedef struct {
  size_t  length;
  uint32_t *bits;
} bitvector_t;

bitvector_t *new_bitvector (size_t len);

void delete_bitvector (bitvector_t *bv);

void set_bit (bitvector_t *bv, size_t bit);

void unset_bit (bitvector_t *bv, size_t bit);

int is_bit_set (bitvector_t *bv, size_t bit);

#ifdef __cplusplus
}
#endif /** cplusplus **/

#endif /** _LIBBITS_H_ **/

