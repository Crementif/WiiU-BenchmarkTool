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
unsigned char selectedScrollSpeed = 20; // Not available through the settings

unsigned char targetFramerate = 60;

// Basic animation variables
float animationTimestamp = 0;
s64 animationStartTimestamp;
unsigned char animationLength = 99; // Set this to how long you want the basic animation to last
bool loopAnimation = false; // If enabled, will make the animation timer reset if it reaches the length that's set with animationLength

unsigned int localHighscore = 0;

// Color Palette
#define OPTION_SELECTED 0xFCFCFC00
#define OPTION 0x99999900

// Physics changes
#define GRAVITY (2)
#define JUMP_VELOCITY (55)

char ipAddress[] = "127.0.0.1";

#define DEBUG_GAME (true)