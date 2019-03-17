#include <gctypes.h>

// Some values for the level generation.
#define LEVEL_SIZE (200)

// Game States
#define INTRO (0)
#define MAIN_MENU (1)
#define GAMEPLAY_CLIENT (2)
#define GAMEPLAY_HOST (3)
#define OPTION_MENU (4)
#define GAME_OVER (5)

unsigned char currScreen = INTRO;
unsigned char selectedInfection = 0;

unsigned char scrollSpeed = 20;
unsigned char targetFramerate = 60;

int transition = 0;
int localHighscore = 0;

u64 gameCountdown = SECS_TO_TICKS(4);

// Color Palette
#define OPTION_SELECTED 0xFCFCFC00
#define OPTION 0x99999900

unsigned char selectedOption = 0;

// Struct to sync states across multiplayer. Some technicalities:
// - The server constantly sends their gravity, position, velocity etc.
// - The client doesn't die unless the server says so.
// - If the client doesn't get certain packages, the physics will be synced later.

// TODO: One user could be running faster due to having more frames...
// - Idea: Could implement a system where the front runner gets slowed down so that the other player can catch up. Can't set players back since one player might've made a new obstacle so the grid would differ.
struct gameStateStruct {
	u64 levelSeed; // Syncs the RNG seed used for level generation
	unsigned char infectionLevel; // Character Selected
	unsigned char scrollSpeed; // Scroll Speed
	unsigned int health;
	unsigned int score;
	// Physics and controls
	int xPosition;
	int upVelocity;
	int downVelocity;

	int blocksTraversed;
	float subPixelScrollOffset;
};

void createNewGame() {
	struct gameStateStruct gameState = {
		.levelSeed = OSGetTime(),
		.infectionLevel = selectedInfection,
		.scrollSpeed = 20,
		.health = 100,
		.score = 0,
		.xPosition = BLOCK_PIXEL_HEIGHT*(STAGE_HEIGHT-1) - (PLAYER_PIXEL_HEIGHT*2),
		.upVelocity = 0,
		.downVelocity = 0,
		.blocksTraversed = 0,
		.subPixelScrollOffset = 0,
	};
}

struct gameStateStruct gameState;
void run_game(bool debugMode);