

#include "algo_intersection.h"
#include "utils.h"

uint32_t bisearch_intersection(const uint32_t* a, uint32_t a_len, const uint32_t* b, uint32_t b_len, uint32_t* c) {
	uint32_t c_pos = 0;
	for (int i = 0; i < b_len; i++) {
		if (bisearch(a, a_len, b[i]) >= 0) {
			c[c_pos++] = b[i];
		}
	}

	return c_pos;
}

