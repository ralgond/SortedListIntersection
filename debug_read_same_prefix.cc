#include <iostream>
#include <stdint.h>

#include "sorted_int_file_reader.h"

using namespace std;

uint16_t _high16(uint32_t i) {
	return (uint16_t)(i >> 16);
}

int main(int argc, char* argv[]) {

	uint32_t n = atoi(argv[1]);
	uint16_t n_high16 = _high16(n);

	string file = argv[2];
	
	uint32_t a_len = 0;
        uint32_t* a = read_sorted_int_from_file(file, &a_len);

	for (int i = 0; i < a_len; i++) {
		if (n_high16 == _high16(a[i]))
			cout<<a[i]<<endl;
	}
	return 0;
}
