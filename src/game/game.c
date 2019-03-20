#include <stdlib.h>
#include <time.h>
#include "..\frameBuffer.h"
#include "game.h"
#include "rendering.h"

int vpadError;
VPADData vpadData;
void switchScreen(unsigned char toScreen);
u64 randInt(u64 seed);

void drawIntro(bool debugMode) {
	drawTextEx(0, 65, 20, 0xF4424200, "Powered by the Trash bites! engine", true, 4, (int)(4.0-animationTimestamp)+1);
	if (animationTimestamp > 4 || debugMode) switchScreen(MAIN_MENU);
}

void drawGameOver() {
	if (localHighscore < gameState.score) localHighscore = gameState.score;
	drawTextEx(0, 72, 10, 0xF4424200, "Trash canned!", true, 5, 1);
	drawTextEx(0, 70, 40, 0xF4424200, "Press A to restart", true, 1, 1);
	drawTextEx(0, 65, 42, 0xF4424200, "Press B to go to the main menu", true, 1, 1);
	drawScore(72, 20);
	if (vpadData.buttonTrigger&VPAD_BUTTON_A || vpadData.buttonTrigger&VPAD_BUTTON_B) {
		if (vpadData.buttonTrigger&VPAD_BUTTON_A) {
			switchScreen(currScreen);
		}
		if (vpadData.buttonTrigger&VPAD_BUTTON_B) switchScreen(MAIN_MENU);
	}
}

void drawMenu() {
	if (animationTimestamp/5.0 <= 0.25) drawTextEx(0, 72, 1, 0xFF905000, ">>> Trash Run <<<", true, 4, 1);
	else if (animationTimestamp/5.0 <= 0.5) drawTextEx(0, 72, 1, 0xFF905000, " >> Trash Run << ", true, 4, 1);
	else if (animationTimestamp/5.0 <= 0.75) drawTextEx(0, 72, 1, 0xFF905000, "  > Trash Run <  ", true, 4, 1);
	else if (animationTimestamp/5.0 <= 1.0) drawTextEx(0, 72, 1, 0xFF905000, "    Trash Run    ", true, 4, 1);
	
	drawTextEx(0, 75, 20, ((selectedOption==0 && (int)animationTimestamp%2==0)? OPTION_SELECTED : OPTION), "LOCAL PLAY", true, 2, 1);
	drawTextEx(0, 78, 22, ((selectedOption==1 && (int)animationTimestamp%2==0)? OPTION_SELECTED : OPTION), "JOIN", true, 2, 1);
	drawTextEx(0, 78, 24, ((selectedOption==2 && (int)animationTimestamp%2==0)? OPTION_SELECTED : OPTION), "HOST", true, 2, 1);
	drawTextEx(0, 77, 26, ((selectedOption==3 && (int)animationTimestamp%2==0)? OPTION_SELECTED : OPTION), "OPTIONS", true, 2, 1);
	
	if (vpadData.buttonTrigger&VPAD_BUTTON_UP && selectedOption >= 1) selectedOption--;
	if (vpadData.buttonTrigger&VPAD_BUTTON_DOWN && selectedOption <= 2) selectedOption++;
	
	if (vpadData.buttonTrigger&VPAD_BUTTON_A) {
		if (selectedOption == 0) switchScreen(GAMEPLAY_LOCAL);
		else if (selectedOption == 1) switchScreen(GAMEPLAY_CLIENT);
		else if (selectedOption == 2) switchScreen(GAMEPLAY_HOST);
		else if (selectedOption == 3) switchScreen(OPTION_MENU);
	}
}

char headOptionCharBuffer[27] = "Head - Rich Evans";
void drawOptionMenu() {
	if (vpadData.buttonTrigger&VPAD_BUTTON_UP) {
		if (selectedOption > 0) selectedOption--;
	}
	else if (vpadData.buttonTrigger&VPAD_BUTTON_DOWN) {
		if (selectedOption < 1) selectedOption--;
	}
	
	if (selectedOption == 0) {
		if (vpadData.buttonTrigger&VPAD_BUTTON_LEFT && gameState.infectionLevel >= 1) gameState.infectionLevel--;
		if (vpadData.buttonTrigger&VPAD_BUTTON_RIGHT && gameState.infectionLevel <= 5) gameState.infectionLevel++;
		memmove(headOptionCharBuffer+7, playerHeads[gameState.infectionLevel], 20);
	}
	if (vpadData.buttonTrigger&VPAD_BUTTON_B) switchScreen(MAIN_MENU);

	drawTextEx(0, 72, 22, 0xFF905000, headOptionCharBuffer, true, 2, 1);
	drawTextEx(0, 72, 24, 0xFF905000, "Speed - ", true, 2, 1);
	drawTextEx(0, 65, 42, 0xF4424200, "Press B to go to the main menu", true, 1, 1);
}

