#include "vpad.h"
#include "sprites.h"

#define STAGE_HEIGHT (9)
#define STAGE_WIDTH (16)

#define BLOCK_PIXEL_HEIGHT (720/STAGE_HEIGHT)
#define BLOCK_PIXEL_WIDTH (1280/STAGE_WIDTH)

#define PLAYER_ARRAY_HEIGHT (31)
#define PLAYER_ARRAY_WIDTH (21)
#define PLAYER_SCALE (4)

#define PLAYER_FEET_HEIGHT (12)
#define PLAYER_PIXEL_HEIGHT (PLAYER_FEET_HEIGHT+PLAYER_ARRAY_HEIGHT*PLAYER_SCALE)
#define RUNNER_X_POS (BLOCK_PIXEL_WIDTH*2)

typedef enum {
	Air,
	Block,
	Potato,
	Platform
} blockType;

typedef enum {
	HighPlatform,
	LowPlatform,
	GapPlatform,
} obstacleType;

blockType gridLayout[STAGE_WIDTH+1][STAGE_HEIGHT];

int previousBottomYPos = BLOCK_PIXEL_HEIGHT*(STAGE_HEIGHT-1)-(PLAYER_PIXEL_HEIGHT*2);

int upVelocity = 0;
int downVelocity = 0;
int gravity = 2;
int legsFrame = 0;

const char* playerHeads[] = {"Bignose", "Exzap", "Cellhunter", "Petergov", "Zabivaka", "Chriztr", "Rich Evans"};
int headUsed = 0;

void drawPlayer(int drawYPos) {
	int arrayIndex = 0;
	for (int y=0; y<PLAYER_ARRAY_HEIGHT*PLAYER_SCALE; y+=PLAYER_SCALE) {
		for (int x=0; x<PLAYER_ARRAY_WIDTH*PLAYER_SCALE; x+=PLAYER_SCALE) {
			uint32 color = 0x00000000;
			
			if (trashcan_body_sprite[arrayIndex] == 0x00) {
				if (y/PLAYER_SCALE >= 2 && y/PLAYER_SCALE <= 12) {
					switch(headUsed) {
						case 0:
							color = bignose_head_sprite[arrayIndex-(PLAYER_ARRAY_WIDTH*2)];
							break;
						case 1:
							color = exzap_head_sprite[arrayIndex-(PLAYER_ARRAY_WIDTH*2)];
							break;
						case 2:
							color = cellhunter_head_sprite[arrayIndex-(PLAYER_ARRAY_WIDTH*2)];
							break;
						case 3:
							color = petergov_head_sprite[arrayIndex-(PLAYER_ARRAY_WIDTH*2)];
							break;
						case 4:
							color = zabivaka_head_sprite[arrayIndex-(PLAYER_ARRAY_WIDTH*2)];
							break;
						case 5:
							color = chriztr_head_sprite[arrayIndex-(PLAYER_ARRAY_WIDTH*2)];
							break;
						case 6:
							color = richevans_head_sprite[arrayIndex-(PLAYER_ARRAY_WIDTH*2)];
							break;
					}
					if (color == 0x00000000) {
						arrayIndex++;
						continue;
					};
				}
				else {
					arrayIndex++;
					continue;
				}
			}
			else color = trashcanPalette[trashcan_body_sprite[arrayIndex]-1];
			
			for (int scaleX=0; scaleX<PLAYER_SCALE; scaleX++) {
				for (int scaleY=0; scaleY<PLAYER_SCALE; scaleY++) {
					if ((RUNNER_X_POS+x+scaleX >= 0 && RUNNER_X_POS+x+scaleX <= 1280) && (drawYPos+y+scaleY >= 0 && drawYPos+y+scaleY <= 720)) {
						frameBufferHexPtr[(RUNNER_X_POS+x+scaleX)+((drawYPos+y+scaleY)*1280)] = color;
					}
				}
			}
			arrayIndex++;
		}
	}
	#define LEG_Y_START (RUNNER_X_POS+8*PLAYER_SCALE)
	int LEG_X_START = drawYPos+PLAYER_ARRAY_HEIGHT*PLAYER_SCALE;
	for (int leg=0; leg<2; leg++) {
		for (int i=LEG_X_START; i<LEG_X_START+PLAYER_FEET_HEIGHT+legsFrame/50-leg; i++) {
			frameBufferHexPtr[(LEG_Y_START+leg*10)+i*1280] = 0x6e6e6e00;
			frameBufferHexPtr[(1+LEG_Y_START+leg*10)+i*1280] = 0x6e6e6e00;
			frameBufferHexPtr[(2+LEG_Y_START+leg*10)+i*1280] = 0x6e6e6e00;
		}
	}
	if (legsFrame>=250) legsFrame = 0;
}

bool renderPlayer(VPADData* controller) {
	int newBottomYPos = previousBottomYPos;
	
	int lowestPoint = BLOCK_PIXEL_HEIGHT*STAGE_HEIGHT;
	for (int y=0; y<STAGE_HEIGHT; y++) { // Calculate height from ground
		if (gridLayout[RUNNER_X_POS/BLOCK_PIXEL_WIDTH][y] == Block || gridLayout[RUNNER_X_POS/BLOCK_PIXEL_WIDTH+1][y] == Block) {
			lowestPoint = BLOCK_PIXEL_HEIGHT*y;
			break;
		}
	}
	
	if (controller->buttonTrigger&VPAD_BUTTON_A && lowestPoint-previousBottomYPos == 0) {
		upVelocity = 55;
	}
	upVelocity -= 2;
	if (upVelocity < 0) {
		upVelocity = 0;
		legsFrame+=10;
	}
	
	if (lowestPoint-previousBottomYPos > 0) {
		downVelocity += gravity;
	}
	else if (lowestPoint-previousBottomYPos == 0) {
		downVelocity = 0;
	}
	else {
		return true;
	}
	if (previousBottomYPos == 720) {
		return true;
	}
	if (downVelocity > lowestPoint-previousBottomYPos) newBottomYPos += lowestPoint-previousBottomYPos;
	else newBottomYPos += downVelocity;
	newBottomYPos -= upVelocity;
	previousBottomYPos = newBottomYPos;
	drawPlayer(newBottomYPos-PLAYER_PIXEL_HEIGHT);
	return false;
}