#include <immintrin.h>
#include <smmintrin.h>

#include <stdint.h>
#include <cstddef>
#include <cassert>
#include <cstring>
#include <iostream>

using namespace std;

static void dump(__m512i v) {
	int item;
	
	__m128i a;
	a = _mm512_extracti32x4_epi32(v, 0);
	item = _mm_extract_epi32(a, 0);
	cout<<item<<endl;
	item = _mm_extract_epi32(a, 1);
	cout<<item<<endl;
	item = _mm_extract_epi32(a, 2);
	cout<<item<<endl;
	item = _mm_extract_epi32(a, 3);
	cout<<item<<endl;

	a = _mm512_extracti32x4_epi32(v, 1);
	item = _mm_extract_epi32(a, 0);
	cout<<item<<endl;
	item = _mm_extract_epi32(a, 1);
	cout<<item<<endl;
	item = _mm_extract_epi32(a, 2);
	cout<<item<<endl;
	item = _mm_extract_epi32(a, 3);
	cout<<item<<endl;
	
	a = _mm512_extracti32x4_epi32(v, 2);
	item = _mm_extract_epi32(a, 0);
	cout<<item<<endl;
	item = _mm_extract_epi32(a, 1);
	cout<<item<<endl;
	item = _mm_extract_epi32(a, 2);
	cout<<item<<endl;
	item = _mm_extract_epi32(a, 3);
	cout<<item<<endl;
	
	a = _mm512_extracti32x4_epi32(v, 3);
	item = _mm_extract_epi32(a, 0);
	cout<<item<<endl;
	item = _mm_extract_epi32(a, 1);
	cout<<item<<endl;
	item = _mm_extract_epi32(a, 2);
	cout<<item<<endl;
	item = _mm_extract_epi32(a, 3);
	cout<<item<<endl;
}

static __m512i shuffle_mask64[65536];

static int getBit(int value, int position) {
        return ((value & (1<<position)) >> position);
}


void prepare_shuffling_dict64() {
        for (int i = 0; i  < 65536; i++) {
                int counter = 0;
                char permutation[64];
                memset(permutation, 0xFF, sizeof(permutation));
                for (char b = 0; b < 16; b++) {
                        if (getBit(i, b)) {
				permutation[counter++] = b & 0xff;
                                permutation[counter++] = (b >> 8) & 0xff;
                                permutation[counter++] = (b >> 16) & 0xff;
                                permutation[counter++] = (b >> 24) & 0xff;

                        }
                }
                __m512i mask = _mm512_loadu_si512(permutation);
                shuffle_mask64[i] = mask;
        }
}


size_t highlyscalable_avx512_intersection(const uint32_t *A, const size_t s_a,
                                     const uint32_t *B, const size_t s_b,
                                     uint32_t *out) {
  assert(out != A);
  assert(out != B);
  const uint32_t *const initout(out);
  size_t i_a = 0, i_b = 0;

  // trim lengths to be a multiple of 16
  size_t st_a = (s_a / 16) * 16;
  size_t st_b = (s_b / 16) * 16;

  while (i_a < st_a && i_b < st_b) {
    __m512i v_a = _mm512_loadu_si512(&A[i_a]);
    __m512i v_b = _mm512_loadu_si512(&B[i_b]);
    //]

    //[ move pointers
    const uint32_t a_max = A[i_a + 15];
    const uint32_t b_max = B[i_b + 15];
    i_a += (a_max <= b_max) * 16;
    i_b += (a_max >= b_max) * 16;
    
    //[ compute mask of common elements
    const __m512i rotate_arg = _mm512_set_epi32(0, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1);
    __mmask16 cmp_mask = _mm512_cmpeq_epu32_mask(v_a, v_b); // pairwise comparison

    for (int i = 0; i < 15; i++) {
	    v_b = _mm512_permutexvar_epi32(rotate_arg, v_b);    // shuffling
	    __mmask16 cmp_mask1 = _mm512_cmpeq_epu32_mask(v_a, v_b);
	    cmp_mask = _mm512_kor(cmp_mask, cmp_mask1);
    }

    const int mask = _mm512_mask2int(cmp_mask);
    //cout<<"mask: "<<mask<<endl;

    //[ copy out common elements
    //const __m256i p = _mm256_permutevar8x32_epi32(v_a, shuffle_mask64[mask]);
    //__m256i p; //_mm256_permutevar8x32_epi32(v_a, shuffle_mask64[mask]);
    __m512i p = _mm512_permutexvar_epi32(shuffle_mask64[mask], v_a);
    _mm512_storeu_si512(out, p);
    out += _mm_popcnt_u32(mask); // a number of elements is a weight of the mask
                                 //]
  }

  // intersect the tail using scalar intersection
  while (i_a < s_a && i_b < s_b) {
    if (A[i_a] < B[i_b]) {
      i_a++;
    } else if (B[i_b] < A[i_a]) {
      i_b++;
    } else {
      *out++ = B[i_b];
      ;
      i_a++;
      i_b++;
    }
  }

  return out - initout;
}




static int main1() {
        prepare_shuffling_dict64();

        uint32_t A[16] = {1, 3, 7, 19, 23, 31, 37, 41, 45, 46, 48, 50, 52, 54, 56, 58};
        uint32_t B[16] = {2, 4, 7, 19, 24, 31, 39, 40, 43, 44, 46, 49, 51, 53, 55, 57};
        uint32_t* C = new uint32_t[16];
        
	uint32_t count = highlyscalable_avx512_intersection(A, 16, B, 16, C);
	cout<<"count="<<count<<endl;
        for (int i = 0; i < count; i++) {
                cout<<C[i]<<endl;
        }
        return 0;
}

static int main2() {
	__m512i rotate_arg = _mm512_set_epi32(0, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1);
	dump(rotate_arg);
        uint32_t A[16] = {1, 3, 7, 19, 23, 31, 37, 41, 45, 46, 48, 50, 52, 54, 56, 58};
	__m512i a = _mm512_loadu_si512(A);
	dump(a);
	cout<<""<<endl;
	__m512i a_1 = _mm512_permutexvar_epi32(rotate_arg, a);
	dump(a_1);
	cout<<""<<endl;
	__m512i a_2 = _mm512_permutexvar_epi32(rotate_arg, a_1);
	dump(a_2);
	return 0;
}
