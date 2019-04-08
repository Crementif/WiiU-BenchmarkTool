#include <stdlib.h>
#include "objects.h"
#include "gamepad.h"

int scrollPixelOffset = 0;

int blockQueueLength = 0;
obstacleType blockQueueType;
obstacleType lastBlockType;

void drawSprite(s8 bufferNum, u32 spritePtr[], int x, int y, int spriteWidth, int spriteHeight, int size) {
	if (bufferNum < 0) {
		drawSprite(0, spritePtr, x, y, spriteHeight, spriteWidth, size);
		drawSprite(1, spritePtr, x, y, spriteHeight, spriteWidth, size);
		return;
	}
	u32 *bufferPtr = bufferNum==0? frameBufferHexPtr : gamepadframeBufferHexPtr;
	int bufferWidth = bufferNum==0? 1280 : 720;
	for (int py = 0; py < spriteHeight*size; py++) {
		for (int px = 0; px < spriteWidth*size; px++) {
			if (x+px < 0 || x+px >= 1280) return;
			bufferPtr[(x+px)+((y+py)*bufferWidth)] = spritePtr[(px/size)+((py/size)*spriteWidth)];
		}
	}
}

void drawTiledSprite(int x, int y, u32 spritePtr[], int tiledBlockWidth, int tiledBlockHeight, int size) {
	for (int px=0; px<tiledBlockWidth; px+=(size*16)) {
		for (int py=0; py<tiledBlockHeight; py+=(size*18)) {
			drawSprite(0, spritePtr, px+x-scrollPixelOffset, py+y, 16, 18, size);
		}
	}
}

void drawSolidBlock(int gridX, int gridY, u32 color) {
	int startX = gridX-scrollPixelOffset;
	if (startX < 0) startX = 0;
	int endX = startX + BLOCK_PIXEL_WIDTH;
	if (endX >= 1280) endX = 1280;
	for (int y=0; y<BLOCK_PIXEL_HEIGHT; y++) {
		for (int x=startX; x<endX; x++) {
			frameBufferHexPtr[(x)+(y*1280+gridY*1280)] = color;
		}
	}
}

void insertHighPlatform() {
	for (int y=0; y<STAGE_HEIGHT; y++) {
		if (y < 6) gridLayout[STAGE_WIDTH][y] = Air;
		else gridLayout[STAGE_WIDTH][y] = PotatoStarch;
	}
	if (blockQueueLength == 5) blockQueueLength = 0;
}

void insertLowPlatform() {
	for (int y=0; y<STAGE_HEIGHT; y++) {
		if (y < 8) gridLayout[STAGE_WIDTH][y] = Air;
		else gridLayout[STAGE_WIDTH][y] = Garbage;
	}
	if (blockQueueLength == 5) blockQueueLength = 0;
}

void insertGapPlatform() {
	for (int y=0; y<STAGE_HEIGHT; y++) {
		blockType newLine = Air;
		if (blockQueueLength == 2 && blockQueueLength == 3) newLine = Potato;
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
					drawSolidBlock(BLOCK_PIXEL_WIDTH*x, BLOCK_PIXEL_HEIGHT*y, 0x38333300);
					break;
				case Garbage:
					drawTiledSprite(BLOCK_PIXEL_WIDTH*x, BLOCK_PIXEL_HEIGHT*y, compressedtrash_block, BLOCK_PIXEL_WIDTH, BLOCK_PIXEL_HEIGHT, 3);
					break;
				case Potato:
					drawTiledSprite(BLOCK_PIXEL_WIDTH*x, BLOCK_PIXEL_HEIGHT*y, potato_block, BLOCK_PIXEL_WIDTH, BLOCK_PIXEL_HEIGHT, 2);
					break;
				case PotatoStarch:
					drawTiledSprite(BLOCK_PIXEL_WIDTH*x, BLOCK_PIXEL_HEIGHT*y, potatostarch_block, BLOCK_PIXEL_WIDTH, BLOCK_PIXEL_HEIGHT, 1);
					break;
				case Air:
					drawSolidBlock(BLOCK_PIXEL_WIDTH*x, BLOCK_PIXEL_HEIGHT*y, 0x314a7200);
					break;
				default:
					drawSolidBlock(BLOCK_PIXEL_WIDTH*x, BLOCK_PIXEL_HEIGHT*y, 0xff005900);
					break;
			}
		}
	}
}

void createVerticalLine() {
	scrollPixelOffset = 0;
	gameState.subPixelScrollOffset = 0;
	// Shift grid to left
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

void renderGame(VPADData* controller) {
	if (!gameState.hostCollided) gameState.subPixelScrollOffset += gameState.scrollSpeed;
	scrollPixelOffset = (int)gameState.subPixelScrollOffset;
	if (scrollPixelOffset >= BLOCK_PIXEL_WIDTH) {
		createVerticalLine();
	}
	drawGrid();
	renderPlayer(controller);
}