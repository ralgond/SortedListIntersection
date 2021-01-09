#include <immintrin.h>
#include <stdint.h>
#include <iostream>

using namespace std;


void dump(__m256i v) {
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


//void method1() {
//	__m256 tmp =  ( __m256 ) _mm256_permute_ps((__m256)_source, _MM_SHUFFLE ( 0,3,2,1 ));
//	* ( _dest ) =  ( __m256i) _mm256_blend_ps(tmp, _mm256_permute2f128_ps ( tmp,tmp,1 ), 136); 
//}
int main() {
	__m256i v;

	int a[8] = {0, 1, 2, 3, 4, 5, 6, 7};

	v = _mm256_loadu_si256((__m256i*)&a[0]);

	__m256i idx = _mm256_set_epi32(0,7,6,5,4,3,2,1);


	v = _mm256_permutevar8x32_epi32(v, idx);
	dump(v);

	cout<<""<<endl;

	v = _mm256_permutevar8x32_epi32(v, idx);
	dump(v);
	return 0;	
}
