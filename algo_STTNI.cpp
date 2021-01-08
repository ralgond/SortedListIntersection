#include <iostream>
#include <cassert>
#include <cstring>

#include <nmmintrin.h>
#include <emmintrin.h>

#include "algo_intersection.h"
#include "sorted_int_file_reader.h"

using namespace std;


inline uint16_t _high16(uint32_t i) {
	return (uint16_t)(i >> 16);
}
inline uint16_t _low16(uint32_t i) {
	return (uint16_t)(i);
}

PrefixList* buildPrefixList(uint32_t* a, uint32_t a_len) {
	PrefixNode* head = NULL;
	PrefixNode* last = NULL;
	for (int i = 0; i < a_len; i++) {
		uint16_t h16 = _high16(a[i]);
		if (head != NULL) {
			assert(last != NULL);
			if (h16 == last->prefix) {
				last->cnt++;
			} else {
				PrefixNode* last2 = new PrefixNode(NULL, i, 1, h16);
				last->next = last2;
				last = last2;
			}
		} else {
			head = new PrefixNode(NULL, i, 1, h16);
			last = head;
		}
	}

	PrefixList* pl = new PrefixList;
	pl->head = head;
	pl->data = a;
	pl->data_len = a_len;
	return pl;
}


void dumpPrefixList(PrefixList* pl) {
	PrefixNode* n = pl->head;
	while (n != NULL) {
		cout<<n->offset<<"\t"<<n->cnt<<"\t"<<hex<<n->prefix<<endl;
		n = n->next;
	}
}

void dumpPrefixList2(PrefixList* pl) {
	PrefixNode* n = pl->head;
        while (n != NULL) {
		for (int i = n->offset; i < n->offset+n->cnt; i++) {
			cout<<pl->data[i]<<endl;
		}
                n = n->next;
        }
}

static __m128i* shuffle_mask16 = new __m128i[256];

inline uint32_t intersect_vector16(uint16_t* A, uint16_t* B, uint16_t* C) {
	uint32_t count = 0;

	__m128i a_v = _mm_loadu_si128((__m128i*)&A[0]);
	__m128i b_v = _mm_loadu_si128((__m128i*)&B[0]);

	__m128i res_v = _mm_cmpestrm(b_v, 8, a_v, 8, _SIDD_UWORD_OPS | _SIDD_CMP_EQUAL_ANY | _SIDD_BIT_MASK);
	//__m128i res_v = _mm_cmpistrm(b_v, a_v, _SIDD_UWORD_OPS | _SIDD_CMP_EQUAL_ANY | _SIDD_BIT_MASK);
	int r = _mm_extract_epi32(res_v, 0);
	__m128i p = _mm_shuffle_epi8(a_v, shuffle_mask16[r]);
	_mm_storeu_si128((__m128i*)&C[count], p);
	count += _mm_popcnt_u32(r);

	return count;
}

uint32_t intersect(uint32_t * a, uint32_t a_len, uint32_t * b, uint32_t b_len, uint32_t* c, uint16_t high16) {
	uint32_t* c_begin = c;
	uint32_t ai = 0;
	uint32_t bi = 0;
	uint32_t a_st = (a_len/8) * 8;
	uint32_t b_st = (b_len/8) * 8;
	uint16_t a16[8];
	uint16_t b16[8];
	uint16_t c16[8];
	while (ai < a_st && bi < b_st) {
		a16[0] = _low16(a[ai+0]);
		a16[1] = _low16(a[ai+1]);
		a16[2] = _low16(a[ai+2]);
		a16[3] = _low16(a[ai+3]);
		a16[4] = _low16(a[ai+4]);
		a16[5] = _low16(a[ai+5]);
		a16[6] = _low16(a[ai+6]);
		a16[7] = _low16(a[ai+7]);

		b16[0] = _low16(b[bi+0]);
		b16[1] = _low16(b[bi+1]);
		b16[2] = _low16(b[bi+2]);
		b16[3] = _low16(b[bi+3]);
		b16[4] = _low16(b[bi+4]);
		b16[5] = _low16(b[bi+5]);
		b16[6] = _low16(b[bi+6]);
		b16[7] = _low16(b[bi+7]);

		uint32_t count = intersect_vector16(a16, b16, c16);

		for (uint32_t i = 0; i < count; i++) {
			uint32_t res = high16;
			res <<= 16;
			res |= c16[i];
			*c = res;
			c++;
		}

		uint16_t a_max = a16[7];
		uint16_t b_max = b16[7];
		ai += (a_max <= b_max) * 8;
		bi += (a_max >= b_max) * 8;
	}


	while (ai < a_len && bi < b_len) {
		if (a[ai] < b[bi]) {
			ai++;
		} else if (a[ai] > b[bi]) {
			bi++;
		} else {
			*c = a[ai];
			c++; ai++; bi++;
		}
	}
	return c - c_begin;
}

