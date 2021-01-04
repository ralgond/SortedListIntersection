
#include "bloom_filter.hpp"
#include "utils.h"

uint32_t bloomfilter_intersection(bloom_filter* filter, const uint32_t* a, uint32_t a_len, const uint32_t* b, uint32_t b_len, uint32_t* c) {
	uint32_t c_pos = 0;
	for (int i = 0; i < b_len; i++) {
		int b_data = b[i];
		if (filter->contains(b_data)) {
			if (bisearch(a, a_len, b_data) >= 0) {
				c[c_pos++] = b_data;
			}
		}
	}
	return c_pos;
}

bloom_filter* create_bloom_filter(const uint32_t* a, uint32_t a_len) {
	bloom_parameters parameters;
	
	parameters.projected_element_count = a_len;
	
	parameters.false_positive_probability = 0.0001;
	
	parameters.random_seed = 0xA5A5A5A5;

	parameters.compute_optimal_parameters();

	bloom_filter* filter = new bloom_filter(parameters);


	for (int i = 0; i < a_len; i++) {
		filter->insert(a[i]);
	}

	return filter;

}
