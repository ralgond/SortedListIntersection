
#include <stdint.h>

#include "bloom_filter.hpp"

uint32_t bisearch_intersection(const uint32_t* a, uint32_t a_len, const uint32_t* b, uint32_t b_len, uint32_t* c);

uint32_t block_merge_based_intersection(const uint32_t* a, uint32_t a_len, const uint32_t* b, uint32_t b_len, uint32_t* c);

bloom_filter* create_bloom_filter(const uint32_t* a, uint32_t a_len);

uint32_t bloomfilter_intersection(bloom_filter* filter, const uint32_t* a, uint32_t a_len, const uint32_t* b, uint32_t b_len, uint32_t* c);


uint32_t simple_merge_based_intersection(const uint32_t* a, uint32_t a_len, const uint32_t* b, uint32_t b_len, uint32_t* c);

uint32_t simple_merge_based_intersection2(const uint32_t* a, uint32_t a_len, const uint32_t* b, uint32_t b_len, uint32_t* c);

size_t v1_avx2_intersection(const uint32_t *freq, size_t lenFreq, const uint32_t *rare, size_t lenRare, uint32_t *matchOut);

size_t v3_avx2_intersection(const uint32_t *freq, size_t lenFreq, const uint32_t *rare, size_t lenRare, uint32_t *matchOut);

size_t match_scalar(const uint32_t *A, const size_t lenA, const uint32_t *B, const size_t lenB, uint32_t *out);

size_t scalar(const uint32_t *A, const size_t lenA, const uint32_t *B, const size_t lenB, uint32_t *out);
