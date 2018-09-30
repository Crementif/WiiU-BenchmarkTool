#include "main.h"

#define ARRAY_SIZE 2560000
#define ITERATIONS 1000

struct pointerEntry_t
{
	struct pointerEntry_t* next;
};

__attribute__((used)) void* test7Result = 0;



int test7_run()
{
	struct pointerEntry_t* ptrArray = (struct pointerEntry_t*)memAlloc(sizeof(struct pointerEntry_t) * (ARRAY_SIZE));
	
	// Create pointer array
	for (int i=0; i<ARRAY_SIZE; i++) {
		ptrArray[i].next = ptrArray+i+1;
	}
	ptrArray[ARRAY_SIZE-1].next = ptrArray+0;
	
	// Pointer chasing!
	struct pointerEntry_t* currPtr = ptrArray;
	
	for (int j=0; j<ITERATIONS; j++)
	{
		for (int i=0; i<ARRAY_SIZE; i++)
		{
			currPtr = currPtr->next;
		}
	}
	
	test7Result = currPtr;
	memFree(ptrArray);
	
	return 0;
}