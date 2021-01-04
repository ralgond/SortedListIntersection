#include <immintrin.h>

#include <stdint.h>
#include <cassert>

#include "algo_intersection.h"

#ifdef __GNUC__
#define COMPILER_LIKELY(x) __builtin_expect((x), 1)
#define COMPILER_RARELY(x) __builtin_expect((x), 0)
#else
#define COMPILER_LIKELY(x) x
#define COMPILER_RARELY(x) x
#endif


size_t v3_avx2_intersection(const uint32_t *freq, const size_t lenFreq, const uint32_t *rare, const size_t lenRare, uint32_t *out) {
  if (lenFreq == 0 || lenRare == 0)
    return 0;
  assert(lenRare <= lenFreq);
  const uint32_t *const initout(out);
  typedef __m128i vec;
  const uint32_t veclen = sizeof(vec) / sizeof(uint32_t);
  const size_t vecmax = veclen - 1;
  const size_t freqspace = 32 * veclen;
  const size_t rarespace = 1;

  const uint32_t *stopFreq = freq + lenFreq - freqspace;
  const uint32_t *stopRare = rare + lenRare - rarespace;
  if (freq > stopFreq) {
    return scalar(freq, lenFreq, rare, lenRare, out);
  }
  while (freq[veclen * 31 + vecmax] < *rare) {
    freq += veclen * 32;
    if (freq > stopFreq)
      goto FINISH_SCALAR;
  }
  for (; rare < stopRare; ++rare) {
    const uint32_t matchRare = *rare; // nextRare;
    const vec Match = _mm_set1_epi32(matchRare);
    while (freq[veclen * 31 + vecmax] < matchRare) { // if no match possible
      freq += veclen * 32;                           // advance 32 vectors
      if (freq > stopFreq)
        goto FINISH_SCALAR;
    }
    vec Q0, Q1, Q2, Q3;
    if (freq[veclen * 15 + vecmax] >= matchRare) {
      if (freq[veclen * 7 + vecmax] < matchRare) {
        Q0 = _mm_or_si128(
            _mm_cmpeq_epi32(
                _mm_loadu_si128(reinterpret_cast<const vec *>(freq) + 8),
                Match),
            _mm_cmpeq_epi32(
                _mm_loadu_si128(reinterpret_cast<const vec *>(freq) + 9),
                Match));
        Q1 = _mm_or_si128(
            _mm_cmpeq_epi32(
                _mm_loadu_si128(reinterpret_cast<const vec *>(freq) + 10),
                Match),
            _mm_cmpeq_epi32(
                _mm_loadu_si128(reinterpret_cast<const vec *>(freq) + 11),
                Match));

        Q2 = _mm_or_si128(
            _mm_cmpeq_epi32(
                _mm_loadu_si128(reinterpret_cast<const vec *>(freq) + 12),
                Match),
            _mm_cmpeq_epi32(
                _mm_loadu_si128(reinterpret_cast<const vec *>(freq) + 13),
                Match));
        Q3 = _mm_or_si128(
            _mm_cmpeq_epi32(
                _mm_loadu_si128(reinterpret_cast<const vec *>(freq) + 14),
                Match),
            _mm_cmpeq_epi32(
                _mm_loadu_si128(reinterpret_cast<const vec *>(freq) + 15),
                Match));
      } else {
        Q0 = _mm_or_si128(
            _mm_cmpeq_epi32(
                _mm_loadu_si128(reinterpret_cast<const vec *>(freq) + 4),
                Match),
            _mm_cmpeq_epi32(
                _mm_loadu_si128(reinterpret_cast<const vec *>(freq) + 5),
                Match));
        Q1 = _mm_or_si128(
            _mm_cmpeq_epi32(
                _mm_loadu_si128(reinterpret_cast<const vec *>(freq) + 6),
                Match),
            _mm_cmpeq_epi32(
                _mm_loadu_si128(reinterpret_cast<const vec *>(freq) + 7),
                Match));
        Q2 = _mm_or_si128(
            _mm_cmpeq_epi32(
                _mm_loadu_si128(reinterpret_cast<const vec *>(freq) + 0),
                Match),
            _mm_cmpeq_epi32(
                _mm_loadu_si128(reinterpret_cast<const vec *>(freq) + 1),
                Match));
        Q3 = _mm_or_si128(
            _mm_cmpeq_epi32(
                _mm_loadu_si128(reinterpret_cast<const vec *>(freq) + 2),
                Match),
            _mm_cmpeq_epi32(
                _mm_loadu_si128(reinterpret_cast<const vec *>(freq) + 3),
                Match));
      }
    } else {
      if (freq[veclen * 23 + vecmax] < matchRare) {
        Q0 = _mm_or_si128(
            _mm_cmpeq_epi32(
                _mm_loadu_si128(reinterpret_cast<const vec *>(freq) + 8 + 16),
                Match),
            _mm_cmpeq_epi32(
                _mm_loadu_si128(reinterpret_cast<const vec *>(freq) + 9 + 16),
                Match));
        Q1 = _mm_or_si128(
            _mm_cmpeq_epi32(
                _mm_loadu_si128(reinterpret_cast<const vec *>(freq) + 10 + 16),
                Match),
            _mm_cmpeq_epi32(
                _mm_loadu_si128(reinterpret_cast<const vec *>(freq) + 11 + 16),
                Match));

        Q2 = _mm_or_si128(
            _mm_cmpeq_epi32(
                _mm_loadu_si128(reinterpret_cast<const vec *>(freq) + 12 + 16),
                Match),
            _mm_cmpeq_epi32(
                _mm_loadu_si128(reinterpret_cast<const vec *>(freq) + 13 + 16),
                Match));
        Q3 = _mm_or_si128(
            _mm_cmpeq_epi32(
                _mm_loadu_si128(reinterpret_cast<const vec *>(freq) + 14 + 16),
                Match),
            _mm_cmpeq_epi32(
                _mm_loadu_si128(reinterpret_cast<const vec *>(freq) + 15 + 16),
                Match));
      } else {
        Q0 = _mm_or_si128(
            _mm_cmpeq_epi32(
                _mm_loadu_si128(reinterpret_cast<const vec *>(freq) + 4 + 16),
                Match),
            _mm_cmpeq_epi32(
                _mm_loadu_si128(reinterpret_cast<const vec *>(freq) + 5 + 16),
                Match));
        Q1 = _mm_or_si128(
            _mm_cmpeq_epi32(
                _mm_loadu_si128(reinterpret_cast<const vec *>(freq) + 6 + 16),
                Match),
            _mm_cmpeq_epi32(
                _mm_loadu_si128(reinterpret_cast<const vec *>(freq) + 7 + 16),
                Match));
        Q2 = _mm_or_si128(
            _mm_cmpeq_epi32(
                _mm_loadu_si128(reinterpret_cast<const vec *>(freq) + 0 + 16),
                Match),
            _mm_cmpeq_epi32(
                _mm_loadu_si128(reinterpret_cast<const vec *>(freq) + 1 + 16),
                Match));
        Q3 = _mm_or_si128(
            _mm_cmpeq_epi32(
                _mm_loadu_si128(reinterpret_cast<const vec *>(freq) + 2 + 16),
                Match),
            _mm_cmpeq_epi32(
                _mm_loadu_si128(reinterpret_cast<const vec *>(freq) + 3 + 16),
                Match));
      }
    }
    const vec F0 = _mm_or_si128(_mm_or_si128(Q0, Q1), _mm_or_si128(Q2, Q3));
#ifdef __SSE4_1__
    if (_mm_testz_si128(F0, F0)) {
#else
    if (!_mm_movemask_epi8(F0)) {
#endif
    } else {
      *out++ = matchRare;
    }
  }

FINISH_SCALAR:
  return (out - initout) + scalar(freq, stopFreq + freqspace - freq, rare,
                                  stopRare + rarespace - rare, out);
}
