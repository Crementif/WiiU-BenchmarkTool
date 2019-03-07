#include <stdlib.h>
#include <time.h>
#include "frameBuffer.h"
#include "game.h"
#include "rendering.h"
#include "multiplayer.h"

int vpadError;
VPADData vpadData;

// menu screens
void drawIntro(bool debugMode) {
	// create transition
	transition++;
	levelSeed = 1;
	if (transition<1.4*aimFPS) drawTextEx(0, 65, 20, 0xF4424200, "Powered by the Trash bites! engine", true, 4, 1.4*aimFPS-transition);
	if (transition>=1.4*aimFPS) drawTextEx(0, 65, 20, 0xF4424200, "Powered by the Trash bites! engine", true, 4, 1);
	if (transition>4*aimFPS || debugMode) {
		transition = 0;
		gameState = MAIN_MENU;
	}
}

bool disableCounter = false;
sint64 firstCountdownMeasure = 0;
int retries = 0;

void drawGameOver() {
	if (highScore < score) highScore = score;
	drawTextEx(0, 72, 10, 0xF4424200, "Trash canned!", true, 5, 1);
	drawTextEx(0, 70, 40, 0xF4424200, "Press A to restart", true, 1, 1);
	drawTextEx(0, 65, 42, 0xF4424200, "Press B to go to the main menu", true, 1, 1);
	if (vpadData.buttonTrigger&VPAD_BUTTON_A || vpadData.buttonTrigger&VPAD_BUTTON_B) {
		levelSeed = OSGetTime();
		scrollPixelOffset = 0;
		scrollSubpixelOffset = 0;
		score = 0;
		blocksTraversed = 0;
		blockQueueLength = 0;
		upVelocity = 0;
		downVelocity = 0;
		gravity = 2;
		disableCounter = false;
		firstCountdownMeasure = 0;
		previousBottomYPos = BLOCK_PIXEL_HEIGHT*(STAGE_HEIGHT-1)-(PLAYER_PIXEL_HEIGHT*2);
		initializeLevelGrid();
		if (vpadData.buttonTrigger&VPAD_BUTTON_A) {
			gameState = GAMEPLAY_CLIENT;
			if (retries < 5) {
				disableCounter = false;
				firstCountdownMeasure = 0;
			}
			retries++;
		}
		if (vpadData.buttonTrigger&VPAD_BUTTON_B) gameState = MAIN_MENU;
	}
}

void drawMenu() {
	if (transition < 1*aimFPS) drawTextEx(0, 72, 1, 0xFF905000, ">>> Trash Run <<<", true, 4, 1);
	else if (transition < 2*aimFPS) drawTextEx(0, 72, 1, 0xFF905000, " >> Trash Run << ", true, 4, 1);
	else if (transition < 3*aimFPS) drawTextEx(0, 72, 1, 0xFF905000, "  > Trash Run <  ", true, 4, 1);
	else if (transition < 4*aimFPS) drawTextEx(0, 72, 1, 0xFF905000, "    Trash Run    ", true, 4, 1);
	transition+=2;
	if (transition>=4*aimFPS) transition = 0;
	
	drawTextEx(0, 78, 20, ((selectedOption==0 && transition&1*aimFPS)? OPTION_SELECTED : OPTION), "JOIN", true, 2, 1);
	drawTextEx(0, 78, 22, ((selectedOption==1 && transition&1*aimFPS)? OPTION_SELECTED : OPTION), "HOST", true, 2, 1);
	drawTextEx(0, 77, 24, ((selectedOption==2 && transition&1*aimFPS)? OPTION_SELECTED : OPTION), "OPTIONS", true, 2, 1);
	
	if (vpadData.buttonTrigger&VPAD_BUTTON_UP && selectedOption >= 1) selectedOption--;
	if (vpadData.buttonTrigger&VPAD_BUTTON_DOWN && selectedOption <= 1) selectedOption++;
	
	if (vpadData.buttonTrigger&VPAD_BUTTON_A) {
		if (selectedOption == 0) gameState = GAMEPLAY_CLIENT;
		else if (selectedOption == 1) gameState = GAMEPLAY_HOST;
		else if (selectedOption == 2) gameState = OPTION_MENU;
		transition = 0;
		selectedOption = 0;
	}
}

