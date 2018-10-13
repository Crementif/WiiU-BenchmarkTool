#include <gctypes.h>

// Some values for the level generation.
#define LEVEL_SIZE (200)

#define STAGE_HEIGHT (9)
#define STAGE_WIDTH (16)

// Game States
#define INTRO (0)
#define MAIN_MENU (1)
#define GAMEPLAY (2)
#define OPTION_MENU (3)
#define GAME_OVER (4)

int gameState = INTRO;
int transition = 0;


int highScore = 0;
int infection = 0;

int gameCountdown = 0;
int gameScore = 0;

int buttonCooldown = 0; // Used for menu navigating
int aimFPS = 60;

// Color Palette
#define OPTION_SELECTED 0xFCFCFC00
#define OPTION 0x99999900

int selectedOption = 0;
void run_game(bool debugMode);

// Game
uint64 levelSeed = 0;
int levelStage[STAGE_HEIGHT][STAGE_WIDTH];