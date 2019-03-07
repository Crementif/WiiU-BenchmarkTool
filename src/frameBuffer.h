// frame buffer

uint8* frameBufferFrontPtr = (uint8*)0xF4000000 + (0*(1280*720*4));
uint8* frameBufferBackPtr = (uint8*)0xF4000000 + (1*(1280*720*4));
uint32* frameBufferHexPtr;

uint8* gamepadframeBufferFrontPtr;
uint8* gamepadframeBufferBackPtr;
uint32* gamepadframeBufferHexPtr;

bool frontOrBackBuffer = false;

void finishFrame()
{
	frontOrBackBuffer = !frontOrBackBuffer;
	if (frontOrBackBuffer == true) {
		frameBufferHexPtr = (uint32*)frameBufferFrontPtr;
		gamepadframeBufferHexPtr = (uint32*)gamepadframeBufferFrontPtr;
	}
	else if (frontOrBackBuffer == false) {
		frameBufferHexPtr = (uint32*)frameBufferBackPtr;
		gamepadframeBufferHexPtr = (uint32*)gamepadframeBufferBackPtr;
	}
	OSScreenFlipBuffersEx(0);
	OSScreenFlipBuffersEx(1);
}

void setPixel(sint32 bufferNum, uint32 x, uint32 y, uint32 color)
{
	if (bufferNum == 0) frameBufferHexPtr[x+y*1280] = color;
	else gamepadframeBufferHexPtr[x+y*896] = color;
}