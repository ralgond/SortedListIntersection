#include <fstream>
#include <string>
#include <iostream>
#include <cstdlib>
#include <string>

#include "utils.h"
#include "algo_intersection.h"

using namespace std;

uint32_t block_merge_intersection(const uint32_t* a, uint32_t a_len, const uint32_t* b, uint32_t b_len, uint32_t* c) {
	uint32_t c_pos = 0;
	uint32_t a_pos = 0;
	uint32_t b_pos = 0;

	while (a_pos+1<a_len && b_pos+1<b_len) {
		int a_data_0 = a[a_pos]; int a_data_1 = a[a_pos+1];
		int b_data_0 = b[b_pos]; int b_data_1 = b[b_pos+1];
		if (a_data_0 == b_data_0) {
			c[c_pos++] = a_data_0;
		}
	        else if (a_data_0 == b_data_1) {	
			c[c_pos++] = a_data_0;
			goto advanceB;
		} 
		else if (a_data_1 == b_data_0) {
			c[c_pos++] = a_data_1;
			goto advanceA;
		}
		if (a_data_1 == b_data_1) {
			c[c_pos++] = a_data_1;
			goto advanceAB;
		}
		else if (a_data_1 > b_data_1) goto advanceB;
		else goto advanceA;
advanceA:
		a_pos += 2;
		if (a_pos >= a_len){break;} else {continue;}
advanceB:
		b_pos += 2;
		if (b_pos >= b_len){break;} else {continue;}
advanceAB:
		a_pos += 2; b_pos += 2;
		if (a_pos >= a_len || b_pos >= b_len){break;}
	}
	
	while (a_pos < a_len && b_pos < b_len) {
		int a_data = a[a_pos];
		int b_data = b[b_pos];
		if (a_data == b_data) {
			c[c_pos++] = a_data;
			a_pos++;
			b_pos++;
		} else if (a_data > b_data) {
			b_pos++;
		} else {
			a_pos++;
		}
	}
	return c_pos;
}
