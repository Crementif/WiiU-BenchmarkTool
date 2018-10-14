#include <stdlib.h>
#include "objects.h"

// Set to the amount of level blocks we have.
#define BLOCK_PIXEL_HEIGHT (720)
#define BLOCK_PIXEL_WIDTH (1280)

#define PICTURE_HEIGHT (629)
#define PICTURE_WIDTH (640)


uint64 randInt(uint64 seed);

void drawBlock() {
	int index = 0;
	for (int y=0; y<PICTURE_HEIGHT; y++) {
		for (int x=0; x<PICTURE_WIDTH; x++) {
			int color = 0x000000FF; // Black
			if (cakeImage[index/2] == 0x66) color = 0xF7674DFF; // Candle Red
			else if (cakeImage[index/2] == 0x77) color = 0xF6EA43FF; // Candle Yellow
			else if (cakeImage[index/2] == 0x11) color = 0x000000FF; // Black
			else if (cakeImage[index/2] == 0xaa) color = 0xFEF6C1FF; // Candle Yellow
			else if (cakeImage[index/2] == 0x55) color = 0xE9E9E9FF; // White
			else if (cakeImage[index/2] == 0x88) color = 0x00ADEFFF; // Top cake
			else if (cakeImage[index/2] == 0x99) color = 0x0191C6FF; // Bottom cake
			else color = 0xE9E9E9FF;
			
			
			OSScreenPutPixelEx(0, 350+x, y, color);
			OSScreenPutPixelEx(1, 350+x, y, color);
			index++;
		}
	}
}

void renderLevel(uint64 seed) {
	//uint64 chosenBlock = randInt(levelSeed) & (COUNT_LEVELBLOCK + 1 - 0);
	drawBlock();
}