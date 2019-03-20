#include "main.h"
#include <stdlib.h>

// This test fills a buffer with random numbers, size determined by BUFFER_SIZE in main.h
u64 randInt(u64 seed);

int test5_run()
{
	uint64 randState = randInt(OSGetTime());
	for (int i=0; i<BUFFER_SIZE;)
	{
		randState = randInt(randState);
		char randBuffer[20];
		itoa(randState, randBuffer, 10);
		memcpy(&fsBuffer[i], randBuffer, strlen(randBuffer));
		i+=strlen(randBuffer);
	}
	
	return 0;
}