uint32_t intersectPrefixList(PrefixList* a, PrefixList* b, uint32_t* c) {
	uint32_t c_pos = 0;
	PrefixNode* an = a->head;
	PrefixNode* bn = b->head;

	while (an != NULL && bn != NULL) {
		if (an->prefix == bn->prefix) {
			c_pos += intersect(a->data + an->offset, an->cnt, b->data + bn->offset, bn->cnt, c + c_pos, an->prefix);
			an = an->next;
			bn = bn->next;
		} else if (an->prefix < bn->prefix) {
			an = an->next;
		} else {
			bn = bn->next;
		}
	}
	return c_pos;
}


int getBit(int value, int position) {
	return ((value & (1<<position)) >> position);
}


void prepare_shuffling_dict16() {
	for (int i = 0; i  < 256; i++) {
		int counter = 0;
		char permutation[16];
		memset(permutation, 0xFF, sizeof(permutation));
		for (char b = 0; b < 8; b++) {
			if (getBit(i, b)) {
				permutation[counter++] = 2*b;
				permutation[counter++] = 2*b + 1;
			}
		}
		__m128i mask = _mm_loadu_si128((const __m128i*)permutation);
		shuffle_mask16[i] = mask;
	}
}


int main3() {
	prepare_shuffling_dict16();

	uint16_t A[8] = {1, 3, 7, 19, 23, 31, 37, 41};
	uint16_t B[8] = {2, 4, 7, 19, 24, 31, 39, 40};
	uint16_t* C = new uint16_t[8];
	uint32_t count = intersect_vector16(A, B, C); 
	cout<<"count="<<count<<endl;
	for (int i = 0; i < count; i++) {
		cout<<C[i]<<endl;
	}
	return 0;
}

int main2() {
	uint32_t a[] = {
		0xFED01001,
		0xFED20001,
		0xFED20002,
		0xFED30001,
		0xFED30002,
		0xFED30003,
	};

	uint32_t a_len = sizeof(a)/sizeof(a[0]);


	PrefixList* a_pl = buildPrefixList(a, a_len);
	dumpPrefixList(a_pl);


	
	uint32_t b[] = {
		0xFED01001,
	//	0xFED20001,
	//	0xFED20002,
		0xFED30001,
	//	0xFED30002,
		0xFED30003,
	};

	uint32_t b_len = sizeof(b)/sizeof(b[0]);


	PrefixList* b_pl = buildPrefixList(b, b_len);
	dumpPrefixList(b_pl);


	uint32_t * c = new uint32_t[a_len];
	uint32_t c_cnt = intersectPrefixList(a_pl, b_pl, c);

	cout<<c_cnt<<endl;
	for (int i = 0; i < c_cnt; i++) {
		cout<<hex<<c[i]<<endl;
	}
	return 0;
}

int main4(int argc, char* argv[]) {
        string file1 = argv[1];

        uint32_t a_len = 0;
        uint32_t* a = read_sorted_int_from_file(file1, &a_len);

        PrefixList* pl = buildPrefixList(a, a_len);

	dumpPrefixList2(pl);

	return 0;
}


int main5() {
        prepare_shuffling_dict16();

        uint32_t _A[8] = {16449536, 16449580, 16449914, 16450040, 16450068, 16450264, 16450296, 16450304};
	uint16_t A[8] = {0};
	for (int i = 0; i < 8; i++) {
		A[i] = _low16(_A[i]);
	}

	uint32_t _B[8] = {16449622, 16449724, 16449807, 16449856, 16449861, 16450264, 16450500, 16450581};
        uint16_t B[8] = {0};
	for (int i = 0; i < 8; i++) {
		B[i] = _low16(_B[i]);
	}

        uint16_t* C = new uint16_t[8];
        uint32_t count = intersect_vector16(A, B, C);
        cout<<"count="<<count<<endl;
        for (int i = 0; i < count; i++) {
                cout<<C[i]<<endl;
        }
        return 0;
}

