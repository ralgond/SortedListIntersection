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


size_t v1_avx2_intersection(const uint32_t *freq, size_t lenFreq, const uint32_t *rare, size_t lenRare, uint32_t *matchOut) {
    assert(lenRare <= lenFreq);
    const uint32_t *matchOrig = matchOut;
    if (lenFreq == 0 || lenRare == 0) return 0;

    const uint64_t kFreqSpace = 2 * 4 * (0 + 1) - 1;
    const uint64_t kRareSpace = 0;

    const uint32_t *stopFreq = &freq[lenFreq] - kFreqSpace;
    const uint32_t *stopRare = &rare[lenRare] - kRareSpace;

    __m256i  Rare;

    __m256i F;

    if (COMPILER_RARELY( (rare >= stopRare) || (freq >= stopFreq) )) goto FINISH_SCALAR;
    uint32_t valRare;
    valRare = rare[0];
    Rare = _mm256_set1_epi32(valRare);

    uint64_t maxFreq;
    maxFreq = freq[2 * 4 - 1];
    F = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(freq));


    if (COMPILER_RARELY(maxFreq < valRare)) goto ADVANCE_FREQ;

ADVANCE_RARE:
    do {
        *matchOut = valRare;
        valRare = rare[1]; // for next iteration
        rare += 1;
        if (COMPILER_RARELY(rare >= stopRare)) {
            rare -= 1;
            goto FINISH_SCALAR;
        }
        F =  _mm256_cmpeq_epi32(F,Rare);
        Rare = _mm256_set1_epi32(valRare);
        if(_mm256_testz_si256(F,F) == 0)
          matchOut ++;
        F = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(freq));

    } while (maxFreq >= valRare);

    uint64_t maxProbe;

ADVANCE_FREQ:
    do {
        const uint64_t kProbe = (0 + 1) * 2 * 4;
        const uint32_t *probeFreq = freq + kProbe;
        maxProbe = freq[(0 + 2) * 2 * 4 - 1];

        if (COMPILER_RARELY(probeFreq >= stopFreq)) {
            goto FINISH_SCALAR;
        }

        freq = probeFreq;

    } while (maxProbe < valRare);

    maxFreq = maxProbe;

    F = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(freq));


    goto ADVANCE_RARE;

    size_t count;
FINISH_SCALAR:
    count = matchOut - matchOrig;

    lenFreq = stopFreq + kFreqSpace - freq;
    lenRare = stopRare + kRareSpace - rare;

    size_t tail = match_scalar(freq, lenFreq, rare, lenRare, matchOut);

    return count + tail;
}

