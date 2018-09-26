#include "main.h"
#include <stdlib.h>


// This program writes a 20mb buffer to the hard-disk to test performance.

#define FS_CLIENT_SIZE                  0x1700
#define FS_CMD_BLOCK_SIZE               0xA80

#define FS_STATUS_OK					0


int test6_run()
{
	void* fsClient = memAlloc(FS_CLIENT_SIZE);
	void* fsCmd = memAlloc(FS_CMD_BLOCK_SIZE);
	int fsHandle = -1;
	
	FSInit();
	FSAddClient(fsClient, -1);
	FSInitCmdBlock(fsCmd);
	
	SAVEInit();
	SAVEInitSaveDir(0xFF);
	SAVEOpenFile(fsClient, fsCmd, 0xFF, "write_benchmark.bin", "w+", &fsHandle, -1);
	
	FSWriteFile(fsClient, fsCmd, fsBuffer, BUFFER_SIZE, 1, fsHandle, 0, -1);
	
	memFree(fsBuffer);
	memFree(fsClient);
	memFree(fsCmd);
	
	return 0;
}