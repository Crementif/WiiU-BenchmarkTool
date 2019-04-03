#define GAMEPAD_WIDTH (896) // Gamepad buffer is byte aligned, so this is not the actual resolution but the length of the line buffer
#define GAMEPAD_HEIGHT (480)

#define SCREEN_WIDTH (1280)


//void gamepadToScreen() {
//	for (int i=0; i<GAMEPAD_HEIGHT; i++) {
//		memcpy(gamepadframeBufferHexPtr+i*GAMEPAD_WIDTH, frameBufferHexPtr+i*SCREEN_WIDTH, GAMEPAD_WIDTH);
//	}
//}

void renderGamepad(VPADData* controller) {
	drawTextEx(1, 4, 2, 0xFF905000, "Attack!", false, 2, 1);
}