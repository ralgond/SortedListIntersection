#include <string>
#include <fstream>
#include <cstdlib>
#include <vector>

using namespace std;

uint32_t* read_sorted_int_from_file(string filepath, uint32_t* size) {
	ifstream inf(filepath);
	if (inf) {
		vector<uint32_t> v;
		string line;
		while (getline(inf, line)) {
			int num = atoi(line.c_str());
			v.push_back(num);
		}

		*size = v.size();
		uint32_t* ret = new uint32_t[*size];
		for (int i = 0; i < v.size(); i++) {
			ret[i] = v.at(i);
		}
		return ret;
	} else {
		return NULL;
	}
}
