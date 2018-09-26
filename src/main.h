#include <gctypes.h>
#include <string.h>

typedef unsigned long long uint64;
typedef signed long long sint64;
typedef unsigned int uint32;
typedef signed int sint32;
typedef unsigned short uint16;
typedef signed short sint16;
typedef unsigned char uint8;
typedef signed char sint8;

void* memAlloc(int size);
void* memAllocEx(int size, int alignment);
void memFree(void* ptr);

int mainFunc(void);

#define BUS_SPEED                       248625000
#define SECS_TO_TICKS(sec)              (((unsigned long long)(sec)) * (BUS_SPEED/4))
#define MILLISECS_TO_TICKS(msec)        (SECS_TO_TICKS(msec) / 1000)
#define MICROSECS_TO_TICKS(usec)        (SECS_TO_TICKS(usec) / 1000000)

#define TICK_SPEED						(BUS_SPEED/4)

// dynload functions
extern int (*OSDynLoad_Acquire)(const char* rpl, u32 *handle);
extern int (*OSDynLoad_FindExport)(u32 handle, int isdata, const char *symbol, void *address);

// coreinit OSScreen functions
extern void (*OSScreenInit)(void);
extern unsigned int (*OSScreenGetBufferSizeEx)(unsigned int bufferNum);
extern int (*OSScreenSetBufferEx)(unsigned int bufferNum, void * addr);
extern int (*OSScreenClearBufferEx)(unsigned int bufferNum, unsigned int temp);
extern int (*OSScreenFlipBuffersEx)(unsigned int bufferNum);
extern int (*OSScreenPutFontEx)(unsigned int bufferNum, unsigned int posX, unsigned int posY, const char * buffer);
extern void (*OSScreenPutPixelEx)(int bufferNum, uint32 posX, uint32 posY, uint32 color);
extern int (*OSScreenEnableEx)(unsigned int bufferNum, int enable);

// coreinit memory
void* (*MEMAllocFromDefaultHeapEx)(uint32_t size, int alignment);

// coreinit time functions
extern u64 (*OSGetTime)(void);

// coreinit thread
extern void (*OSSleepTicks)(u64 ticks);

// coreinit misc
extern int (*_snprintf)(char *buf, int n, const char *format, ... );

// coreinit filesystem
extern void (*FSInit)(void);
extern int (*FSAddClient)(void *client, int flag);
extern void (*FSInitCmdBlock)(void *block);
extern int (*FSWriteFile)(void *client, void *block, const void *source, int blockSize, int blockCount, int fileHandle, int flag, int error);

// nn_save
extern void (*SAVEInit)(void);
extern void (*SAVEInitSaveDir)(unsigned char accountSlot);
extern int (*SAVEOpenFile)(void *client, void *block, unsigned char accountSlot, const char *path, const char *mode, int *fileHandle, int errHandling);


// drawing
void drawText(sint32 bufferNum, sint32 x, sint32 y, uint32 color, char* str);
void drawHorizontalLineW2(sint32 bufferNum, sint32 x, sint32 y, sint32 width, uint32 color);
void setDebugMessage(char* str);
void setDebugMessageInt(int number);
extern char debugMessage[256];

// buffer size for random and write test
#define BUFFER_SIZE						20000000
extern long ptr_fsBuffer;

