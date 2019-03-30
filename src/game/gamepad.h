#define GAMEPAD_WIDTH (896)
#define GAMEPAD_HEIGHT (480)

#define SCREEN_WIDTH (1280)

#define DEBUG_GAMEPAD_VIEW (true)

void gamepadToScreen() {
	for (int i=0; i<GAMEPAD_HEIGHT; i++) {
		memcpy(&frameBufferHexPtr[i*SCREEN_WIDTH], &gamepadframeBufferHexPtr[i*GAMEPAD_WIDTH], GAMEPAD_WIDTH);
	}
#if DEBUG_GAMEPAD_VIEW != true
	OSScreenClearBufferEx(0, 0);
	OSScreenClearBufferEx(1, 0);
#endif
}

void screenToGamepad() {
}

void renderGamepad(VPADData* controller) {
	drawTextEx(0, 4, 4, 0xF4424200, "GAMEPAD MENU", true, 1, 1);
	gamepadToScreen();
}