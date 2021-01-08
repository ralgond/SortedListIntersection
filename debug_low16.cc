#include <iostream>

using namespace std;

uint16_t _low16(uint32_t i) {
	return (uint16_t)i;
}

int main(int argc, char* argv[]) {
	uint32_t a = atoi(argv[1]);
	cout<<hex<<_low16(a)<<endl;
}
