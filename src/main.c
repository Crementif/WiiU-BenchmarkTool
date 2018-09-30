#include "main.h"
#include "benchmarkTests.h"
#include "game/game.c"

#define TEST_STATE_QUEUED		(0)
#define TEST_STATE_RUNNING		(1)
#define TEST_STATE_COMPLETED	(2)
#define TEST_STATE_FAILED		(3)
#define TEST_STATE_SKIPPED		(4)

// If set to a specific benchmark index, will only execute that one. Setting it to -1 will execute all benchmarks normally.
#define DEBUG_BENCHMARK	(-1)

typedef struct
{
	uint64 tickStart;
	uint64 tickEnd;
	uint8 state;
	char* name;
	int(*testFunc)();
}testResultState_t;

testResultState_t testResultList[32];
sint32 testResultCount = 0;
int allComplete = 0;

void drawStatusScreen()
{
	OSScreenClearBufferEx(0, 0);
	OSScreenClearBufferEx(1, 0);
	drawText(-1, 1, 1, 0xFF905000, "Wii U benchmark tool v0.3 (single-core)");
	drawText(-1, 1, 3, 0xFFFFFFFF, "Status      Duration    Test-Name");
	drawHorizontalLineW2(-1, 1, 4, 50, 0xFFFFFFFF);
	char text[128];
	allComplete = 1;
	sint32 totalTime = 0;
	for(sint32 i=0; i<testResultCount; i++)
	{
		char durationText[32];
		durationText[0] = '?';
		durationText[1] = '\0';
		if( testResultList[i].state == TEST_STATE_COMPLETED )
		{
			drawText(0, 1, 5+i, 0x80FF8000, "COMPLETED");
			// calculate duration
			uint64 durationInTicks = testResultList[i].tickEnd - testResultList[i].tickStart;
			uint64 durationinMS = durationInTicks * (uint64)1000 / (uint64)TICK_SPEED;
			_snprintf(durationText, sizeof(text), "%dms", (sint32)durationinMS);
			totalTime += (sint32)durationinMS;
		}
		else if( testResultList[i].state == TEST_STATE_FAILED )
		{
			allComplete = 0;
			drawText(-1, 1, 5+i, 0xFF808000, "FAILED");
		}
		else if( testResultList[i].state == TEST_STATE_QUEUED )
		{
			allComplete = 0;
			drawText(-1, 1, 5+i, 0x80808000, "QUEUED");
		}
		else if( testResultList[i].state == TEST_STATE_SKIPPED )
		{
			drawText(-1, 1, 5+i, 0x80FF8000, "SKIPPED");
		}
		else
		{
			allComplete = 0;
			drawText(-1, 1, 5+i, 0x8080FF00, "RUNNING");
		}
		_snprintf(text, sizeof(text), "%-11s %-11s", durationText, testResultList[i].name);
		drawText(-1, 1+12, 5+i, 0xFFFFFFFF, text);
	}
	// draw total time if all tests completed
	if( allComplete != 0 )
	{
		_snprintf(text, sizeof(text), "Total: %dms", totalTime);
		drawText(-1, 1, 5+testResultCount+2, 0xFFFFFFFF, text);
	}
	if( strlen(debugMessage) != 0 )
	{
		drawText(-1, 1, 5+testResultCount+5, 0xFF505000, "Debug Message:");
		drawText(-1, 1, 5+testResultCount+6, 0xFF505000, debugMessage);
	}
	OSScreenFlipBuffersEx(0);
	OSScreenFlipBuffersEx(1);
}

void drawResult()
{
	drawStatusScreen();
}

void queueTest(int(*testFunc)(), char* name)
{
	sint32 testIndex = testResultCount;
	testResultList[testIndex].name = name;
	testResultList[testIndex].testFunc = testFunc;
	testResultList[testIndex].tickStart = 0;
	testResultList[testIndex].tickEnd = 0;
	testResultList[testIndex].state = TEST_STATE_QUEUED;
	testResultCount++;
}

void runTest(int testIndex)
{
	testResultList[testIndex].state = TEST_STATE_RUNNING;
	drawStatusScreen();
	uint64 tickStart = OSGetTime();
	if( DEBUG_BENCHMARK == -1 || DEBUG_BENCHMARK == testIndex ) testResultList[testIndex].testFunc();
	uint64 tickEnd = OSGetTime();
	testResultList[testIndex].tickStart = tickStart;
	testResultList[testIndex].tickEnd = tickEnd;
	if( DEBUG_BENCHMARK == -1 || DEBUG_BENCHMARK == testIndex ) testResultList[testIndex].state = TEST_STATE_COMPLETED;
	else testResultList[testIndex].state = TEST_STATE_SKIPPED;
	drawStatusScreen();
}

int mainFunc(void)
{
	OSScreenInit();
	OSScreenSetBufferEx(0, (void*)0xF4000000);
	OSScreenSetBufferEx(1, (void*)(0xF4000000 + OSScreenGetBufferSizeEx(0)));
	OSScreenEnableEx(0, 1);
	OSScreenEnableEx(1, 1);
	OSScreenClearBufferEx(0, 0);
	OSScreenClearBufferEx(1, 0);
	OSScreenFlipBuffersEx(0);
	
	drawStatusScreen();
	
	fsBuffer = (unsigned char*)memAllocEx(BUFFER_SIZE, 64);
	int vpadError;
	VPADData vpadData;
	
	queueTest(test1_run, "ALU"); // arithmetic in a loop
	queueTest(test2_run, "AES128"); // AES128 compression
	queueTest(test3_run, "COPY"); // memory copy
	queueTest(test4_run, "RECUR"); // recursive functions
	queueTest(test5_run, "RANDGEN"); // fills 20mb buffer with random numbers
	queueTest(test6_run, "WRITE"); // write 20mb file
	queueTest(test7_run, "POINTER CHASING"); // create and chase a pointer array
	
	// run tests
	for(sint32 i=0; i<testResultCount; i++)
	{
		runTest(i);
	}
	while(!(vpadData.buttonTrigger&VPAD_BUTTON_A))
	{
		VPADRead(0, &vpadData, 1, &vpadError);
		drawResult();
		OSSleepTicks(MILLISECS_TO_TICKS(1000/30)); // aim for roughly 30 FPS
	}
	run_game(false);
	return 0;
}

unsigned char* fsBuffer = 0;