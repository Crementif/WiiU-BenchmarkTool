#include <gctypes.h>

// Some values for the level generation.
#define LEVEL_SIZE (200)

#define INTRO (0)
#define MAIN_MENU (1)
#define GAMEPLAY_LOCAL (2)
#define GAMEPLAY_CLIENT (3)
#define GAMEPLAY_HOST (4)
#define OPTION_MENU (5)

unsigned char currScreen = INTRO;
unsigned char selectedInfection = 0;
unsigned char selectedOption = 0;

unsigned char scrollSpeed = 20;
unsigned char targetFramerate = 60;

unsigned char animationLength = 99;
float animationTimestamp = 0;
s64 animationStartTimestamp;
bool loopAnimation = false;

unsigned int localHighscore = 0;

// Color Palette
#define OPTION_SELECTED 0xFCFCFC00
#define OPTION 0x99999900

// Physics changes
#define GRAVITY (2)
#define JUMP_VELOCITY (55)

char *ipAddress = "127.0.0.1";

void run_game(bool debugMode);