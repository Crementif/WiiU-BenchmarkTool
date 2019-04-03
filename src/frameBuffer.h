// frame buffer

u8* frameBufferFrontPtr = (uint8*)0xF4000000 + (0*(1280*720*4));
u8* frameBufferBackPtr = (uint8*)0xF4000000 + (1*(1280*720*4));
u32* frameBufferHexPtr;

u8* gamepadframeBufferFrontPtr;
u8* gamepadframeBufferBackPtr;
u32* gamepadframeBufferHexPtr;

bool frontOrBackBuffer = false;

void finishFrame()
{
	frontOrBackBuffer = !frontOrBackBuffer;
	if (frontOrBackBuffer == true) {
		frameBufferHexPtr = (u32*)frameBufferFrontPtr;
		gamepadframeBufferHexPtr = (u32*)gamepadframeBufferFrontPtr;
	}
	else if (frontOrBackBuffer == false) {
		frameBufferHexPtr = (u32*)frameBufferBackPtr;
		gamepadframeBufferHexPtr = (u32*)gamepadframeBufferBackPtr;
	}
	OSScreenFlipBuffersEx(0);
	OSScreenFlipBuffersEx(1);
}

void setPixel(sint32 bufferNum, uint32 x, uint32 y, uint32 color)
{
	if (bufferNum == 0) frameBufferHexPtr[x+y*1280] = color;
	else gamepadframeBufferHexPtr[x+y*896] = color;
}