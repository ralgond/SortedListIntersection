#include <sys/time.h>

#include <cstddef>

#include "utils.h"

long currentTimeMs() {
    struct timeval time;
    gettimeofday(&time, NULL);
    return time.tv_sec*1000 + time.tv_usec/1000;
}

int bisearch(const uint32_t* a, uint32_t a_len, uint32_t target) {
	int low = 0;
	int high = a_len - 1;

	while (high-low > 4) {
		int mid = low + (high-low) / 2;
		int mid_v = a[mid];
		if (mid_v == target)
			return mid;
		else if (target < mid_v)
			high = mid - 1;
		else
			low = mid + 1;
	}

	int i = low;
	for (; i<=high; i++) {
		if (!(a[i] < target))
			break;
	}

	if (a[i] == target)
		return i;
	else
		return -1*(i+1);
}
