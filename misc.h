/* @file misc.h
**
** miscellaneous definitions and inline functions
** @@
******************************************************************************/

#ifndef MISC_H
#define MISC_H

#include <sys/resource.h>
#include <sys/time.h>
#include <stdint.h>
#include <math.h>

double realtime(void);

double cputime(void);

long peakrss(void);

// Prints to the provided buffer a nice number of bytes (KB, MB, GB, etc)
//from https://www.mbeckler.org/blog/?p=114
void print_size(const char* name, uint64_t bytes);

int64_t mm_parse_num(const char* str);

#endif
