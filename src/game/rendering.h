#include <stdlib.h>
#include "objects.h"

// Set to the amount of level blocks we have.
#define BLOCK_PIXEL_HEIGHT (720/STAGE_HEIGHT)
#define BLOCK_PIXEL_WIDTH (1280/STAGE_WIDTH)

uint64 randInt(uint64 seed);

void drawBlock(int blockType, int gridX, int gridY) {
	for (int y=0; y<BLOCK_PIXEL_HEIGHT; y++) {
		for (int x=0; x<BLOCK_PIXEL_WIDTH; x++) {
			uint32 color = 0xF4424200;
			if (blockType == 0) color=0xFF905000;
			OSScreenPutPixelEx(0, (gridX*BLOCK_PIXEL_WIDTH)+x, (gridY*BLOCK_PIXEL_HEIGHT)+y, color);
			OSScreenPutPixelEx(1, (gridX*BLOCK_PIXEL_WIDTH)+x, (gridY*BLOCK_PIXEL_HEIGHT)+y, color);
		}
	}
}

void drawGrid() {
	for (int y=0; y<STAGE_HEIGHT; y++) {
		for (int x=0; x<STAGE_WIDTH; x++) {
			drawBlock(levelStage[x][y], x, y);
		}
	}
}

void generateObstacle(int obstacle) {
	
}

void generateRandomObstacle(int minSize, int maxSize) {
	
}

void renderLevel(uint64 seed) {
	//uint64 chosenBlock = randInt(levelSeed) & (COUNT_LEVELBLOCK + 1 - 0);
	drawGrid();
}