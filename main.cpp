#include <fstream>
#include <string>
#include <iostream>
#include <cstdlib>
#include <string>
#include <stdint.h>
#include "utils.h"

#include "sorted_int_file_reader.h"

#include "algo_intersection.h"

using namespace std;

float qps(int N, long ms) {
	return N*1.0/(ms*1.0/1000);
}

int main(int argc, char* argv[]) {

	string file1 = argv[1];
	string file2 = argv[2];

	uint32_t a_len = 0;
	uint32_t* a = read_sorted_int_from_file(file1, &a_len);
	cout<<"file "<<file1<<": "<<a_len<<endl;

	uint32_t b_len = 0;
	uint32_t* b = read_sorted_int_from_file(file2, &b_len);
	cout<<"file "<<file2<<": "<<b_len<<endl;


	uint32_t rareLen = 0;
	uint32_t* rare = NULL;
	uint32_t freqLen = 0;
	uint32_t* freq = NULL;

	if (a_len < b_len) {
		rareLen = a_len;
		rare = a;
		freqLen = b_len;
		freq = b;
	} else {
		rareLen = b_len;
		rare = b;
		freqLen = a_len;
		freq = a;
	}


	uint32_t* c = new uint32_t[rareLen];

	uint32_t cnt = 0;

	bloom_filter* filter = create_bloom_filter(freq, freqLen);

	long time1, time2;

	int N = 10000;

	time1 = currentTimeMs();
	for (int i = 0; i < 10000; i++)
		cnt = v1_avx2_intersection(freq, freqLen, rare, rareLen, c);
	time2 = currentTimeMs();
	cout << "v1_avx2\t" << (time2-time1) << "ms"<<"\t"<<qps(N, (time2-time1))<<"\t"<<cnt<<endl;
	
	time1 = currentTimeMs();
	for (int i = 0; i < 10000; i++)
		cnt = v3_avx2_intersection(freq, freqLen, rare, rareLen, c);
	time2 = currentTimeMs();
	cout << "v3_avx2\t" << (time2-time1) << "ms"<<"\t"<<qps(N, (time2-time1))<<"\t"<<cnt<<endl;

	time1 = currentTimeMs();
	for (int i = 0; i < 10000; i++)
		cnt = highlyscalable_SIMD_intersection(freq, freqLen, rare, rareLen, c);
	time2 = currentTimeMs();
	cout << "highlyscalable_SIMD\t" << (time2-time1) << "ms"<<"\t"<<qps(N, (time2-time1))<<"\t"<<cnt<<endl;

	time1 = currentTimeMs();
	for (int i = 0; i < 10000; i++)
		cnt = lemire_highlyscalable_SIMD_intersection(freq, freqLen, rare, rareLen, c);
	time2 = currentTimeMs();
	cout << "lemire_highlyscalable_SIMD\t" << (time2-time1) << "ms"<<"\t"<<qps(N, (time2-time1))<<"\t"<<cnt<<endl;

	time1 = currentTimeMs();
	for (int i = 0; i < 10000; i++)
		cnt = scalar(freq, freqLen, rare, rareLen, c);
	time2 = currentTimeMs();
	cout << "scalar\t" << (time2-time1) << "ms"<<"\t"<<qps(N, (time2-time1))<<"\t"<<cnt<<endl;

	time1 = currentTimeMs();
	for (int i = 0; i < 10000; i++)
		cnt = block_merge_intersection(freq, freqLen, rare, rareLen, c);
	time2 = currentTimeMs();
	cout << "block_merge\t" << (time2-time1) << "ms"<<"\t"<<qps(N, (time2-time1))<<"\t"<<cnt<<endl;

//	time1 = currentTimeMs();
//	for (int i = 0; i < 10000; i++)
//		cnt = bisearch_intersection(freq, freqLen, rare, rareLen, c);
//	time2 = currentTimeMs();
//	cout << "bisearch\t" << (time2-time1) << "ms"<<"\t"<<qps(N, (time2-time1))<<"\t"<<cnt<<endl;
	
//	time1 = currentTimeMs();
//	for (int i = 0; i < 10000; i++)
//		cnt = bloomfilter_intersection(filter, freq, freqLen, rare, rareLen, c);
//	time2 = currentTimeMs();
//	cout << "bloom\t" << (time2-time1) << "ms"<<"\t"<<qps(N, (time2-time1))<<"\t"<<cnt<<endl;

	prepare_shuffling_dict16();
	PrefixList* rarePl = buildPrefixList(rare, rareLen);
	PrefixList* freqPl = buildPrefixList(freq, freqLen);

	time1 = currentTimeMs();
	for (int i = 0; i < 10000; i++)
		cnt = intersectPrefixList(freqPl, rarePl, c);
	time2 = currentTimeMs();
	cout << "sttni\t" << (time2-time1) << "ms"<<"\t"<<qps(N, (time2-time1))<<"\t"<<cnt<<endl;


	return 0;
}