void drawGame() {
	if (currScreen == GAMEPLAY_HOST || currScreen == GAMEPLAY_CLIENT) if (!establishConnection()) return;
	
	if (!gameState.hostCollided) {
		renderLevel(&vpadData);
		drawScore(10, 3);

		if (animationTimestamp < 1) drawTextEx(0, 72, 3, 0xFF905000, "3", false, 2, 2);
		else if (animationTimestamp < 2) drawTextEx(0, 72, 3, 0xFF905000, "2", false, 2, 2);
		else if (animationTimestamp < 3) drawTextEx(0, 72, 3, 0xFF905000, "1", false, 2, 2);
		else if (animationTimestamp < 4) {
			drawTextEx(0, 72, 3, 0xFF905000, "GO", false, 2, 2);
			gameState.scrollSpeed = 20;
		}
	}
	else drawGameOver();
}

void switchScreen(unsigned char toScreen) {
	animationTimestamp = 0;
	animationStartTimestamp = OSGetTime();
	selectedOption = 0;
	switch (toScreen) {
		case MAIN_MENU:
			if (currScreen == GAMEPLAY_HOST || currScreen == GAMEPLAY_CLIENT) {
				socketclose(client_socket);
				client_socket = -1;
			}
			loopAnimation = true;
			animationLength = 5;
			break;
		case OPTION_MENU:
			loopAnimation = true;
			animationLength = 2;
			break;
		case GAMEPLAY_HOST:
		case GAMEPLAY_CLIENT:
			if (!initializeSocket()) {
				// If initialization failed, that status should be kept on screen.
				while (true) {}
			}
			/* FALLTHROUGH */
		case GAMEPLAY_LOCAL:
			gameState = (struct gameStateStruct) {
				.levelSeed = OSGetTime(),
				.infectionLevel = selectedInfection,
				.scrollSpeed = 0,
				.health = 100,
				.score = 0,
				.hostCollided = false,
				.yPosition = BLOCK_PIXEL_HEIGHT * (STAGE_HEIGHT - 1) - (PLAYER_PIXEL_HEIGHT * 2),
				.upVelocity = 0,
				.downVelocity = 0,
				.blocksTraversed = 0,
				.subPixelScrollOffset = 0,
			};
			// Clear grid
			for (int x = 0; x<STAGE_WIDTH + 1; x++) {
				for (int y = 0; y<STAGE_HEIGHT; y++) {
					if (y == STAGE_HEIGHT - 1) gridLayout[x][y] = Block;
					else gridLayout[x][y] = Air;
				}
			}
			loopAnimation = false;
			animationLength = 3;
			break;
	}
	currScreen = toScreen;
}

void run_game(bool debugMode)
{
	socket_lib_init();
	animationStartTimestamp = OSGetTime();
	while(true) {
		OSScreenClearBufferEx(0, 0);
		OSScreenClearBufferEx(1, 0);
		u64 currTimestamp = OSGetTime();
		animationTimestamp = TICKS_TO_SECS(currTimestamp - animationStartTimestamp);
		if (loopAnimation && (animationTimestamp > animationLength)) {
			animationTimestamp = 0;
			animationStartTimestamp = currTimestamp;
		}
		VPADRead(0, &vpadData, 1, &vpadError);
		if (currScreen == INTRO) drawIntro(debugMode);
		if (currScreen == MAIN_MENU) drawMenu();
		if (currScreen == GAMEPLAY_LOCAL || currScreen == GAMEPLAY_CLIENT || currScreen == GAMEPLAY_HOST) drawGame();
		if (currScreen == OPTION_MENU) drawOptionMenu();
		finishFrame();
		OSSleepTicks(MILLISECS_TO_TICKS(1000/targetFramerate)-/*Time to finish the frame*/(OSGetTime()-currTimestamp));
	}
	return;
}