#include <string.h>
#include "main.h"

int (*OSDynLoad_Acquire)(const char* rpl, u32 *handle);
int (*OSDynLoad_FindExport)(u32 handle, int isdata, const char *symbol, void *address);

// coreinit OSScreen
void (*OSScreenInit)(void);
unsigned int (*OSScreenGetBufferSizeEx)(unsigned int bufferNum);
int (*OSScreenSetBufferEx)(unsigned int bufferNum, void * addr);
int (*OSScreenClearBufferEx)(unsigned int bufferNum, unsigned int temp);
int (*OSScreenFlipBuffersEx)(unsigned int bufferNum);
int (*OSScreenPutFontEx)(unsigned int bufferNum, unsigned int posX, unsigned int posY, const char * buffer);
void (*OSScreenPutPixelEx)(int bufferNum, uint32 posX, uint32 posY, uint32 color);
int (*OSScreenEnableEx)(unsigned int bufferNum, int enable);
// coreinit memory
void* (*MEMAllocFromDefaultHeapEx)(uint32_t size, int alignment);
void (*MEMFreeToDefaultHeap)(void* addr);

// coreinit time
u64  (*OSGetTime)(void);
void (*OSSleepTicks)(u64 ticks);

// coreinit misc
int (*_snprintf)(char *buf, int n, const char *format, ... );

// coreinit filesystem
void (*FSInit)(void);
int (*FSAddClient)(void *client, int flag);
void (*FSInitCmdBlock)(void *block);
int (*FSWriteFile)(void *client, void *block, const void *source, int blockSize, int blockCount, int fileHandle, int flag, int error);

// nn_save
void (*SAVEInit)(void);
void (*SAVEInitSaveDir)(unsigned char accountSlot);
int (*SAVEOpenFile)(void *client, void *block, unsigned char accountSlot, const char *path, const char *mode, int *fileHandle, int errHandling);

// vpad
void (*VPADBASEInit)(void);
int (*VPADRead)(int padnum, void/*VPADData*/ *buffer, int num_datasets, int *err);

// nsysnet
void (*socket_lib_init)(void);
int (*socket)(int domain, int type, int protocol);
int (*socketclose)(int s);
int (*setsockopt)(int s, int level, int optname, void *optval, int optlen);
int (*bind)(s32 s, void *name, s32 namelen);
int (*recv)(int s, void *buffer, int size, int flags);
int (*recvfrom)(int s, void *buffer, int size, int flags, void *src_addr, int srclen);
int (*send)(int s, const void *buffer, int size, int flags);
int (*sendto)(int s, const void *buffer, int size, int flags, void *dest_addr, int destlen);

int (*inet_pton)(int af, const char *src, void *dst);


