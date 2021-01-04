#include <fstream>
#include <string>
#include <iostream>
#include <cstdlib>
#include <string>

#include "utils.h"

using namespace std;

uint32_t simple_merge_intersection(const uint32_t* a, uint32_t a_len, const uint32_t* b, uint32_t b_len, uint32_t* c) {
	uint32_t pos_c = 0;
	uint32_t pos_a = 0;
	uint32_t pos_b = 0;

	while (pos_a < a_len && pos_b < b_len) {
		int data_a = a[pos_a];
		int data_b = b[pos_b];
		if (data_a == data_b) {
			c[pos_c++] = data_a;
			pos_a++;
			pos_b++;
		} else if (data_a > data_b) {
			pos_b++;
		} else {
			pos_a++;
		}
	}

	return pos_c;
}


uint32_t simple_merge_intersection2(const uint32_t* a, uint32_t a_len, uint32_t* b, uint32_t b_len, uint32_t* c) {
	uint32_t pos_c = 0;
	uint32_t pos_a = 0;
	uint32_t pos_b = 0;

	while (pos_a < a_len && pos_b < b_len) {
		int data_a = a[pos_a];
		int data_b = b[pos_b];
		if (data_a == data_b) {
			c[pos_c++] = data_a;
			pos_a++;
			pos_b++;
		} else {
			pos_a += (data_a < data_b);
			pos_b += (data_b < data_a);
		}
	}

	return pos_c;
}

