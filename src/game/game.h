#include <gctypes.h>



// Game States
#define INTRO (0)
#define MAIN_MENU (1)
#define GAMEPLAY (2)
#define OPTION_MENU (3)
#define GAME_OVER (4)

int highScore = 0;
int gameState = INTRO;
int transition = 0;
int aimFPS = 30;

int cooldown = 0;
int score = 0;


// Color Palette
#define OPTION_SELECTED 0xFCFCFC00
#define OPTION 0x99999900



int selectedOption = 0;
void run_game(bool debugMode);