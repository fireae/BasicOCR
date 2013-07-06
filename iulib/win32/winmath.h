#ifndef h_winmath__
#define h_winmath__

#ifdef _WIN32

#include <cstdlib>

double round(double input) {
	return floor(input);
}

#endif

#endif