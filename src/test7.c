#include "main.h"

// Code adopted from this research paper http://www.stuffedcow.net/files/gpuarch-ispass2010.pdf which code can be found on http://www.stuffedcow.net/research/cudabmk


#define REPEAT256(S) S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S S

#define ARRAY_SIZE 241600000 // I can't tell if my code doesn't do something or if this number really needs to be high before it gets into a 1 second benchmark.
#define ITERATIONS 4
#define IGNORE_ITERATIONS 1
#define STRIDE (ARRAY_SIZE) // Set from 1 to ARRAY_SIZE

int GCF(int a,int b) {
int gcf;

if(b!=0) gcf=GCF(b, a%b);
else gcf=a;

return gcf;
}

int test7_run()
{
	// Create buffers
	unsigned int ** d_a;
	unsigned int * h_a = (unsigned int *)memAlloc(sizeof(unsigned int) * (ARRAY_SIZE));
	
	// Create pointer array
	for (int i=0; i<ARRAY_SIZE; i++) {
		h_a[i] = ((unsigned int)(uintptr_t)d_a) + ((i+STRIDE) % ARRAY_SIZE)*sizeof(unsigned int);
	}
	
	unsigned int *currPtr = (unsigned int*)h_a;
	
	// The GCF step thing is noted as an optimalization, does this invalidate the benchmark?
	int step = GCF(STRIDE, ARRAY_SIZE);
	for (int i= -IGNORE_ITERATIONS; i<ITERATIONS; i+=step) {
		REPEAT256(currPtr=*(unsigned int **)currPtr;) // Unroll macro, creates an unrolled loop of 256 instructions
	}
	memFree(h_a);
	
	return 0;
}