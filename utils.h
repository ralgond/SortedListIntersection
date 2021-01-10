#ifndef _SLI_UTILS_H_
#define _SLI_UTILS_H_

#include <stdint.h>

long currentTimeMs();

int bisearch(const uint32_t* a, uint32_t a_len, uint32_t target);

#endif
