#include "main.h"
#include <stdlib.h>

// This test fills a buffer with random numbers, size determined by BUFFER_SIZE in main.h


// Implement Xorshift RNG
static uint64 randState;
uint64 randInt()
{
	randState ^= randState << 13;
	randState ^= randState >> 17;
	randState ^= randState << 5;
	return randState;
}

int test5_run()
{
	randState = OSGetTime();
	uint64 newAddition;
	for (int i=0; i<BUFFER_SIZE;)
	{
		newAddition = randInt();
		char randBuffer[20];
		itoa(newAddition, randBuffer, 10);
		memcpy(&fsBuffer[i], randBuffer, strlen(randBuffer));
		i+=strlen(randBuffer);
	}
	
	return 0;
}