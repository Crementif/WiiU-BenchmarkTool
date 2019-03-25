#include <stdlib.h>
#include "objects.h"

// Handle transitions between the "full" states
int scrollPixelOffset = 0;
float scrollSubpixelOffset = 0;

int blockQueueLength = 0;
obstacleType blockQueueType;
obstacleType lastBlockType;
u64 randInt(u64 seed);

void drawBlock(int gridX, int gridY, blockType blockDrawType) {
	for (int y=0; y<BLOCK_PIXEL_HEIGHT; y++) {
		if (gridY+y < 0 || gridY+y > 720) continue;
		for (int x=0; x<BLOCK_PIXEL_WIDTH; x++) {
			if (gridX+x-scrollPixelOffset < 0 || gridX+x-scrollPixelOffset > 1280) continue;
			u32 color = 0xf200ff00;
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
			blockQueueType = randInt(gameState.levelSeed+gameState.blocksTraversed+extraSeed) % (2 + 1);
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
	gameState.score++;
	gameState.blocksTraversed++;
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

void drawScore(unsigned char scoreXPos, unsigned char scoreYPos) {
	char scoreCharBuffer[] = "Score: 4294967295\0";
	itoa(gameState.score, scoreCharBuffer+7, 10);
	drawTextEx(0, scoreXPos, scoreYPos, 0xFF905000, scoreCharBuffer, false, 2, 1);
	if (currScreen != GAMEPLAY_CLIENT) {
		if (localHighscore < gameState.score) localHighscore = gameState.score;
		char highscoreCharBuffer[] = "Highscore: 4294967295\0";
		itoa(localHighscore, highscoreCharBuffer+11, 10);
		drawTextEx(0, scoreXPos, scoreYPos - 2, 0xFF905000, highscoreCharBuffer, false, 2, 1);
	}
}

void renderLevel(VPADData* controller) {
	if (!gameState.hostCollided) gameState.subPixelScrollOffset += gameState.scrollSpeed;
	scrollPixelOffset = (int)scrollSubpixelOffset;
	if (scrollPixelOffset >= BLOCK_PIXEL_WIDTH) {
		createVerticalLine();
	}
	drawGrid();
	renderPlayer(controller);
}