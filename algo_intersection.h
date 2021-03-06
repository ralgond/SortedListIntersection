#ifndef _ALGO_INTERSECTION_H_
#define _ALGO_INTERSECTION_H_


#include <stdint.h>

#include "bloom_filter.hpp"

uint32_t bisearch_intersection(const uint32_t* a, uint32_t a_len, const uint32_t* b, uint32_t b_len, uint32_t* c);

uint32_t block_merge_intersection(const uint32_t* a, uint32_t a_len, const uint32_t* b, uint32_t b_len, uint32_t* c);

bloom_filter* create_bloom_filter(const uint32_t* a, uint32_t a_len);
uint32_t bloomfilter_intersection(bloom_filter* filter, const uint32_t* a, uint32_t a_len, const uint32_t* b, uint32_t b_len, uint32_t* c);


size_t v1_avx2_intersection(const uint32_t *freq, size_t lenFreq, const uint32_t *rare, size_t lenRare, uint32_t *matchOut);

size_t v3_avx2_intersection(const uint32_t *freq, size_t lenFreq, const uint32_t *rare, size_t lenRare, uint32_t *matchOut);

size_t match_scalar(const uint32_t *A, const size_t lenA, const uint32_t *B, const size_t lenB, uint32_t *out);

size_t scalar(const uint32_t *A, const size_t lenA, const uint32_t *B, const size_t lenB, uint32_t *out);


size_t highlyscalable_SIMD_intersection(const uint32_t *A, const size_t s_a,
                                     const uint32_t *B, const size_t s_b,
                                     uint32_t *out);

void prepare_shuffling_dict32();
size_t highlyscalable_avx2_intersection(const uint32_t *A, const size_t s_a,
                                     const uint32_t *B, const size_t s_b,
                                     uint32_t *out);

void prepare_shuffling_dict64();
size_t highlyscalable_avx512_intersection(const uint32_t *A, const size_t s_a,
                                     const uint32_t *B, const size_t s_b,
                                     uint32_t *out);


size_t lemire_highlyscalable_SIMD_intersection(const uint32_t *A, const size_t s_a,
                                            const uint32_t *B, const size_t s_b,
                                            uint32_t *out);


////////////////////////////////////////////////////////////////////////////////////
class PrefixNode {
public:
        PrefixNode* next;
        uint32_t offset;
        uint16_t cnt;
        uint16_t prefix;

        PrefixNode(PrefixNode* next, uint32_t offset, uint16_t cnt, uint16_t prefix) {
                this->next = next;
                this->offset = offset;
                this->cnt = cnt;
                this->prefix = prefix;
        }
};

class PrefixList {
public:
        PrefixNode* head;
        uint32_t* data;
        uint32_t data_len;
};

PrefixList* buildPrefixList(uint32_t* a, uint32_t a_len);

uint32_t intersectPrefixList(PrefixList* a, PrefixList* b, uint32_t* c);

void prepare_shuffling_dict16();

///////////////////////////////////////////////////////////////////////////////////

#endif
