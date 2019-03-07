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

int gameState = INTRO;
int transition = 0;

int highScore = 0;
int infection = 0;

u64 gameCountdown = SECS_TO_TICKS(4);
int runSpeed = 20;

int buttonCooldown = 0; // Used for menu navigating
int aimFPS = 60;

// Color Palette
#define OPTION_SELECTED 0xFCFCFC00
#define OPTION 0x99999900

int selectedOption = 0;

void run_game(bool debugMode);