void drawOptionMenu() {
	char headOptionCharBuffer[27] = "Head -                     ";
	if (vpadData.buttonTrigger&VPAD_BUTTON_UP) {
		if (selectedOption > 0) selectedOption--;
	}
	else if (vpadData.buttonTrigger&VPAD_BUTTON_DOWN) {
		if (selectedOption < 1) selectedOption--;
	}
	
	if (selectedOption == 0) {
		if (vpadData.buttonTrigger&VPAD_BUTTON_LEFT && headUsed >= 1) headUsed--;
		if (vpadData.buttonTrigger&VPAD_BUTTON_RIGHT && headUsed <= 5) headUsed++;
		memmove(headOptionCharBuffer+7, playerHeads[headUsed], 20);
	}
	
	if (vpadData.buttonTrigger&VPAD_BUTTON_A) {
		// Change stuff
	}
	if (vpadData.buttonTrigger&VPAD_BUTTON_B) {
		transition = 0;
		selectedOption = 0;
		gameState = MAIN_MENU;
	}
	drawTextEx(0, 72, 22, 0xFF905000, headOptionCharBuffer, true, 2, 1);
	drawTextEx(0, 72, 24, 0xFF905000, "Speed - ", true, 2, 1);
	drawTextEx(0, 65, 42, 0xF4424200, "Press B to go to the main menu", true, 1, 1);
}

bool waitingForConnection = true;

bool drawGame(bool hostOrClient) {
	if (hostOrClient == true) {
		// Initialize server and wait for a connection response
		waitForConnection();
	}
	//else if (gameState == GAMEPLAY_HOST)
	if (firstCountdownMeasure == 0) firstCountdownMeasure = OSGetTime()+SECS_TO_TICKS(4);
	sint64 gameCountdownPast = firstCountdownMeasure - OSGetTime();
	
	bool playerIsDead = false;
	if (gameCountdownPast > 0) {
		renderLevel(0, &vpadData);
	}
	else {
		playerIsDead = renderLevel(runSpeed, &vpadData);
	}
	
	if (gameCountdownPast < -1) disableCounter = true;
	
	if (!disableCounter) {
		if (gameCountdownPast > (sint64)SECS_TO_TICKS(3)) drawTextEx(0, 72, 3, 0xFF905000, "3", false, 2, 2);
		else if (gameCountdownPast > (sint64)SECS_TO_TICKS(2)) drawTextEx(0, 72, 3, 0xFF905000, "2", false, 2, 2);
		else if (gameCountdownPast > (sint64)SECS_TO_TICKS(1)) drawTextEx(0, 72, 3, 0xFF905000, "1", false, 2, 2);
		else drawTextEx(0, 72, 3, 0xFF905000, "GO", false, 2, 2);
	}
	return playerIsDead;
}

void run_game(bool debugMode)
{
	initializeLevelGrid();
	u64 ExecutionTime;
	socket_lib_init();
	while(true) {
		ExecutionTime = OSGetTime();
		OSScreenClearBufferEx(0, 0);
		OSScreenClearBufferEx(1, 0);
		VPADRead(0, &vpadData, 1, &vpadError);
		bool playerIsDead = false;
		if (gameState == INTRO) drawIntro(debugMode);
		if (gameState == MAIN_MENU) drawMenu();
		if (gameState == GAMEPLAY_CLIENT) playerIsDead = drawGame(false);
		if (gameState == GAMEPLAY_HOST) playerIsDead = drawGame(true);
		if (gameState == GAME_OVER) drawGameOver();
		if (gameState == OPTION_MENU) drawOptionMenu();
		if (playerIsDead) gameState = GAME_OVER;
		if ((gameState == GAMEPLAY_CLIENT || gameState == GAMEPLAY_HOST) || gameState == GAME_OVER) {
			int xPos;
			int yPos;
			if (gameState == GAMEPLAY_CLIENT || gameState == GAMEPLAY_HOST) {
				xPos = 10;
				yPos = 3;
			}
			else {
				xPos = 72;
				yPos = 20;
			}
			char scoreCharBuffer[] = "Score:          \0";
			itoa(score, scoreCharBuffer+7, 10);
			drawTextEx(0, xPos, yPos, 0xFF905000, scoreCharBuffer, false, 2, 1);
			char highscoreCharBuffer[] = "Highscore:          \0";
			itoa(highScore, highscoreCharBuffer+12, 10);
			drawTextEx(0, xPos, yPos-2, 0xFF905000, highscoreCharBuffer, false, 2, 1);
		}
		finishFrame();
		ExecutionTime = OSGetTime() - ExecutionTime;
		OSSleepTicks(MILLISECS_TO_TICKS(1000/aimFPS)-ExecutionTime);
	}
	return;
}