int __entry_menu(int argc, char **argv)
{	
	// function offsets setup by Homebrew Launcher:
	// 0x00801500 -> OSDynLoad_Acquire
	// 0x00801504 -> OSDynLoad_FindExport
	
	OSDynLoad_Acquire = (int(*)(const char*, u32*))*(u32*)0x00801500;
	OSDynLoad_FindExport = (int (*)(u32, int, const char *, void*))*(u32*)0x00801504;
	u32 coreinitHandle;
	u32 nn_saveHandle;
	u32 vpadHandle;
	u32 netHandle;
	OSDynLoad_Acquire("coreinit", &coreinitHandle);
	OSDynLoad_Acquire("nn_save", &nn_saveHandle);
	OSDynLoad_Acquire("vpad", &vpadHandle);
	OSDynLoad_Acquire("nsysnet", &netHandle);
	
	// coreinit OSScreen
	OSDynLoad_FindExport(coreinitHandle, 0, "OSScreenInit", (void*)&OSScreenInit);
	OSDynLoad_FindExport(coreinitHandle, 0, "OSScreenGetBufferSizeEx", (void*)&OSScreenGetBufferSizeEx);
	OSDynLoad_FindExport(coreinitHandle, 0, "OSScreenSetBufferEx", (void*)&OSScreenSetBufferEx);
	OSDynLoad_FindExport(coreinitHandle, 0, "OSScreenClearBufferEx", (void*)&OSScreenClearBufferEx);
	OSDynLoad_FindExport(coreinitHandle, 0, "OSScreenFlipBuffersEx", (void*)&OSScreenFlipBuffersEx);
	OSDynLoad_FindExport(coreinitHandle, 0, "OSScreenPutFontEx", (void*)&OSScreenPutFontEx);
	OSDynLoad_FindExport(coreinitHandle, 0, "OSScreenPutPixelEx", (void*)&OSScreenPutPixelEx);
	OSDynLoad_FindExport(coreinitHandle, 0, "OSScreenEnableEx", (void*)&OSScreenEnableEx);
	// coreinit time
	OSDynLoad_FindExport(coreinitHandle, 0, "OSGetTime", (void*)&OSGetTime);
	// coreinit thread
	OSDynLoad_FindExport(coreinitHandle, 0, "OSSleepTicks", (void*)&OSSleepTicks);
	// coreinit memory
	void* temp;
	OSDynLoad_FindExport(coreinitHandle, 1, "MEMAllocFromDefaultHeapEx", (void*)&temp);
	*(void**)&MEMAllocFromDefaultHeapEx = *(void**)temp;
	OSDynLoad_FindExport(coreinitHandle, 1, "MEMFreeToDefaultHeap", (void*)&temp);
	*(void**)&MEMFreeToDefaultHeap = *(void**)temp;
	// coreinit misc
	OSDynLoad_FindExport(coreinitHandle, 0, "__os_snprintf", (void*)&_snprintf);
	// coreinit filesystem
	OSDynLoad_FindExport(coreinitHandle, 0, "FSInit", (void*)&FSInit);
	OSDynLoad_FindExport(coreinitHandle, 0, "FSAddClient", (void*)&FSAddClient);
	OSDynLoad_FindExport(coreinitHandle, 0, "FSInitCmdBlock", (void*)&FSInitCmdBlock);
	OSDynLoad_FindExport(coreinitHandle, 0, "FSWriteFile", (void*)&FSWriteFile);
	
	// nn_save
	OSDynLoad_FindExport(nn_saveHandle, 0, "SAVEOpenFile", (void*)&SAVEOpenFile);
	OSDynLoad_FindExport(nn_saveHandle, 0, "SAVEInit", (void*)&SAVEInit);
	OSDynLoad_FindExport(nn_saveHandle, 0, "SAVEInitSaveDir", (void*)&SAVEInitSaveDir);
	
	// vpad
	OSDynLoad_FindExport(vpadHandle, 0, "VPADBASEInit", (void*)&VPADBASEInit);
	OSDynLoad_FindExport(vpadHandle, 0, "VPADRead", (void*)&VPADRead);
	VPADBASEInit();
	
	// nsysnet
	OSDynLoad_FindExport(netHandle, 0, "socket_lib_init", (void*)&socket_lib_init);
	
	OSDynLoad_FindExport(netHandle, 0, "socket", (void*)&socket);
	OSDynLoad_FindExport(netHandle, 0, "socketclose", (void*)&socketclose);
	OSDynLoad_FindExport(netHandle, 0, "setsockopt", (void*)&setsockopt);
	OSDynLoad_FindExport(netHandle, 0, "bind", (void*)&bind);
	OSDynLoad_FindExport(netHandle, 0, "inet_pton", (void*)&inet_pton);

	OSDynLoad_FindExport(netHandle, 0, "recv", (void*)&recv);
	OSDynLoad_FindExport(netHandle, 0, "recvfrom", (void*)&recvfrom);
	OSDynLoad_FindExport(netHandle, 0, "send", (void*)&send);
	OSDynLoad_FindExport(netHandle, 0, "sendto", (void*)&sendto);
	
    int ret = mainFunc();
    return ret;
}

void* memAlloc(int size)
{
	return MEMAllocFromDefaultHeapEx(size, 16);
}

void* memAllocEx(int size, int alignment)
{
    return MEMAllocFromDefaultHeapEx(size, alignment);
}

void memFree(void* ptr)
{
	MEMFreeToDefaultHeap(ptr);
}

char debugMessage[256];