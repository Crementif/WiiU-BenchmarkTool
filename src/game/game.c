#include "main.h"
#include "game.h"
#include "vpad.h"


int vpadError;
VPADData vpadData;

// menu screens
void drawIntro(bool debugMode) {
	// create transition
	transition++;
	if (transition<1.4*aimFPS) drawTextEx(-1, 65, 20, 0xF4424200, "Powered by the Trash bites! engine", true, 4, 1.4*aimFPS-(transition));
	if (transition>=1.4*aimFPS) drawTextEx(-1, 65, 20, 0xF4424200, "Powered by the Trash bites! engine", true, 4, 1);
	if (transition>4*aimFPS || debugMode) {
		transition = 0;
		gameState = MAIN_MENU;
	}
}

void drawMenu() {
	if (transition < 1*aimFPS) drawTextEx(-1, 72, 1, 0xFF905000, ">>> Trash Run <<<", true, 4, 1);
	else if (transition < 2*aimFPS) drawTextEx(-1, 72, 1, 0xFF905000, " >> Trash Run << ", true, 4, 1);
	else if (transition < 3*aimFPS) drawTextEx(-1, 72, 1, 0xFF905000, "  > Trash Run <  ", true, 4, 1);
	else if (transition < 4*aimFPS) drawTextEx(-1, 72, 1, 0xFF905000, "    Trash Run    ", true, 4, 1);
	transition+=2;
	if (transition>=4*aimFPS) transition = 0;
	
	drawTextEx(-1, 78, 20, ((selectedOption==0 && transition&1*aimFPS)? OPTION_SELECTED : OPTION), "PLAY", true, 2, 1);
	drawTextEx(-1, 77, 22, ((selectedOption==1 && transition&1*aimFPS)? OPTION_SELECTED : OPTION), "OPTIONS", true, 2, 1);
	
	if (vpadData.buttonTrigger&VPAD_BUTTON_A) {
		gameState = MAIN_MENU+1+selectedOption;
		transition = 0;
		selectedOption = 0;
		return;
	}
	if (vpadData.buttonTrigger&VPAD_BUTTON_UP || vpadData.buttonTrigger&VPAD_BUTTON_DOWN) {
		selectedOption = !selectedOption;
	}
}

void drawOptionMenu() {
	// TODO: Implement actual settings
	drawTextEx(-1, 72, 20, 0xFF905000, "FPS 60", true, 2, 1);
	drawTextEx(-1, 72, 22, 0xFF905000, "4K textures", true, 2, 1);
	if (vpadData.buttonTrigger&VPAD_BUTTON_UP || vpadData.buttonTrigger&VPAD_BUTTON_DOWN) {
		selectedOption = !selectedOption;
	}
	if (vpadData.buttonTrigger&VPAD_BUTTON_A) {
		// Change stuff
	}
	if (vpadData.buttonTrigger&VPAD_BUTTON_B) {
		transition = 0;
		selectedOption = 0;
		gameState = MAIN_MENU;
	}
}


void drawGame() {
	if (cooldown == 3) drawTextEx(-1, 72, 3, 0xFF905000, "3", true, 2, 1);
	if (cooldown == 2) drawTextEx(-1, 72, 3, 0xFF905000, "2", true, 2, 1);
	if (cooldown == 1) drawTextEx(-1, 72, 3, 0xFF905000, "1", true, 2, 1);
	if (cooldown == 0) drawTextEx(-1, 72, 3, 0xFF905000, "GO", true, 2, 1);
	
}


void run_game(bool debugMode)
{
	while(true) {
		OSScreenClearBufferEx(0, 0);
		OSScreenClearBufferEx(1, 0);
		VPADRead(0, &vpadData, 1, &vpadError);
		if (gameState == INTRO) drawIntro(debugMode);
		if (gameState == MAIN_MENU) drawMenu();
		if (gameState == GAMEPLAY) drawGame();
		if (gameState == OPTION_MENU) drawOptionMenu();
		OSScreenFlipBuffersEx(0);
		OSScreenFlipBuffersEx(1);
		OSSleepTicks(MILLISECS_TO_TICKS(1000/aimFPS)); // aim for roughly 30 FPS
	}
	return;
}