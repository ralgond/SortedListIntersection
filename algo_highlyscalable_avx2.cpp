#include <immintrin.h>

#include <stdint.h>
#include <cstddef>
#include <cassert>
#include <cstring>
#include <iostream>

using namespace std;

static void dump(__m256i v) {
	int item;
	item = _mm256_extract_epi32(v, 0);
	cout<<item<<endl;
	item = _mm256_extract_epi32(v, 1);
	cout<<item<<endl;
	item = _mm256_extract_epi32(v, 2);
	cout<<item<<endl;
	item = _mm256_extract_epi32(v, 3);
	cout<<item<<endl;
	item = _mm256_extract_epi32(v, 4);
	cout<<item<<endl;
	item = _mm256_extract_epi32(v, 5);
	cout<<item<<endl;
	item = _mm256_extract_epi32(v, 6);
	cout<<item<<endl;
	item = _mm256_extract_epi32(v, 7);
	cout<<item<<endl;
}

static __m256i shuffle_mask32[256];

static int getBit(int value, int position) {
        return ((value & (1<<position)) >> position);
}


void prepare_shuffling_dict32() {
        for (int i = 0; i  < 256; i++) {
                int counter = 0;
                char permutation[32];
                memset(permutation, 0xFF, sizeof(permutation));
                for (char b = 0; b < 8; b++) {
                        if (getBit(i, b)) {
                                //permutation[counter++] = 4*b;
                                //permutation[counter++] = 4*b + 1;
                                //permutation[counter++] = 4*b + 2;
                                //permutation[counter++] = 4*b + 3;
				
				permutation[counter++] = b & 0xff;
                                permutation[counter++] = (b >> 8) & 0xff;
                                permutation[counter++] = (b >> 16) & 0xff;
                                permutation[counter++] = (b >> 24) & 0xff;

                        }
                }
                __m256i mask = _mm256_loadu_si256((const __m256i*)permutation);
                shuffle_mask32[i] = mask;
        }
}


size_t highlyscalable_avx2_intersection(const uint32_t *A, const size_t s_a,
                                     const uint32_t *B, const size_t s_b,
                                     uint32_t *out) {
  assert(out != A);
  assert(out != B);
  const uint32_t *const initout(out);
  size_t i_a = 0, i_b = 0;

  // trim lengths to be a multiple of 8
  size_t st_a = (s_a / 8) * 8;
  size_t st_b = (s_b / 8) * 8;

  while (i_a < st_a && i_b < st_b) {
    //[ load segments of four 32-bit elements
    __m256i v_a = _mm256_loadu_si256((__m256i *)&A[i_a]);
    __m256i v_b = _mm256_loadu_si256((__m256i *)&B[i_b]);
    //]

    //[ move pointers
    const uint32_t a_max = A[i_a + 7];
    const uint32_t b_max = B[i_b + 7];
    i_a += (a_max <= b_max) * 8;
    i_b += (a_max >= b_max) * 8;
    //]

    //[ compute mask of common elements
    const __m256i rotate_arg = _mm256_set_epi32(0,7,6,5,4,3,2,1);
    __m256i cmp_mask1 = _mm256_cmpeq_epi32(v_a, v_b); // pairwise comparison

    v_b = _mm256_permutevar8x32_epi32(v_b, rotate_arg);    // shuffling
    __m256i cmp_mask2 = _mm256_cmpeq_epi32(v_a, v_b); // again...
    
    v_b = _mm256_permutevar8x32_epi32(v_b, rotate_arg);
    __m256i cmp_mask3 = _mm256_cmpeq_epi32(v_a, v_b); // and again...
    
    v_b = _mm256_permutevar8x32_epi32(v_b, rotate_arg);
    __m256i cmp_mask4 = _mm256_cmpeq_epi32(v_a, v_b); // and again.

    v_b = _mm256_permutevar8x32_epi32(v_b, rotate_arg);
    __m256i cmp_mask5 = _mm256_cmpeq_epi32(v_a, v_b); // and again.

    v_b = _mm256_permutevar8x32_epi32(v_b, rotate_arg);
    __m256i cmp_mask6 = _mm256_cmpeq_epi32(v_a, v_b); // and again.
    
    v_b = _mm256_permutevar8x32_epi32(v_b, rotate_arg);
    __m256i cmp_mask7 = _mm256_cmpeq_epi32(v_a, v_b); // and again.

    v_b = _mm256_permutevar8x32_epi32(v_b, rotate_arg);
    __m256i cmp_mask8 = _mm256_cmpeq_epi32(v_a, v_b); // and again.
    
    __m256i cmp_mask_1_4 = _mm256_or_si256(
        _mm256_or_si256(cmp_mask1, cmp_mask2),
        _mm256_or_si256(cmp_mask3, cmp_mask4)); // OR-ing of comparison masks
    __m256i cmp_mask_5_8 = _mm256_or_si256(
        _mm256_or_si256(cmp_mask5, cmp_mask6),
        _mm256_or_si256(cmp_mask7, cmp_mask8)); // OR-ing of comparison masks

    __m256i cmp_mask = _mm256_or_si256(cmp_mask_1_4, cmp_mask_5_8);

    // convert the 256-bit mask to the 8-bit mask
    const int mask = _mm256_movemask_ps(_mm256_castsi256_ps(cmp_mask));
    //cout<<"mask: "<<mask<<endl;

    //[ copy out common elements
    const __m256i p = _mm256_permutevar8x32_epi32(v_a, shuffle_mask32[mask]);
    _mm256_storeu_si256((__m256i *)out, p);
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




int main1() {
        prepare_shuffling_dict32();

        uint32_t A[8] = {1, 3, 7, 19, 23, 31, 37, 41};
        uint32_t B[8] = {2, 4, 7, 19, 24, 31, 39, 40};
        uint32_t* C = new uint32_t[8];
        
	uint32_t count = highlyscalable_avx2_intersection(A, 8, B, 8, C);
	cout<<"count="<<count<<endl;
        for (int i = 0; i < count; i++) {
                cout<<C[i]<<endl;
        }
        return 0;
}

