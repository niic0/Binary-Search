#include <stdio.h>
#include <stdlib.h>

#include "chrono.h"

int main () {
	unsigned long long int i;
	unsigned long long int N = 1e8;
	unsigned long long int a = 0;
	float t;

	chrono_reset();
	for (i=0 ; i<N ; i++) a += i;
	t = chrono_lap();
	printf("%llu %.6lf s\n",a,t);

	chrono_reset();
	a = N*(N+1)/2;
	t = chrono_lap();
	printf("%llu %.6lf s\n",a,t);
	
	return 0;
}
