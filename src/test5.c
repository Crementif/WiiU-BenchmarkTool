#include "main.h"

// This program writes a 20mb file to the hard-disk to test performance.
#define FS_CLIENT_SIZE                  0x1700
#define FS_CMD_BLOCK_SIZE               0xA80

#define FS_STATUS_OK					0


int test5_run()
{
	void* fsClient = memAlloc(FS_CLIENT_SIZE);
    void* fsCmd = memAlloc(FS_CMD_BLOCK_SIZE);
	int fsHandle = -1;
	
	
	FSInit();
	
	FSInitCmdBlock(fsCmd);
	
	int fsOpenStatus = SAVEOpenFile(fsClient, fsCmd, "test5_benchmark.bin", "r+", &fsHandle, -1);
	if (fsOpenStatus != FS_STATUS_OK) return -1;
	//int NSSGetRandom(min, max);
	
	return 0;
}