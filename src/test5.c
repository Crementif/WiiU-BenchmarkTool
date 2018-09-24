#include "main.h"

// This program writes a 20mb buffer to the hard-disk to test performance.
#define BUFFER_SIZE						20000000

#define FS_CLIENT_SIZE                  0x1700
#define FS_CMD_BLOCK_SIZE               0xA80

#define FS_STATUS_OK					0


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
	void* fsClient = memAlloc(FS_CLIENT_SIZE);
	void* fsCmd = memAlloc(FS_CMD_BLOCK_SIZE);
	unsigned char* fsBuffer = (unsigned char*)memAllocEx(BUFFER_SIZE, 64);
	int fsHandle = -1;
	
	FSInit();
	FSAddClient(fsClient, -1);
	FSInitCmdBlock(fsCmd);
	
	SAVEInit();
	SAVEInitSaveDir(0xFF);
	int saveStatus = SAVEOpenFile(fsClient, fsCmd, 0xFF, "write_benchmark.bin", "w+", &fsHandle, -1);
	
	randState = OSGetTime();
	
	setDebugMessageInt(randInt());
	
	int writeStatus = FSWriteFile(fsClient, fsCmd, fsBuffer, BUFFER_SIZE, 1, fsHandle, 0, -1);
	
	return 0;
}