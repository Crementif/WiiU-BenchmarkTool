#include <stdlib.h>
#include "objects.h"

#define GRID_DEBUG (0)

uint64 randInt(uint64 seed);
uint64 levelSeed = 0;
uint64 blocksTraversed = 0;
int score = 0;

// Handle transitions between the "full" states (where exactly 16 blocks are present)
int scrollPixelOffset = 0;
float scrollSubpixelOffset = 0;

int blockQueueLength = 0;
obstacleType blockQueueType;
obstacleType lastBlockType;

void drawBlock(int gridX, int gridY, blockType blockDrawType) {
	for (int y=0; y<BLOCK_PIXEL_HEIGHT-GRID_DEBUG; y++) {
		if (gridY+y < 0 || gridY+y > 720) continue;
		for (int x=0; x<BLOCK_PIXEL_WIDTH-GRID_DEBUG; x++) {
			if (gridX+x-scrollPixelOffset < 0 || gridX+x-scrollPixelOffset > 1280) continue;
			uint32 color = 0xf200ff00;
			if (blockDrawType == Air) color = 0xefefef00;
			else if (blockDrawType == Block) color = 0x38333300;
			frameBufferHexPtr[(gridX+x-scrollPixelOffset)+((gridY+y)*1280)] = color;
		}
	}
}

void insertHighPlatform() {
	for (int y=0; y<STAGE_HEIGHT; y++) {
		if (y < 6) gridLayout[STAGE_WIDTH][y] = Air;
		else gridLayout[STAGE_WIDTH][y] = Block;
	}
	if (blockQueueLength == 5) blockQueueLength = 0;
}

void insertLowPlatform() {
	for (int y=0; y<STAGE_HEIGHT; y++) {
		if (y < 8) gridLayout[STAGE_WIDTH][y] = Air;
		else gridLayout[STAGE_WIDTH][y] = Block;
	}
	if (blockQueueLength == 5) blockQueueLength = 0;
}

void insertGapPlatform() {
	for (int y=0; y<STAGE_HEIGHT; y++) {
		blockType newLine = Air;
		if (blockQueueLength == 2 && blockQueueLength == 3) newLine = Block;
		if (y < 8) gridLayout[STAGE_WIDTH][y] = Air;
		else gridLayout[STAGE_WIDTH][y] = newLine;
	}
	if (blockQueueLength == 5) blockQueueLength = 0;
}

void generateObstacleLine() {
	if (blockQueueLength == 0) {
		int extraSeed = 0;
		while (blockQueueType == lastBlockType) {
			blockQueueType = randInt(levelSeed+blocksTraversed+extraSeed) % (2 + 1);
			extraSeed++;
		}
		lastBlockType = blockQueueType;
	}
	blockQueueLength++;
	switch(blockQueueType) {
		case HighPlatform:
			insertHighPlatform();
			break;
		case LowPlatform:
			insertLowPlatform();
			break;
		case GapPlatform:
			insertGapPlatform();
			break;
	}
	score++;
	blocksTraversed++;
}

void initializeLevelGrid() {
	for (int x=0; x<STAGE_WIDTH+1; x++) {
		for (int y=0; y<STAGE_HEIGHT; y++) {
			if (y==STAGE_HEIGHT-1) gridLayout[x][y]=Block;
			else gridLayout[x][y]=Air;
		}
	}
}

void drawGrid() {
	for (int x=0; x<STAGE_WIDTH+1; x++) {
		for (int y=0; y<STAGE_HEIGHT; y++) {
			switch(gridLayout[x][y]) {
				case Block:
					drawBlock(BLOCK_PIXEL_WIDTH*x, BLOCK_PIXEL_HEIGHT*y, gridLayout[x][y]);
					break;
				default:
					drawBlock(BLOCK_PIXEL_WIDTH*x, BLOCK_PIXEL_HEIGHT*y, gridLayout[x][y]);
					break;
			}
		}
	}
}

void createVerticalLine() {
	// First, shift grid to left and reset the offsets
	scrollPixelOffset = 0;
	scrollSubpixelOffset = 0;
	for (int x=1; x<(STAGE_WIDTH+1); x++) {
		for (int y=0; y<STAGE_HEIGHT; y++) {
			gridLayout[x-1][y] = gridLayout[x][y];
		}
	}
	generateObstacleLine();
}

bool renderLevel(int backgroundScrollSpeed, VPADData* controller) {
	scrollSubpixelOffset+=backgroundScrollSpeed;
	scrollPixelOffset = (int)scrollSubpixelOffset;
	if (scrollPixelOffset >= BLOCK_PIXEL_WIDTH) {
		createVerticalLine();
	}
	drawGrid();
	return renderPlayer(controller);
}