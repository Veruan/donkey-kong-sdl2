#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern "C"
{
#include"../SDL2-2.0.10/include/SDL.h"
#include"../SDL2-2.0.10/include/SDL_main.h"
}

const int SCREEN_WIDTH = 1200; //pixels
const int SCREEN_HEIGHT = 840;

const int levelWidth = 9;
const int levelHeight = 33;

const int xModifier = 120; //level to actual position
const int yModifier = 16;

const int yPlatformOffset = 15; // spawn
const int yLadderOffset = 13;
const int yPlayerOffset = 13;
const int yKongOffset = 12;
const int yTrophyOffset = 14;

const char* menuText = "WELCOME TO THE GAME";
const double menuTextScale = 5.0;
const int menuOptions = 4;
const int titleFlag = 10;
const int pseudonymStartPlace = 50;

const int playerLives = 3;
const double playerSpeed = 500.0;
const double jumpStrength = -60.0;
const double playerGravity = 1.0;

const int barrelCount = 5;
const double barrelSpeed = 300.0;
const double barrelGravity = 1.0;

const double throwTimer = 1.2;

const int trophyValue = 500;
const int barrelValue = 100;
const int levelValue = 1000;

typedef struct menu_t
{
	int active[menuOptions];
};


typedef struct colors_t
{
	int black;
	int red;
	int green;
	int blue;
	int yellowOrange;
	int platinum;
	int darkPurple;
	int darkSky;
};


typedef struct hitbox_t
{
	int top;
	int left;
	int right;
	int bottom;
};


typedef struct player_t
{
	char* name;
	int points;
	SDL_Surface* sprite;
	int playerWidth;
	int playerHeight;
	int playerX;
	int playerY; // im mniejszy tym wyzej
	double verticalVelocity;
	double horizontalVelocity;
	hitbox_t hitbox;
	int lives;
	int ladderFlag;
	int jumpFlag;
	int pointFlag;
	int pointGrabbed;
	double pointTimer;
	double animationTimer;
};


typedef struct kong_t
{
	SDL_Surface* sprite;
	int kongWidth;
	int kongHeight;
	int kongX;
	int kongY;
	double timer;
	hitbox_t hitbox;
	int throwFlag;
	double animationTimer;
};


typedef struct barrel_t
{
	SDL_Surface* sprite;
	int barrelWidth;
	int barrelHeight;
	int barrelX;
	int barrelY;
	double verticalVelocity;
	double horizontalVelocity;
	hitbox_t hitbox;
	int ladderFlag;
	int throwFlag;
	double animationTimer;
};


typedef struct platform_t
{
	SDL_Surface* sprite;
	int platformWidth;
	int platformHeight;
	int platformX;
	int platformY;
	hitbox_t hitbox;
};


typedef struct ladder_t
{
	SDL_Surface* sprite;
	int ladderY;
	int ladderX;
	int ladderHeight;
	int ladderWidth;
	hitbox_t hitbox;
};


typedef struct screen_t
{
	SDL_Surface* screen;
	SDL_Surface* charset;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* scrtex;
};


typedef struct trophy_t
{
	SDL_Surface* sprite;
	int trophyX;
	int trophyY;
	int trophyWidth;
	int trophyHeight;
	hitbox_t hitbox;
};


typedef struct level_t
{
	int levelNum;
	int platformCount;
	int ladderCount;
	int trophyCount;
	int* platformsX;
	int* platformsY;
	int* laddersX;
	int* laddersY;
	int* trophiesX;
	int* trophiesY;
	int playerSpawn[2];
	int kongSpawn[2];
	char rampColor[10];
};


typedef struct sprites_t
{
	SDL_Surface* Pleft1;
	SDL_Surface* Pleft2;

	SDL_Surface* Pright1;
	SDL_Surface* Pright2;

	SDL_Surface* Pladder1;
	SDL_Surface* Pladder2;

	SDL_Surface* PjumpLeft;
	SDL_Surface* PjumpRight;

	SDL_Surface* Pdead;

	SDL_Surface* kongStand;
	SDL_Surface* kongThrow;
	SDL_Surface* kongBarrel;

	SDL_Surface* barrel1;
	SDL_Surface* barrel2;

	SDL_Surface* live;
};


typedef struct items_t
{
	player_t player;
	kong_t kong;

	platform_t* platform;
	ladder_t* ladder;
	barrel_t* barrel;
	trophy_t* trophy;

	sprites_t sprites;
};


typedef struct commands_t
{
	int quit;
	int restart;
	int menu;
	int startAnother;
};


void generalLoop(int* t1, screen_t screen, char* text, SDL_Event event, colors_t colors, level_t level, items_t items, commands_t* commands, int levelNum, char* playerName);
void handleMainMenu(screen_t* screen, colors_t* colors, items_t* items, level_t* level, int* levelNum, char** playerName);
void handleRestart(char** playerName, items_t* items, screen_t* screen, level_t* level);
void handleNextLevel(char** playerName, items_t* items, screen_t* screen, level_t* level);
void handleQuit(screen_t* screen, items_t* items, level_t* level);

void initAll(screen_t* screen, colors_t* colors, level_t* level, items_t* items, int* levelNumber, char** playerName);

void initStatic(screen_t* screen, colors_t* colors);
void initDynamic(screen_t screen, items_t* items, level_t* level, int levelNum, char* playerName);

void initWindowAndRenderer(screen_t* screen);

void initSprites(screen_t screen, sprites_t* sprites);
SDL_Surface* initBMP(screen_t* screen, char* path);

#pragma region menu
void menu(screen_t* screen, colors_t colors, int* levelNumber, char** playerName);
void initMenu(menu_t* menu);
void drawMenu(screen_t screen, colors_t colors, menu_t menu);
void drawMenuText(screen_t screen, colors_t colors, menu_t menu);
void drawMenuString(screen_t screen, colors_t colors, const char* text, int y, menu_t menu, int index);
void menuEvent(SDL_Event event, int* levelEntered, int* option, menu_t* menu);
void menuKeyDown(SDL_Event event, int* levelEntered, int* option, menu_t* menu);
int findActive(menu_t menu);
void optionView(int option, screen_t* screen, colors_t colors, SDL_Event event, char** playerName);
void enterPseudonym(screen_t* screen, SDL_Event event, colors_t colors, char** playerName);
void handleInput(SDL_Event event, int* pseudonymEntered, screen_t* screen, char* text, int* cursorPos);
#pragma endregion

#pragma region colors
void initColors(colors_t* colors, screen_t screen);
#pragma endregion

#pragma region platforms
void initPlatform(platform_t** platform, screen_t screen, level_t level);
void initPlatformSprite(platform_t* platform, screen_t screen, level_t level);
void initPlatformSpawn(platform_t* platform, level_t level, int index);
void initPlatformHitbox(platform_t* platform);
platform_t* allocatePlatforms(level_t level);
#pragma endregion

#pragma region ladders
void initLadder(ladder_t** ladder, screen_t screen, level_t level);
void initLadderSprite(ladder_t* ladder, screen_t screen, level_t level);
void initLadderSpawn(ladder_t* ladder, level_t level, int index);
void initLadderHitbox(ladder_t* ladder);
ladder_t* allocateLadders(level_t level);
#pragma endregion

#pragma region trophies
void initTrophies(trophy_t** trophy, screen_t screen, level_t level);
void initTrophySprite(trophy_t* trophy, screen_t screen);
void initTrophySpawn(trophy_t* trophy, level_t level, int index);
void initTrophyHitbox(trophy_t* trophy);
trophy_t* allocateTrophies(level_t level);

void updateTrophyPosition(items_t* items, int index);

void handlePoints(items_t* items, level_t level);
#pragma endregion

#pragma region player
void initPlayer(items_t* items, screen_t screen, level_t level, char* playerName);
void initPlayerSprite(items_t* items, screen_t screen);
void initPlayerSpawn(player_t* player, level_t level);
void initPlayerVelocity(player_t* player);
void initPlayerHitbox(player_t* player);
void initPlayerInfo(player_t* player, char* playerName);

void updatePlayerPosition(player_t* player, double valueX, double valueY);
void updatePlayerHitbox(player_t* player, double valueX, double valueY);
#pragma endregion

#pragma region enemies
void initKong(items_t* items, screen_t screen, level_t level);
void initKongSprite(items_t* items, screen_t screen);
void initKongSpawn(kong_t* kong, level_t level);
void initKongHitbox(kong_t* kong);

barrel_t* allocateBarrels();
void initBarrels(items_t* items, screen_t screen);
void initBarrelSprite(barrel_t* barrel, items_t* items, screen_t screen);
void initBarrelSpawn(barrel_t* barrel);
void initBarrelHitbox(barrel_t* barrel);
void initBarrelVelocity(barrel_t* barrel);

void handleBarrelThrow(items_t* items);
void updateBarrelPosition(barrel_t* barrel, double valueX, double valueY);
void updateBarrelHitbox(barrel_t* barrel, double valueX, double valueY);
int calculateBarrelKongDistance(barrel_t* barrel, kong_t kong);
int barrelKongY(barrel_t barrel, kong_t kong);
int barrelKongX(barrel_t barrel, kong_t kong);

void handleBarrelXY(barrel_t barrel, double* barrelDistanceX, double* barrelDistanceY, double delta);
void handleBarrelX(barrel_t barrel, double* barrelDistanceX, double delta);
void updateBarrelsPosition(barrel_t** barrel, double delta);
#pragma endregion

#pragma region level
void initLevel(level_t* level, int levelNum);
char* handleFilePath(int levelNum);
void writeLevelInfo(char** levelTxt, const char* filePath, level_t* level);
char** allocateLevelTxt();

void interpretLevelData(level_t* level, char** levelTxt);
void interpretItemCount(level_t* level, char** levelTxt);
void allocatePlatformsCoords(level_t* level, int platformCount);
void allocateLaddersCoords(level_t* level, int ladderCount);
void allocateTrophyCoords(level_t* level, int trophyCount);
void interpretItemsPosition(level_t* level, char** levelTxt);
#pragma endregion

#pragma region draw
void updateScreen(screen_t screen);
void drawString(screen_t screen, int x, int y, const char* text, double scale);
void drawSurface(screen_t screen, SDL_Surface* sprite, int x, int y);
void drawPixel(SDL_Surface* surface, int x, int y, Uint32 color);
void drawLine(screen_t screen, int x, int y, int l, int dx, int dy, Uint32 color);
void drawRectangle(screen_t screen, int x, int y, int width, int height, Uint32 outlineColor, Uint32 fillColor);
void drawGameMenu(screen_t screen, colors_t colors, items_t items, double worldTime);
void drawScene(screen_t screen, colors_t colors, items_t items, level_t level, double worldTime);
void drawStaticItems(screen_t screen, items_t items, level_t level);
void drawDynamicItems(screen_t screen, items_t items);
void draw(screen_t screen, colors_t colors, items_t items, level_t level, double worldTime);
void drawHit(items_t items, screen_t screen);
// rysowanie linii o d�ugo�ci l w pionie (gdy dx = 0, dy = 1) 
// b�d� poziomie (gdy dx = 1, dy = 0)
// draw a vertical (when dx = 0, dy = 1) or horizontal (when dx = 1, dy = 0) line
#pragma endregion

#pragma region movementAppliance
void handleX(player_t player, double* distanceX, double delta);
void handleY(player_t player, double* distanceY, double delta);
void handleXY(player_t player, double* distanceX, double* distanceY, double delta);
#pragma endregion

void gameLoop(int t1, screen_t screen, char* text, SDL_Event event, colors_t colors, level_t* level, items_t* items, commands_t* commands);
void handleTime(double t1, double* t2, double* delta, double* worldTime, int* quit, items_t* items);

#pragma region events
void eventHandler(int* quit, player_t* player, platform_t* platform, ladder_t* ladder, SDL_Event event, level_t* level, commands_t* commands);
void handleKeyDown(SDL_Event event, int* quit, player_t* player, platform_t* platform, ladder_t* ladder, level_t* level, commands_t* commands);

void handleHit(items_t* items, commands_t* commands, screen_t screen, int* quit);
void handleEnd(commands_t* commands, int* quit, level_t level);
void hitInput(SDL_Event event, int* quit, int* breakout, commands_t* commands);
#pragma endregion

#pragma region physics
void physics(items_t* items, level_t level, screen_t screen, double delta, int* quit, commands_t* commands);

void playerPhysics(player_t* player, platform_t* platform, ladder_t* ladder, level_t level, double delta);
void gravity(player_t* player, platform_t* platform, level_t level);
int detectColissionX(player_t* player, platform_t* platform, level_t level, double delta);
int detectBottomColissionY(player_t player, platform_t* platform, level_t level);
int detectTopCollisionY(player_t player, platform_t* platform, level_t level);
int checkInRange(int x, int smaller, int greater);
int canEnterLadder(player_t player, ladder_t* ladder, level_t level, double modifier);
int isOnLadder(player_t player, ladder_t* ladder, level_t level);
int checkLadderPlayerY(player_t player, ladder_t ladder);

void barrelPhysics(barrel_t** barrel, platform_t* platform, ladder_t* ladder, level_t level, double delta);
void gravityBarrel(barrel_t* barrel, platform_t* platform, level_t level, double delta);
int detectBarrelColissionX(barrel_t barrel, platform_t* platform, level_t level, double delta);
int detectBottomBarrelColissionY(barrel_t barrel, platform_t* platform, level_t level, double delta, int* y);

int detectPlayerBarrelCollision(barrel_t barrel, player_t player, double delta);
int detectKongCollision(items_t items);
int detectPlayerTrophyCollision(items_t* items, level_t level);
int detectPlayerBarrelJump(items_t items);
#pragma endregion

#pragma region movement
void handleJump(player_t* player, platform_t* platform, level_t level);
void handleMovement(player_t* player, char side, platform_t* platform, level_t level);
void handleLadder(player_t* player, ladder_t* ladder, level_t level, char sign);
void enterLadder(player_t* player, double modifier);
#pragma endregion

#pragma region animation
void initPlayerMovementSprites(screen_t screen, sprites_t* sprites);
void initPlayerSpecialSprites(screen_t screen, sprites_t* sprites);

void initKongSprites(screen_t screen, sprites_t* sprites);
void initAssetSprites(screen_t screen, sprites_t* sprites);

void updateSprites(items_t* items, double delta);

void updatePlayerSprite(items_t* items);
void handleRightMovSprite(items_t* items);
void handleLeftMovSprite(items_t* items);
void handleJumpSprite(items_t* items);
void handleLadderSprite(items_t* items);
void handlePlayerPoint(player_t player, screen_t screen);
void pointInfo(items_t* items, double delta);

void updateKongSprite(items_t* items);

void updateBarrelSprite(items_t* items, double delta);
#pragma endregion

#pragma region free
void resetCommands(commands_t* commands);
void freeAll(screen_t* screen, items_t* items, level_t* level);
void freeLevelInfo(level_t* level);
void freeSurfaces(screen_t* screen);
void freePlatforms(platform_t** platform);
void freeLadders(ladder_t** ladder);
void freeTrophies(trophy_t** trophy);
void freeLevelSurfaces(sprites_t* sprites, items_t* items);
void freePlayerSprites(sprites_t* sprites);
void freeEnemySprites(sprites_t* sprites);

void freeLevelTxt(char** levelTxt);
#pragma endregion

#ifdef __cplusplus
extern "C"
#endif


int main(int argc, char** argv)
{
	SDL_Event event = { 0 };
	screen_t screen;
	colors_t colors;
	level_t level;
	items_t items;
	commands_t commands;
	int levelNum;
	char* playerName = "\0";

	initAll(&screen, &colors, &level, &items, &levelNum, &playerName);

	char text[128];
	int t1 = SDL_GetTicks();

	generalLoop(&t1, screen, text, event, colors, level, items, &commands, levelNum, playerName);

	return 0;
}


void generalLoop(int* t1, screen_t screen, char* text, SDL_Event event, colors_t colors, level_t level, items_t items, commands_t* commands, int levelNum, char* playerName)
{
	while (1)
	{
		gameLoop(*t1, screen, text, event, colors, &level, &items, commands);

		if (commands->menu == 1)
		{
			handleMainMenu(&screen, &colors, &items, &level, &levelNum, &playerName);
			*t1 = SDL_GetTicks();
		}

		if (commands->restart == 1)
		{
			handleRestart(&playerName, &items, &screen, &level);
			*t1 = SDL_GetTicks();
		}

		if (commands->quit == 1)
		{
			handleQuit(&screen, &items, &level);
			break;
		}

		if (commands->startAnother == 1)
		{
			handleNextLevel(&playerName, &items, &screen, &level);

		}

		resetCommands(commands);
	}
}


void handleMainMenu(screen_t* screen, colors_t* colors, items_t* items, level_t* level, int* levelNum, char** playerName)
{
	SDL_Quit();
	freeAll(screen, items, level);
	initAll(screen, colors, level, items, levelNum, playerName);
}


void handleRestart(char** playerName, items_t* items, screen_t* screen, level_t* level)
{
	*playerName = items->player.name;
	int playerLives = items->player.lives;
	int playerPoints = items->player.points;
	int levelActualNum = level->levelNum;
	initDynamic(*screen, items, level, levelActualNum, *playerName);
	(&items->player)->lives = playerLives;
	(&items->player)->points = playerPoints;
}


void handleNextLevel(char** playerName, items_t* items, screen_t* screen, level_t* level)
{
	*playerName = items->player.name;
	int playerLives = items->player.lives;
	int playerPoints = items->player.points + levelValue;
	initDynamic(*screen, items, level, level->levelNum + 1, *playerName);
	(&items->player)->lives = playerLives;
	(&items->player)->points = playerPoints;
}


void handleQuit(screen_t* screen, items_t* items, level_t* level)
{
	SDL_Quit();
	freeAll(screen, items, level);
}


void initAll(screen_t* screen, colors_t* colors, level_t* level, items_t* items, int* levelNumber, char** playerName)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("SDL_Init error: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	initStatic(screen, colors);

	menu(screen, *colors, levelNumber, playerName);
	
	initDynamic(*screen, items, level, *levelNumber, *playerName);
}


void initStatic(screen_t* screen, colors_t* colors)
{
	initWindowAndRenderer(screen);
	initColors(colors, *screen);
	screen->charset = initBMP(screen, "./cs8x8.bmp");
	SDL_SetColorKey(screen->charset, true, 0x000000);
}


void menu(screen_t* screen, colors_t colors, int* levelNumber, char** playerName)
{
	int levelEntered = 0, option;

	menu_t menu;
	initMenu(&menu);
	SDL_Event event = { 0 };


	while (!levelEntered)
	{
		drawMenu(*screen, colors, menu);

		menuEvent(event, &levelEntered, &option, &menu);
	}

	*levelNumber = option + 1;

	event = { 0 };
	optionView(option, screen, colors, event, playerName);
}


void initDynamic(screen_t screen, items_t* items, level_t* level, int levelNum, char* playerName)
{
	initLevel(level, levelNum);
	initSprites(screen, &items->sprites);
	initPlatform(&items->platform, screen, *level);
	initLadder(&items->ladder, screen, *level);
	initPlayer(items, screen, *level, playerName);
	initKong(items, screen, *level);
	initBarrels(items, screen);
	initTrophies(&items->trophy, screen, *level);
}


void initColors(colors_t* colors, screen_t screen)
{
	colors->black = SDL_MapRGB(screen.screen->format, 0x00, 0x00, 0x00);
	colors->green = SDL_MapRGB(screen.screen->format, 0x00, 0xFF, 0x00);
	colors->red = SDL_MapRGB(screen.screen->format, 0xFF, 0x00, 0x00);
	colors->blue = SDL_MapRGB(screen.screen->format, 0x11, 0x11, 0xCC);
	colors->yellowOrange = SDL_MapRGB(screen.screen->format, 0xFF, 0xAA, 0x47);
	colors->platinum = SDL_MapRGB(screen.screen->format, 0xE6, 0xE8, 0xE6);
	colors->darkPurple = SDL_MapRGB(screen.screen->format, 0x42, 0x00, 0x39);
	colors->darkSky = SDL_MapRGB(screen.screen->format, 0x00, 0x00, 0x0E);
}



void initMenu(menu_t* menu)
{
	for (int i = 0; i < menuOptions; i++)
		menu->active[i] = 0;

	menu->active[0] = 1;
}


void menuEvent(SDL_Event event, int* levelEntered, int* option, menu_t* menu)
{
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			menuKeyDown(event, levelEntered, option, menu);
			break;

		case SDL_KEYUP:
			break;

		case SDL_QUIT:
			*levelEntered = 1;
			break;
		}
	}
}


void menuKeyDown(SDL_Event event, int* levelEntered, int* option, menu_t* menu)
{
	int active = findActive(*menu);

	switch (event.key.keysym.sym)
	{
	case SDLK_ESCAPE:
		exit(EXIT_SUCCESS);
		break;

	case SDLK_UP:
		menu->active[active] = 0;

		if (active == 0)
			active = menuOptions;

		menu->active[active - 1] = 1;
		break;

	case SDLK_DOWN:
		menu->active[active] = 0;

		if (active == menuOptions - 1)
			active = -1;

		menu->active[active + 1] = 1;
		break;

	case SDLK_RETURN:
		active = findActive(*menu);
		*option = active;
		*levelEntered = 1;
		break;
	}
}


int findActive(menu_t menu)
{
	for (int i = 0; i < menuOptions; i++)
	{
		if (menu.active[i] == 1)
			return i;
	}

	perror("Error - no active menu option found");
	exit(EXIT_FAILURE);
}


void optionView(int option, screen_t* screen, colors_t colors, SDL_Event event, char** playerName)
{
	if (option != menuOptions - 1)
	{
		enterPseudonym(screen, event, colors, playerName);
	}

	else if(option == menuOptions - 1)
		exit(EXIT_SUCCESS);

}


void enterPseudonym(screen_t* screen, SDL_Event event, colors_t colors, char** playerName)
{
	int pseudonymEntered = 0;
	char text[20] = "\0";
	int cursorPos = 0;
	int x = SCREEN_WIDTH / 2 - SCREEN_WIDTH / 5, y = SCREEN_HEIGHT / 2;

	while (!pseudonymEntered)
	{
		drawRectangle(*screen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, colors.black, colors.black);
		drawString(*screen, x, y - 50, "ENTER YOUR NAME", 3.0);
		drawString(*screen, x, y, text, 3.0);
		updateScreen(*screen);

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_KEYDOWN)
				handleInput(event, &pseudonymEntered, screen, text, &cursorPos);
		}
	}

	*playerName = (text[0] == '\0') ? strdup("?") : strdup(text);
}


void handleInput(SDL_Event event, int* pseudonymEntered, screen_t* screen, char* text, int* cursorPos) 
{
	char letter;

	switch (event.key.keysym.sym) 
	{
		case SDLK_ESCAPE:
			freeSurfaces(screen);
			exit(EXIT_SUCCESS);
			break;

		case SDLK_BACKSPACE:

			if (*cursorPos > 0) 
			{
				(*cursorPos)--;
				text[*cursorPos] = '\0';
			}
			break;

		case SDLK_RETURN:
			*pseudonymEntered = 1;
			break;

		default:
			if (event.key.keysym.sym >= SDLK_a && event.key.keysym.sym <= SDLK_z) 
			{
				letter = (char)event.key.keysym.sym;

				if (*cursorPos < 19)
				{
					text[*cursorPos] = letter;
					text[*cursorPos + 1] = '\0'; 
					(*cursorPos)++;
				}

			}
			break;
	}
}


void drawMenu(screen_t screen, colors_t colors, menu_t menu)
{
	SDL_FillRect(screen.screen, NULL, colors.blue);
	drawRectangle(screen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, colors.platinum, colors.darkPurple);
	drawMenuText(screen, colors, menu);
	updateScreen(screen);
}


void drawMenuText(screen_t screen, colors_t colors, menu_t menu)
{
	drawMenuString(screen, colors, "DONKEY KONG BY 198290", SCREEN_HEIGHT / 10, menu, titleFlag);
	drawMenuString(screen, colors, "Level 1", 3 * SCREEN_HEIGHT / 10, menu, 0);
	drawMenuString(screen, colors, "Level 2", 4 * SCREEN_HEIGHT / 10, menu, 1);
	drawMenuString(screen, colors, "Level 3", 5 * SCREEN_HEIGHT / 10, menu, 2);
	drawMenuString(screen, colors, "High Score Table", 7 * SCREEN_HEIGHT / 10, menu, 3);
}


void drawMenuString(screen_t screen, colors_t colors, const char* text, int y, menu_t menu, int index)
{
	drawString(screen, SCREEN_WIDTH / 2 - 4 * int(menuTextScale) * strlen(text), y, text, menuTextScale);

	if (index != titleFlag && menu.active[index] == 1)
		drawRectangle(screen, SCREEN_WIDTH / 2 - 4 * int(menuTextScale) * strlen(text),
			y + 8 * int(menuTextScale), strlen(text) * 8 * int(menuTextScale), 3, colors.yellowOrange, colors.yellowOrange);
}


void initSprites(screen_t screen, sprites_t* sprites)
{
	initPlayerMovementSprites(screen, sprites);
	initPlayerSpecialSprites(screen, sprites);

	initKongSprites(screen, sprites);

	initAssetSprites(screen, sprites);
}


void initPlayerMovementSprites(screen_t screen, sprites_t* sprites)
{
	sprites->Pleft1 = initBMP(&screen, "./DonkeyKongTextures2/playerL1.bmp");
	sprites->Pleft2 = initBMP(&screen, "./DonkeyKongTextures2/playerL2.bmp");

	sprites->Pright1 = initBMP(&screen, "./DonkeyKongTextures2/playerR1.bmp");
	sprites->Pright2 = initBMP(&screen, "./DonkeyKongTextures2/playerR2.bmp");
}


void initPlayerSpecialSprites(screen_t screen, sprites_t* sprites)
{
	sprites->PjumpLeft = initBMP(&screen, "./DonkeyKongTextures2/playerJL.bmp");
	sprites->PjumpRight = initBMP(&screen, "./DonkeyKongTextures2/playerJR.bmp");

	sprites->Pladder1 = initBMP(&screen, "./DonkeyKongTextures2/playerLadder1.bmp");
	sprites->Pladder2 = initBMP(&screen, "./DonkeyKongTextures2/playerLadder2.bmp");

	sprites->Pdead = initBMP(&screen, "./DonkeyKongTextures2/playerDead.bmp");
}


void initKongSprites(screen_t screen, sprites_t* sprites)
{
	sprites->kongStand = initBMP(&screen, "./DonkeyKongTextures2/kong.bmp");
	sprites->kongThrow = initBMP(&screen, "./DonkeyKongTextures2/kongThrow.bmp");
	sprites->kongBarrel = initBMP(&screen, "./DonkeyKongTextures2/kongBarrel.bmp");
}


void initAssetSprites(screen_t screen, sprites_t* sprites)
{
	sprites->barrel1 = initBMP(&screen, "./DonkeyKongTextures2/barrel1.bmp");
	sprites->barrel2 = initBMP(&screen, "./DonkeyKongTextures2/barrel2.bmp");

	sprites->live = initBMP(&screen, "./DonkeyKongTextures2/live.bmp");
}


platform_t* allocatePlatforms(level_t level)
{
	platform_t* platforms = (platform_t*)malloc(level.platformCount * sizeof(platform_t));

	if (platforms == NULL)
	{
		perror("Error - allocating memory for platforms");
		exit(EXIT_FAILURE);
	}

	return platforms;
}


void initPlatform(platform_t** platform, screen_t screen, level_t level)
{
	*platform = allocatePlatforms(level);

	for (int i = 0; i < level.platformCount; i++)
	{
		initPlatformSprite(&(*platform)[i], screen, level);
		initPlatformSpawn(&(*platform)[i], level, i);
		initPlatformHitbox(&(*platform)[i]);
	}
}


void initPlatformSprite(platform_t* platform, screen_t screen, level_t level)
{
	char rampPath[40];
	snprintf(rampPath, sizeof(rampPath), "%s-%s.bmp", "./DonkeyKongTextures2/ramp", level.rampColor);

	platform->sprite = initBMP(&screen, rampPath);
	platform->platformHeight = platform->sprite->h;
	platform->platformWidth = platform->sprite->w;
}


void initPlatformSpawn(platform_t* platform, level_t level, int index)
{
	platform->platformX = level.platformsX[index];
	platform->platformY = level.platformsY[index];
}


void initPlatformHitbox(platform_t* platform)
{

	platform->hitbox.top = platform->platformY - (platform->platformHeight / 2);
	platform->hitbox.bottom = platform->platformY + (platform->platformHeight / 2);
	platform->hitbox.left = platform->platformX - (platform->platformWidth / 2);
	platform->hitbox.right = platform->platformX + (platform->platformWidth / 2);

}


void initLadder(ladder_t** ladder, screen_t screen, level_t level)
{
	*ladder = allocateLadders(level);

	for (int i = 0; i < level.ladderCount; i++)
	{
		initLadderSprite(&(*ladder)[i], screen, level);
		initLadderSpawn(&(*ladder)[i], level, i);
		initLadderHitbox(&(*ladder)[i]);
	}
}


ladder_t* allocateLadders(level_t level)
{
	ladder_t* ladders = (ladder_t*)malloc(level.ladderCount * sizeof(ladder_t));

	if (ladders == NULL)
	{
		perror("Error - allocating memory for platforms");
		exit(EXIT_FAILURE);
	}

	return ladders;
}


void initLadderSprite(ladder_t* ladder, screen_t screen, level_t level)
{
	char ladderPath[40];
	snprintf(ladderPath, sizeof(ladderPath), "%s-%s.bmp", "./DonkeyKongTextures2/ladder", level.rampColor);

	ladder->sprite = initBMP(&screen, ladderPath);
	ladder->ladderHeight = ladder->sprite->h;
	ladder->ladderWidth = ladder->sprite->w;
}


void initLadderSpawn(ladder_t* ladder, level_t level, int index)
{
	ladder->ladderX = level.laddersX[index];
	ladder->ladderY = level.laddersY[index];
}


void initLadderHitbox(ladder_t* ladder)
{

	ladder->hitbox.top = ladder->ladderY - (ladder->ladderHeight / 2);
	ladder->hitbox.bottom = ladder->ladderY + (ladder->ladderHeight / 2);
	ladder->hitbox.left = ladder->ladderX - (ladder->ladderWidth / 2);
	ladder->hitbox.right = ladder->ladderX + (ladder->ladderWidth / 2);

}


void initPlayer(items_t* items, screen_t screen, level_t level, char* playerName)
{
	initPlayerSprite(items, screen);
	initPlayerSpawn(&items->player, level);
	initPlayerHitbox(&items->player);
	initPlayerVelocity(&items->player);
	initPlayerInfo(&items->player, playerName);
}


void initPlayerInfo(player_t* player, char* playerName)
{
	player->ladderFlag = 0;
	player->jumpFlag = 0;
	player->animationTimer = 0;
	player->lives = playerLives;
	player->name = playerName;
	player->points = 0;
	player->pointFlag = 0;
	player->pointTimer = 0;
	player->pointGrabbed = 0;
}


void initPlayerSprite(items_t* items, screen_t screen)
{
	(&items->player)->sprite = items->sprites.Pleft1;
	(&items->player)->playerHeight = (&items->player)->sprite->h;
	(&items->player)->playerWidth = (&items->player)->sprite->w;
}


void initPlayerHitbox(player_t* player)
{
	player->hitbox.top = player->playerY - (player->playerHeight / 2);
	player->hitbox.bottom = player->playerY + (player->playerHeight / 2);
	player->hitbox.left = player->playerX - (player->playerWidth / 2);
	player->hitbox.right = player->playerX + (player->playerWidth / 2);
}


void updatePlayerHitbox(player_t* player, double valueX, double valueY)
{
	player->hitbox.top += valueY;
	player->hitbox.bottom += valueY;
	player->hitbox.left += valueX;
	player->hitbox.right += valueX;
}


void initPlayerSpawn(player_t* player, level_t level)
{
	player->playerY = level.playerSpawn[0];
	player->playerX = level.playerSpawn[1];
}


void initPlayerVelocity(player_t* player)
{
	player->verticalVelocity = 0;
	player->horizontalVelocity = 0;
}


void initLevel(level_t* level, int levelNum)
{
	char* filePath = handleFilePath(levelNum);
	char** levelTxt = allocateLevelTxt();
	level->levelNum = levelNum;
	writeLevelInfo(levelTxt, filePath, level);

	interpretLevelData(level, levelTxt);

	freeLevelTxt(levelTxt);
	free(filePath);
}


char* handleFilePath(int levelNum)
{
	char base[15] = "./levels/level";
	char levelNumTxt[2];

	snprintf(levelNumTxt, sizeof(levelNumTxt), "%d", levelNum);

	char* filePath = (char*)malloc(strlen(base) + strlen(levelNumTxt) + 5);

	if (filePath == NULL)
	{
		perror("Error - error allocating memory for file path");
		exit(EXIT_FAILURE);
	}

	strcpy(filePath, base);
	strcat(filePath, levelNumTxt);
	strcat(filePath, ".txt");

	return filePath;
}


char** allocateLevelTxt()
{
	char** levelTxt = (char**)malloc(levelHeight * sizeof(char*));

	if (levelTxt == NULL)
	{
		perror("Error - error allocating memory for levelTxt");
		exit(EXIT_FAILURE);
	}


	for (int i = 0; i < levelHeight; i++)
	{
		levelTxt[i] = (char*)malloc(levelWidth * sizeof(char));

		if (levelTxt[i] == NULL)
		{
			perror("Error - error allocating memory for levelTxt");
			exit(EXIT_FAILURE);
		}
	}

	return levelTxt;
}


void writeLevelInfo(char** levelTxt, const char* filePath, level_t* level)
{
	FILE* file = fopen(filePath, "r");

	if (file == NULL)
	{
		perror("Error - error opening level file");
		exit(EXIT_FAILURE);
	}

	fscanf(file, "%s", level->rampColor);

	for (int i = 0; i < levelHeight; i++)
	{
		for (int j = 0; j < levelWidth; j++)
		{
			char fileChar;

			fileChar = fgetc(file);

			if (fileChar == '#' || fileChar == 'X' || fileChar == 'O' ||
				fileChar == 'H' || fileChar == 'K' || fileChar == 'T')
				levelTxt[i][j] = fileChar;

			else
				j--;
		}
	}

	fclose(file);
}


void interpretLevelData(level_t* level, char** levelTxt)
{
	interpretItemCount(level, levelTxt);
	allocatePlatformsCoords(level, level->platformCount);
	allocateLaddersCoords(level, level->ladderCount);
	allocateTrophyCoords(level, level->trophyCount);
	interpretItemsPosition(level, levelTxt);
}


void interpretItemCount(level_t* level, char** levelTxt)
{
	int platformCount = 0;
	int ladderCount = 0;
	int trophyCount = 0;

	for (int i = 0; i < levelHeight; i++)
	{
		for (int j = 0; j < levelWidth; j++)
		{
			if (levelTxt[i][j] == 'X' || levelTxt[i][j] == 'x')
			{
				platformCount++;
			}

			if (levelTxt[i][j] == 'H' || levelTxt[i][j] == 'h')
			{
				ladderCount++;
			}

			if (levelTxt[i][j] == 'T' || levelTxt[i][j] == 't')
			{
				trophyCount++;
			}
		}
	}

	level->platformCount = platformCount;
	level->ladderCount = ladderCount;
	level->trophyCount = trophyCount;
}


void allocatePlatformsCoords(level_t* level, int platformCount)
{
	level->platformsX = (int*)malloc(platformCount * sizeof(int));
	level->platformsY = (int*)malloc(platformCount * sizeof(int));

	if (level->platformsX == NULL || level->platformsY == NULL)
	{
		perror("Error - error allocating memory for level platform info");
		exit(EXIT_FAILURE);
	}
}


void allocateLaddersCoords(level_t* level, int ladderCount)
{
	level->laddersX = (int*)malloc(ladderCount * sizeof(int));
	level->laddersY = (int*)malloc(ladderCount * sizeof(int));

	if (level->laddersX == NULL || level->laddersY == NULL)
	{
		perror("Error - error allocating memory for level ladder info");
		exit(EXIT_FAILURE);
	}
}


void allocateTrophyCoords(level_t* level, int trophyCount)
{
	level->trophiesX = (int*)malloc(trophyCount * sizeof(int));
	level->trophiesY = (int*)malloc(trophyCount * sizeof(int));

	if (level->trophiesX == NULL || level->trophiesY == NULL)
	{
		perror("Error - error allocating memory for level trophy info");
		exit(EXIT_FAILURE);
	}
}


void interpretItemsPosition(level_t* level, char** levelTxt)
{
	int platformCounter = 0;
	int ladderCounter = 0;
	int trophyCounter = 0;

	for (int i = 0; i < levelHeight; i++)
	{
		for (int j = 0; j < levelWidth; j++)
		{

			switch (levelTxt[i][j])
			{
			case 'X':
				level->platformsY[platformCounter] = (i + yPlatformOffset) * yModifier;
				level->platformsX[platformCounter] = (j + 1) * xModifier;

				platformCounter++;
				break;

			case 'O':
				level->playerSpawn[0] = (i + yPlayerOffset) * yModifier;
				level->playerSpawn[1] = (j + 1) * xModifier;
				break;

			case 'K':
				level->kongSpawn[0] = (i + yKongOffset) * yModifier;
				level->kongSpawn[1] = (j + 1) * xModifier;
				break;

			case 'H':
				level->laddersY[ladderCounter] = (i + yLadderOffset) * yModifier;
				level->laddersX[ladderCounter] = (j + 1) * xModifier;

				ladderCounter++;
				break;

			case 'T':
				level->trophiesY[trophyCounter] = (i + yTrophyOffset) * yModifier;
				level->trophiesX[trophyCounter] = (j + 1) * xModifier;

				trophyCounter++;
				break;
			}
		}
	}
}


void updatePlayerPosition(player_t* player, double valueX, double valueY)
{
	player->playerX += valueX;
	player->playerY += valueY;
	updatePlayerHitbox(player, valueX, valueY);
}


void initWindowAndRenderer(screen_t* screen)
{
	if (SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP, &screen->window, &screen->renderer) != 0)
	{
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	SDL_RenderSetLogicalSize(screen->renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(screen->renderer, 0, 0, 0, 255);

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_SetWindowTitle(screen->window, "Donkey Kong - 198290");
	SDL_ShowCursor(SDL_DISABLE);
	SDL_SetWindowIcon(screen->window, initBMP(screen, "./DonkeyKongTextures2/icon.bmp"));

	screen->screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	screen->scrtex = SDL_CreateTexture(screen->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
}


void initKong(items_t* items, screen_t screen, level_t level)
{
	initKongSprite(items, screen);
	initKongSpawn(&items->kong, level);
	initKongHitbox(&items->kong);

	(&items->kong)->timer = 0;
	(&items->kong)->throwFlag = 0;
	(&items->kong)->animationTimer = 0;
}


void initKongSprite(items_t* items, screen_t screen)
{
	(&items->kong)->sprite = items->sprites.kongStand;

	(&items->kong)->kongWidth = (&items->kong)->sprite->w;
	(&items->kong)->kongHeight = (&items->kong)->sprite->h;
}


void initKongSpawn(kong_t* kong, level_t level)
{
	kong->kongY = level.kongSpawn[0];
	kong->kongX = level.kongSpawn[1];
}


void initKongHitbox(kong_t* kong)
{
	kong->hitbox.top = kong->kongY - (kong->kongHeight / 2);
	kong->hitbox.bottom = kong->kongY + (kong->kongHeight / 2);
	kong->hitbox.left = kong->kongX - (kong->kongWidth / 2);
	kong->hitbox.right = kong->kongX + (kong->kongWidth / 2);
}


void initBarrels(items_t* items, screen_t screen)
{
	items->barrel = allocateBarrels();

	for (int i = 0; i < barrelCount; i++)
	{
		initBarrelSprite(&(*(&items->barrel))[i], items, screen);
		initBarrelSpawn(&(*(&items->barrel))[i]);
		initBarrelHitbox(&(*(&items->barrel))[i]);
		initBarrelVelocity(&(*(&items->barrel))[i]);
		(&(*(&items->barrel))[i])->ladderFlag = 0;
		(&(*(&items->barrel))[i])->throwFlag = 0;
		(&(*(&items->barrel))[i])->animationTimer = 0;
	}
}


barrel_t* allocateBarrels()
{
	barrel_t* barrel = (barrel_t*)malloc(barrelCount * sizeof(barrel_t));
	if (barrel == NULL)
	{
		perror("Error - error allocating memory for barrels");
		exit(EXIT_FAILURE);
	}

	return barrel;
}


void initBarrelSprite(barrel_t* barrel, items_t* items, screen_t screen)
{
	barrel->sprite = items->sprites.barrel1;

	barrel->barrelWidth = barrel->sprite->w;
	barrel->barrelHeight = barrel->sprite->h;
}


void initBarrelSpawn(barrel_t* barrel)
{
	barrel->barrelX = SCREEN_WIDTH + 200;
	barrel->barrelY = SCREEN_HEIGHT + 200;
}


void initBarrelHitbox(barrel_t* barrel)
{
	barrel->hitbox.top = barrel->barrelY - (barrel->barrelHeight / 2);
	barrel->hitbox.bottom = barrel->barrelY + (barrel->barrelHeight / 2);
	barrel->hitbox.left = barrel->barrelX - (barrel->barrelWidth / 2);
	barrel->hitbox.right = barrel->barrelX + (barrel->barrelWidth / 2);
}


void initBarrelVelocity(barrel_t* barrel)
{
	barrel->verticalVelocity = 0;
	barrel->horizontalVelocity = 0;
}


void initTrophies(trophy_t** trophy, screen_t screen, level_t level)
{
	*trophy = allocateTrophies(level);

	for (int i = 0; i < level.trophyCount; i++)
	{
		initTrophySprite(&(*trophy)[i], screen);
		initTrophySpawn(&(*trophy)[i], level, i);
		initTrophyHitbox(&(*trophy)[i]);
	}
}


trophy_t* allocateTrophies(level_t level)
{
	trophy_t* trophy = (trophy_t*)malloc(level.trophyCount * sizeof(trophy_t));
	if (trophy == NULL)
	{
		perror("Error - error allocating memory for trophies");
		exit(EXIT_FAILURE);
	}

	return trophy;
}


void initTrophySprite(trophy_t* trophy, screen_t screen)
{
	trophy->sprite = initBMP(&screen, "./DonkeyKongTextures2/trophy.bmp");
	trophy->trophyWidth = trophy->sprite->w;
	trophy->trophyHeight = trophy->sprite->h;
}


void initTrophySpawn(trophy_t* trophy, level_t level, int index)
{
	trophy->trophyY = level.trophiesY[index];
	trophy->trophyX = level.trophiesX[index];
}


void initTrophyHitbox(trophy_t* trophy)
{
	trophy->hitbox.top = trophy->trophyY - (trophy->trophyHeight / 2);
	trophy->hitbox.bottom = trophy->trophyY + (trophy->trophyHeight / 2);
	trophy->hitbox.left = trophy->trophyX - (trophy->trophyWidth / 2);
	trophy->hitbox.right = trophy->trophyX + (trophy->trophyWidth / 2);
}


SDL_Surface* initBMP(screen_t* screen, char* path)
{
	SDL_Surface* bmp = SDL_LoadBMP(path);

	if (bmp == NULL)
	{
		printf("SDL_LoadBMP(%s) error: %s\n", path, SDL_GetError());
		SDL_FreeSurface(screen->screen);
		SDL_DestroyTexture(screen->scrtex);
		SDL_DestroyWindow(screen->window);
		SDL_DestroyRenderer(screen->renderer);
		SDL_Quit();
		exit(EXIT_FAILURE);
	}

	return bmp;
}


void freeAll(screen_t* screen, items_t* items, level_t* level)
{
	freeSurfaces(screen);
	freeLevelSurfaces(&items->sprites, items);
	freePlatforms(&items->platform);
	freeLadders(&items->ladder);
	freeTrophies(&items->trophy);
	freeLevelInfo(level);
}


void freeLevelInfo(level_t* level)
{
	free(level->laddersX);
	free(level->laddersY);
	free(level->platformsX);
	free(level->platformsY);
	free(level->trophiesX);
	free(level->trophiesY);
}


void freeSurfaces(screen_t* screen)
{
	SDL_FreeSurface(screen->charset);
	SDL_FreeSurface(screen->screen);
	SDL_DestroyTexture(screen->scrtex);
	SDL_DestroyRenderer(screen->renderer);
	SDL_DestroyWindow(screen->window);
}


void freeTrophies(trophy_t** trophy)
{
	free(*trophy);
}


void freePlatforms(platform_t** platform)
{
	free(*platform);
}


void freeLadders(ladder_t** ladder)
{
	free(*ladder);
}


void freeLevelTxt(char** levelTxt)
{
	for (int i = 0; i < levelHeight; i++)
	{
		free(levelTxt[i]);
	}

	free(levelTxt);
}


void drawString(screen_t screen, int x, int y, const char* text, double scale)
{
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8 * scale;
	d.h = 8 * scale;

	while (*text)
	{
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;

		s.x = px;
		s.y = py;

		d.x = x;
		d.y = y;

		SDL_BlitScaled(screen.charset, &s, screen.screen, &d);
		x += 8 * scale;
		text++;
	}
}


void drawSurface(screen_t screen, SDL_Surface* sprite, int x, int y)
{
	SDL_Rect dest;
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen.screen, &dest);
}


void drawPixel(SDL_Surface* surface, int x, int y, Uint32 color)
{
	int bpp = surface->format->BytesPerPixel;
	Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32*)p = color;
}


void drawRectangle(screen_t screen, int x, int y, int width, int height, Uint32 outlineColor, Uint32 fillColor)
{
	drawLine(screen, x, y, height, 0, 1, outlineColor);
	drawLine(screen, x + width - 1, y, height, 0, 1, outlineColor);
	drawLine(screen, x, y, width, 1, 0, outlineColor);
	drawLine(screen, x, y + height - 1, width, 1, 0, outlineColor);

	for (int i = y + 1; i < y + height - 1; i++)
		drawLine(screen, x + 1, i, width - 2, 1, 0, fillColor);
}


void drawLine(screen_t screen, int x, int y, int l, int dx, int dy, Uint32 color)
{
	for (int i = 0; i < l; i++)
	{
		drawPixel(screen.screen, x, y, color);
		x += dx;
		y += dy;
	}
}


void drawGameMenu(screen_t screen, colors_t colors, items_t items, double worldTime)
{
	char text[128];

	drawRectangle(screen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT / 10, colors.red, colors.black);

	for(int i = 0; i < items.player.lives; i ++)
		drawSurface(screen, items.sprites.live, (SCREEN_WIDTH / 2 - 500 + (40 * i)), SCREEN_HEIGHT / 10 / 2);

	drawString(screen, SCREEN_WIDTH / 2 - 4 * strlen(items.player.name), SCREEN_HEIGHT / 10 / 2, items.player.name, 1);

	sprintf(text, "Time : %.1lf    Points : %d", worldTime, items.player.points);
	drawString(screen, SCREEN_WIDTH / 2 - 4 * strlen(text), SCREEN_HEIGHT / 15 / 2, text, 1);
}


void drawScene(screen_t screen, colors_t colors, items_t items, level_t level, double worldTime)
{
	SDL_FillRect(screen.screen, NULL, colors.darkSky);

	drawStaticItems(screen, items, level);
	
	drawDynamicItems(screen, items);

	drawGameMenu(screen, colors, items, worldTime);
}


void drawStaticItems(screen_t screen, items_t items, level_t level)
{
	for (int i = 0; i < level.platformCount; i++)
		drawSurface(screen, items.platform[i].sprite, items.platform[i].platformX, items.platform[i].platformY);

	for (int i = 0; i < level.ladderCount; i++)
		drawSurface(screen, items.ladder[i].sprite, items.ladder[i].ladderX, items.ladder[i].ladderY);

	for (int i = 0; i < level.trophyCount; i++)
	{
		if (items.trophy[i].trophyX == 0 && items.trophy[i].trophyY == 0)
			continue;

		drawSurface(screen, items.trophy[i].sprite, items.trophy[i].trophyX, items.trophy[i].trophyY);
	}
}


void drawDynamicItems(screen_t screen, items_t items)
{
	handlePlayerPoint(items.player, screen);

	drawSurface(screen, items.player.sprite, items.player.playerX, items.player.playerY);

	for (int i = 0; i < barrelCount; i++)
		drawSurface(screen, items.barrel[i].sprite, items.barrel[i].barrelX, items.barrel[i].barrelY);

	drawSurface(screen, items.kong.sprite, items.kong.kongX, items.kong.kongY);
}


void handlePlayerPoint(player_t player, screen_t screen)
{
	char text[12] = { '\0' };

	if (player.pointGrabbed > 0 && player.pointTimer <= 1.0)
	{
		snprintf(text, sizeof(text), "%d", player.pointGrabbed);

		drawString(screen, player.playerX - 12, player.hitbox.top - 8, text, 1.0);
	}
}


void updateScreen(screen_t screen)
{
	SDL_UpdateTexture(screen.scrtex, NULL, screen.screen->pixels, screen.screen->pitch);
	SDL_RenderCopy(screen.renderer, screen.scrtex, NULL, NULL);
	SDL_RenderPresent(screen.renderer);
}


void draw(screen_t screen, colors_t colors, items_t items, level_t level, double worldTime)
{
	drawScene(screen, colors, items, level, worldTime);
	updateScreen(screen);
}


void handleX(player_t player, double* distanceX, double delta)
{
	static double accumulatedX = 0.0;
	static double accumulatedNX = 0.0;

	if (player.horizontalVelocity >= 0)
	{
		accumulatedNX = 0.0;

		accumulatedX += player.horizontalVelocity * delta;

		*distanceX = accumulatedX;

		if (accumulatedX >= 1.0)
			accumulatedX = 0;
	}

	if (player.horizontalVelocity < 0)
	{
		accumulatedNX += player.horizontalVelocity * delta;

		*distanceX = round(accumulatedNX);

		if (*distanceX < -1.0);

		if (round(accumulatedNX) <= -1.0)
			accumulatedNX = 0;
	}
}


void handleY(player_t player, double* distanceY, double delta)
{
	*distanceY = player.verticalVelocity * delta;

	if (*distanceY > 0)
		*distanceY += 1.0;
}


void handleXY(player_t player, double* distanceX, double* distanceY, double delta)
{
	handleX(player, distanceX, delta);
	handleY(player, distanceY, delta);
}


void handleBarrelXY(barrel_t barrel, double* barrelDistanceX, double* barrelDistanceY, double delta)
{
	if (barrel.verticalVelocity > 0)
		*barrelDistanceY = barrel.verticalVelocity * delta + 1.0;

	if (barrel.horizontalVelocity != 0)
		handleBarrelX(barrel, barrelDistanceX, delta);

	else
		*barrelDistanceX = 0;
}


void handleBarrelX(barrel_t barrel, double* barrelDistanceX, double delta)
{
	if (barrel.horizontalVelocity < 0)
		*barrelDistanceX = barrel.horizontalVelocity * delta;

	else if (barrel.horizontalVelocity > 0)
		*barrelDistanceX = barrel.horizontalVelocity * delta + 1.0;
}


void gameLoop(int t1, screen_t screen, char* text, SDL_Event event, colors_t colors, level_t* level, items_t* items, commands_t* commands)
{
	int quit = 0;
	double t2, delta, frames = 0, distanceX = 0, distanceY = 0, worldTime = 120;

	while (!quit)
	{
		handleTime(t1, &t2, &delta, &worldTime, &quit, items);

		handleBarrelThrow(items);
		physics(items, *level, screen, delta, &quit, commands);
		handleXY(items->player, &distanceX, &distanceY, delta);
		updateBarrelsPosition((&items->barrel), delta);

		draw(screen, colors, *items, *level, worldTime);

		eventHandler(&quit, (&items->player), items->platform, items->ladder, event, level, commands);

		updatePlayerPosition((&items->player), distanceX, distanceY);
		updateSprites(items, delta);

		frames++;
	}
}


void handleTime(double t1, double* t2, double* delta, double* worldTime, int* quit, items_t* items)
{
	*t2 = SDL_GetTicks();
	*delta = fmin((*t2 - t1) * 0.001, 0.002);
	t1 = *t2;
	(&items->kong)->timer += *delta;
	if (*worldTime <= 0)
		*quit = 1;
	else
		*worldTime -= *delta;
}


void updateSprites(items_t* items, double delta)
{
	(&items->player)->animationTimer += delta;
	(&items->kong)->animationTimer += delta;

	pointInfo(items, delta);

	updatePlayerSprite(items);
	updateKongSprite(items);

	updateBarrelSprite(items, delta);
}


void updateBarrelSprite(items_t* items, double delta)
{
	for (int i = 0; i < barrelCount; i++)
	{
		if ((&(*(&items->barrel))[i])->throwFlag == 1)
		{
			(&(*(&items->barrel))[i])->animationTimer += delta;

			if ((&(*(&items->barrel))[i])->animationTimer >= 0.2)
			{
				(&(*(&items->barrel))[i])->sprite = items->sprites.barrel2;

				if ((&(*(&items->barrel))[i])->animationTimer >= 0.4)
					(&(*(&items->barrel))[i])->animationTimer = 0;
			}

			else
				(&(*(&items->barrel))[i])->sprite = items->sprites.barrel1;
		}
	}
}


void pointInfo(items_t* items, double delta)
{
	if (items->player.pointGrabbed > 0 && (&items->player)->pointTimer <= 1.0)
		(&items->player)->pointTimer += delta;
	else
	{
		(&items->player)->pointTimer = 0;
		(&items->player)->pointGrabbed = 0;
	}
}


void updatePlayerSprite(items_t* items)
{
	if ((&items->player)->jumpFlag == 1)
	{
		handleJumpSprite(items);
	}

	else if ((&items->player)->ladderFlag == 1)
	{
		handleLadderSprite(items);
		
	}

	else if ((&items->player)->horizontalVelocity > 0)
	{
		handleRightMovSprite(items);
	}

	else if ((&items->player)->horizontalVelocity < 0)
	{
		handleLeftMovSprite(items);
	}

	else
		(&items->player)->animationTimer = 0;
}


void handleLadderSprite(items_t* items)
{
	if ((&items->player)->animationTimer >= 0.2)
	{
		(&items->player)->sprite = items->sprites.Pladder2;

		if ((&items->player)->animationTimer >= 0.4)
			(&items->player)->animationTimer = 0;
	}

	else
		(&items->player)->sprite = items->sprites.Pladder1;
}


void handleRightMovSprite(items_t* items)
{
	if ((&items->player)->animationTimer >= 0.1)
	{
		(&items->player)->sprite = items->sprites.Pright2;

		if ((&items->player)->animationTimer >= 0.2)
			(&items->player)->animationTimer = 0;
	}

	else
		(&items->player)->sprite = items->sprites.Pright1;
}


void handleLeftMovSprite(items_t* items)
{
	if ((&items->player)->animationTimer >= 0.1)
	{
		(&items->player)->sprite = items->sprites.Pleft2;

		if ((&items->player)->animationTimer >= 0.2)
			(&items->player)->animationTimer = 0;
	}

	else
		(&items->player)->sprite = items->sprites.Pleft1;
}


void handleJumpSprite(items_t* items)
{
	if ((&items->player)->horizontalVelocity >= 0)
		(&items->player)->sprite = items->sprites.PjumpRight;

	else
		(&items->player)->sprite = items->sprites.PjumpLeft;

	(&items->player)->animationTimer = 0;
}


void updateKongSprite(items_t* items)
{
	if ((&items->kong)->throwFlag == 1)
	{
		(&items->kong)->sprite = items->sprites.kongBarrel;

		if ((&items->kong)->animationTimer >= 0.1)
			(&items->kong)->sprite = items->sprites.kongThrow;

		if ((&items->kong)->animationTimer >= 0.8)
			(&items->kong)->throwFlag = 0;
	}

	else
	{
		(&items->kong)->sprite = items->sprites.kongStand;
		(&items->kong)->animationTimer = 0;
	}
}


void handleBarrelThrow(items_t* items)
{
	if ((& items->kong)->timer >= throwTimer)
	{
		(&items->kong)->timer = 0;
		(&items->kong)->throwFlag = 1;
		
		int index = calculateBarrelKongDistance(items->barrel, items->kong);
		int x = barrelKongX((items->barrel)[index], items->kong);
		int y = barrelKongY((items->barrel)[index], items->kong) + items->kong.kongHeight / 3 - (items->barrel)[index].barrelHeight / 2;

		updateBarrelPosition(&(*(&items->barrel))[index], x, y);

		(&(*(&items->barrel))[index])->horizontalVelocity = barrelSpeed;
		(&(*(&items->barrel))[index])->verticalVelocity = 0;
		(&(*(&items->barrel))[index])->throwFlag = 1;
	}
}


int barrelKongY(barrel_t barrel, kong_t kong)
{
	return kong.kongY - barrel.barrelY;
}


int barrelKongX(barrel_t barrel, kong_t kong)
{
	return kong.kongX - barrel.barrelX;
}


void updateBarrelPosition(barrel_t* barrel, double valueX, double valueY)
{
	barrel->barrelX += valueX;
	barrel->barrelY += valueY;
	updateBarrelHitbox(barrel, valueX, valueY);
}


void updateBarrelsPosition(barrel_t** barrel, double delta)
{
	double distanceX = 0, distanceY = 0;

	for (int i = 0; i < barrelCount; i++)
	{
		handleBarrelXY((*barrel)[i], &distanceX, &distanceY, delta);
		updateBarrelPosition(&(*barrel)[i], distanceX, distanceY);
	}
}


void updateBarrelHitbox(barrel_t* barrel, double valueX, double valueY)
{
	barrel->hitbox.top += valueY;
	barrel->hitbox.bottom += valueY;
	barrel->hitbox.left += valueX;
	barrel->hitbox.right += valueX;
}


int calculateBarrelKongDistance(barrel_t* barrel, kong_t kong)
{
	int closest = 0;

	for (int i = 0; i < barrelCount; i++)
	{
		if (abs(barrel[i].barrelY - kong.kongY) > abs(barrel[closest].barrelY - kong.kongY))
		{
			closest = i;
		}
	}

	return closest;
}


void eventHandler(int* quit, player_t* player, platform_t* platform, ladder_t* ladder, SDL_Event event, level_t* level, commands_t* commands)
{
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			handleKeyDown(event, quit, player, platform, ladder, level, commands);
			break;

		case SDL_KEYUP:
			player->horizontalVelocity = 0;
			player->verticalVelocity = 0;
			break;

		case SDL_QUIT:
			*quit = 1;
			break;
		}
	}
}


void handleKeyDown(SDL_Event event, int* quit, player_t* player, platform_t* platform, ladder_t* ladder, level_t* level, commands_t* commands)
{
	switch (event.key.keysym.sym)
	{
	case SDLK_ESCAPE: case SDLK_q:
		*quit = 1;
		commands->quit = (event.key.keysym.sym == SDLK_ESCAPE);
		commands->menu = (event.key.keysym.sym == SDLK_q);
		break;

	case SDLK_n: case SDLK_1: case SDLK_2: case SDLK_3:
		*quit = 1;
		commands->restart = 1;
		player->points = 0;
		level->levelNum = (event.key.keysym.sym == SDLK_n) ? level->levelNum : event.key.keysym.sym - SDLK_1 + 1;
		break;

	case SDLK_LEFT: case SDLK_RIGHT:
		handleMovement(player, (event.key.keysym.sym == SDLK_LEFT) ? 'L' : 'R', platform, *level);
		break;

	case SDLK_SPACE:
		handleJump(player, platform, *level);
		break;

	case SDLK_UP: case SDLK_DOWN:
		handleLadder(player, ladder, *level, (event.key.keysym.sym == SDLK_UP) ? '+' : '-');
		break;
	}
}



void handleJump(player_t* player, platform_t* platform, level_t level)
{
	for (int i = 0; i < level.platformCount; i++)
	{
		if (platform[i].hitbox.top == player->hitbox.bottom && player->verticalVelocity == 0)
		{
			player->jumpFlag = 1;
			player->verticalVelocity = jumpStrength;
			player->playerY -= 1;
			updatePlayerHitbox(player, 0, -1);
		}
	}
}


void handleMovement(player_t* player, char side, platform_t* platform, level_t level)
{
	switch (side)
	{
	case 'R':
		player->horizontalVelocity = playerSpeed;
		break;

	case 'L':
		player->horizontalVelocity = -1.0 * playerSpeed;
		break;
	}
}


void handleLadder(player_t* player, ladder_t* ladder, level_t level, char sign)
{
	double modifier;

	switch (sign)
	{
	case '+':
		modifier = -1.0;
		break;
	case '-':
		modifier = 1.0;
		break;
	default:
		perror("Error - unknown ladder modifier sign");
		exit(EXIT_FAILURE);
	}

	if (player->ladderFlag == 0 && canEnterLadder(*player, ladder, level, modifier) == 0)
		enterLadder(player, modifier);

	else if (player->ladderFlag == 1)
	{
		player->verticalVelocity = modifier * 0.5;
	}
}


int canEnterLadder(player_t player, ladder_t* ladder, level_t level, double modifier)
{
	for (int i = 0; i < level.ladderCount; i++)
	{
		if ((player.verticalVelocity == 0 || player.ladderFlag == 1) &&
			checkInRange(player.playerX, ladder[i].hitbox.left, ladder[i].hitbox.right) == 0 &&
			checkLadderPlayerY(player, ladder[i]) == 0)
		{
			if (modifier < 0)
			{
				if (ladder[i].hitbox.top < player.hitbox.top)
					return 0;
			}

			if (modifier > 0)
			{
				if (ladder[i].hitbox.bottom > player.hitbox.bottom)
					return 0;
			}

		}
	}

	return 1;
}


int isOnLadder(player_t player, ladder_t* ladder, level_t level)
{
	for (int i = 0; i < level.ladderCount; i++)
	{
		if ((player.verticalVelocity == 0 || player.ladderFlag == 1) &&
			checkInRange(player.playerX, ladder[i].hitbox.left, ladder[i].hitbox.right) == 0 &&
			checkLadderPlayerY(player, ladder[i]) == 0)
		{
			return 0;
		}
	}

	return 1;
}


void enterLadder(player_t* player, double modifier)
{
	int value = 10 * int(modifier);

	player->ladderFlag = 1;
	player->playerY += value;
	updatePlayerHitbox(player, 0, value);
}


int checkLadderPlayerY(player_t player, ladder_t ladder)
{
	if (checkInRange(player.playerY, ladder.hitbox.top, ladder.hitbox.bottom) == 0 ||
		checkInRange(player.hitbox.top, ladder.hitbox.top, ladder.hitbox.bottom) == 0 ||
		checkInRange(player.hitbox.bottom, ladder.hitbox.top, ladder.hitbox.bottom) == 0)
		return 0;

	else
		return 1;
}


int checkInRange(int x, int smaller, int greater) //0 jesli tak 1 jesli nie
{
	if (x >= smaller && x <= greater)
		return 0;

	else
		return 1;
}


void physics(items_t* items, level_t level, screen_t screen, double delta, int* quit, commands_t* commands)
{
	playerPhysics((&items->player), items->platform, items->ladder, level, delta);
	barrelPhysics((&items->barrel), items->platform, items->ladder, level, delta);

	for (int i = 0; i < barrelCount; i++)
	{
		if (detectPlayerBarrelCollision((items->barrel)[i], items->player, delta) == 1)
			handleHit(items, commands, screen, quit);
	}

	handlePoints(items, level);

	if (detectKongCollision(*items) == 1)
		handleEnd(commands, quit, level);
}


void handleEnd(commands_t* commands, int* quit, level_t level)
{
	*quit = 1;

	if (level.levelNum == 3)
		commands->quit = 1;

	else
		commands->startAnother = 1;
}


int detectKongCollision(items_t items)
{
	if (checkInRange(items.player.playerX, items.kong.hitbox.left, items.kong.hitbox.right) == 0 &&
		checkInRange(items.player.playerY, items.kong.hitbox.top, items.kong.hitbox.bottom) == 0)
		return 1;

	return 0;
}


void handleHit(items_t* items, commands_t* commands, screen_t screen, int* quit)
{
	int breakout = 0;

	(&items->player)->sprite = items->sprites.Pdead;
	(&items->player)->lives -= 1;

	if ((&items->player)->lives == 0)
	{
		breakout = 1;
		*quit = 1;
		commands->menu = 1;
	}

	drawHit(*items, screen);

	while (!breakout)
	{
		SDL_Event event = { 0 };

		hitInput(event, quit, &breakout, commands);
	}

}


void hitInput(SDL_Event event, int* quit, int* breakout, commands_t* commands)
{
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				*quit = 1;
				*breakout = 1;
				commands->quit = 1;
				break;

			case SDLK_n:
				*quit = 1;
				*breakout = 1;
				commands->restart = 1;
				break;

			case SDLK_q:
				*quit = 1;
				*breakout = 1;
				commands->menu = 1;
				break;
			}
			break;

		case SDL_QUIT:
			*quit = 1;
			*breakout = 1;
			break;
		}
	}
}


void drawHit(items_t items, screen_t screen)
{
	drawSurface(screen, items.player.sprite, items.player.playerX, items.player.playerY);
	drawString(screen, SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 3, "Continue?", 4.0);
	drawString(screen, SCREEN_WIDTH / 2 - 120 , SCREEN_HEIGHT / 3 + 40, "n - restart", 4.0);
	drawString(screen, SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 3 + 80, "q - back to menu", 4.0);
	drawString(screen, SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 3 + 120, "Esc - quit", 4.0);
	updateScreen(screen);
}


void barrelPhysics(barrel_t** barrel, platform_t* platform, ladder_t* ladder, level_t level, double delta)
{
	for (int i = 0; i < barrelCount; i++)
	{
		if ((*barrel)[i].throwFlag == 0)
			continue;

		if ((*barrel)[i].ladderFlag == 0)
		{
			if (detectBarrelColissionX((*barrel)[i], platform, level, delta) == 1)
				(&(*barrel)[i])->horizontalVelocity = -1.0 * (&(*barrel)[i])->horizontalVelocity;
		}

		gravityBarrel(&(*barrel)[i], platform, level, delta);
	}
}


void playerPhysics(player_t* player, platform_t* platform, ladder_t* ladder, level_t level, double delta)
{
	if (player->ladderFlag == 1 && isOnLadder(*player, ladder, level) != 0) //check if still on ladder
		player->ladderFlag = 0;

	if (player->ladderFlag == 0)
	{
		if (detectTopCollisionY(*player, platform, level) == 1) //make him bounce back from the top
			player->verticalVelocity = 0;
	}

	if (player->ladderFlag == 0)
	{
		if (detectColissionX(player, platform, level, delta) == 1)
			player->horizontalVelocity = 0;
	}

	gravity(player, platform, level);
}


void gravity(player_t* player, platform_t* platform, level_t level)
{
	int colissionFlag = detectBottomColissionY(*player, platform, level);

	if (player->ladderFlag == 0 && colissionFlag == 0)
		player->verticalVelocity += playerGravity;

	if (colissionFlag == 1)
	{
		player->verticalVelocity = 0;
		player->ladderFlag = 0;
		player->jumpFlag = 0;
		player->pointFlag = 1;
	}
}


int detectBottomColissionY(player_t player, platform_t* platform, level_t level)
{
	for (int i = 0; i < level.platformCount; i++)
	{
		int L = platform[i].hitbox.left, R = platform[i].hitbox.right;

		if (player.hitbox.bottom != platform[i].hitbox.top)
			continue;

		else if (checkInRange(player.hitbox.left, L, R) == 1 && checkInRange(player.hitbox.right, L, R) == 1 && checkInRange(player.playerX, L, R) == 1)
			continue;

		else
		{
			return 1;
		}
	}

	return 0;
}


int detectTopCollisionY(player_t player, platform_t* platform, level_t level)
{
	for (int i = 0; i < level.platformCount; i++)
	{
		int L = platform[i].hitbox.left, R = platform[i].hitbox.right;

		if (player.hitbox.top != platform[i].hitbox.bottom)
			continue;

		else if (checkInRange(player.hitbox.left, L, R) == 1 && checkInRange(player.hitbox.right, L, R) == 1 && checkInRange(player.playerX, L, R) == 1)
			continue;

		else
		{
			return 1;
		}
	}

	return 0;
}


int detectColissionX(player_t* player, platform_t* platform, level_t level, double delta)
{
	int playerPos, distance;
	double modifier;

	if (player->horizontalVelocity < 0)
	{
		modifier = -1.0;
		playerPos = player->hitbox.left;
	}

	else if (player->horizontalVelocity >= 0)
	{
		modifier = 1.0;
		playerPos = player->hitbox.right;
	}

	else
	{
		perror("Error - unknown side");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < level.platformCount; i++)
	{
		int T = platform[i].hitbox.top, B = platform[i].hitbox.bottom, L = platform[i].hitbox.left, R = platform[i].hitbox.right;

		if (checkInRange(player->playerY, T, B) == 0 || checkInRange(player->hitbox.top, T, B) == 0)
		{
			if (checkInRange(playerPos + modifier * player->horizontalVelocity * delta, L, R) == 0)
			{
				if (player->hitbox.bottom - T <= platform[i].platformHeight / 2)
				{
					updatePlayerPosition(player, modifier * 3.0, player->hitbox.top - T);
				}

				else
					return 1;
			}
		}
	}

	return 0;
}


int detectBottomBarrelColissionY(barrel_t barrel, platform_t* platform, level_t level, double delta, int* y)
{
	for (int i = 0; i < level.platformCount; i++)
	{
		int L = platform[i].hitbox.left, R = platform[i].hitbox.right, T = platform[i].hitbox.top, B = platform[i].hitbox.bottom;

		if (barrel.hitbox.bottom != platform[i].hitbox.top && checkInRange(barrel.hitbox.bottom + int(barrel.verticalVelocity * delta), T, B) != 0)
			continue;

		else if (checkInRange(barrel.hitbox.left, L, R) == 1 && checkInRange(barrel.hitbox.right, L, R) == 1 && checkInRange(barrel.barrelX, L, R) == 1)
			continue;

		else
		{
			*y = platform[i].hitbox.top - barrel.hitbox.bottom;
			return 1;
		}
	}

	return 0;
}


int detectBarrelColissionX(barrel_t barrel, platform_t* platform, level_t level, double delta)
{
	int barrelPos, distance;
	double modifier;

	if (barrel.horizontalVelocity < 0)
	{
		modifier = -1.0;
		barrelPos = barrel.hitbox.left;
	}

	else if (barrel.horizontalVelocity >= 0)
	{
		modifier = 1.0;
		barrelPos = barrel.hitbox.right;
	}

	else
	{
		perror("Error - unknown side");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < level.platformCount; i++)
	{
		int T = platform[i].hitbox.top, B = platform[i].hitbox.bottom, L = platform[i].hitbox.left, R = platform[i].hitbox.right;

		if (checkInRange(barrel.barrelY, T, B) == 0 || checkInRange(barrel.hitbox.top, T, B) == 0)
		{
			if (checkInRange(barrelPos + modifier * barrel.horizontalVelocity * delta, L, R) == 0)
				return 1;
		}
	}

	return 0;
}


void gravityBarrel(barrel_t* barrel, platform_t* platform, level_t level, double delta)
{
	if (barrel->throwFlag == 1)
	{
		int y, colissionFlag = detectBottomBarrelColissionY(*barrel, platform, level, delta, &y);

		if (barrel->ladderFlag == 0 && colissionFlag == 0)
			barrel->verticalVelocity += barrelGravity;

		if (colissionFlag == 1)
		{
			updateBarrelPosition(barrel, 0, y);
			barrel->verticalVelocity = 0;
			barrel->ladderFlag = 0;
		}
	}
}


int detectPlayerBarrelCollision(barrel_t barrel, player_t player, double delta)
{
	if (checkInRange(barrel.barrelY, player.hitbox.top, player.hitbox.bottom) == 0 ||
		checkInRange(barrel.hitbox.top, player.hitbox.top, player.hitbox.bottom) == 0 ||
		checkInRange(barrel.hitbox.bottom, player.hitbox.top, player.hitbox.bottom) == 0)
	{
		if (checkInRange(barrel.hitbox.left + barrel.horizontalVelocity * delta, player.hitbox.left, player.hitbox.right) == 0 ||
			checkInRange(barrel.hitbox.right + barrel.horizontalVelocity * delta, player.hitbox.left, player.hitbox.right) == 0)
			return 1;
	}

	return 0;
}


int detectPlayerTrophyCollision(items_t* items, level_t level)
{
	for (int i = 0; i < level.trophyCount; i++)
	{
		if (checkInRange(items->player.playerX, items->trophy[i].hitbox.left, items->trophy[i].hitbox.right) == 0 &&
			checkInRange(items->player.playerY, items->trophy[i].hitbox.top, items->trophy[i].hitbox.bottom) == 0)
		{
			updateTrophyPosition(items, i);
			return 1;
		}
	}
	
	return 0;
}


void updateTrophyPosition(items_t* items, int index)
{
	(&items->trophy[index])->trophyY = 0;
	(&items->trophy[index])->trophyX = 0;

	(&items->trophy[index])->hitbox.top = 0;
	(&items->trophy[index])->hitbox.bottom = 0;
	(&items->trophy[index])->hitbox.left = 0;
	(&items->trophy[index])->hitbox.right = 0;

}


int detectPlayerBarrelJump(items_t items)
{
	for (int i = 0; i < barrelCount; i++)
	{
		if (items.barrel[i].throwFlag == 1 && items.player.pointFlag == 1)
		{
			if (checkInRange(items.player.playerX, items.barrel[i].hitbox.left, items.barrel[i].hitbox.right) == 0 &&
				checkInRange(items.player.playerY, items.barrel[i].hitbox.top - 2 * items.barrel[i].barrelHeight, items.barrel[i].hitbox.top) == 0)
			{
				return 1;
			}
		}
	}

	return 0;
}


void handlePoints(items_t* items, level_t level)
{
	if (detectPlayerTrophyCollision(items, level) == 1)
	{
		(&items->player)->points += trophyValue;
		(&items->player)->pointGrabbed = trophyValue;
	}

	if (detectPlayerBarrelJump(*items) == 1)
	{
		(&items->player)->points += barrelValue;
		(&items->player)->pointGrabbed = barrelValue;
		(&items->player)->pointFlag = 0;
	}

}


void resetCommands(commands_t* commands)
{
	commands->menu = 0;
	commands->quit = 0;
	commands->restart = 0;
}


void freeLevelSurfaces(sprites_t* sprites, items_t* items)
{
	freePlayerSprites(sprites);
	freeEnemySprites(sprites);
}


void freePlayerSprites(sprites_t* sprites)
{
	SDL_FreeSurface(sprites->Pleft1);
	SDL_FreeSurface(sprites->Pleft2);

	SDL_FreeSurface(sprites->Pright1);
	SDL_FreeSurface(sprites->Pright2);

	SDL_FreeSurface(sprites->PjumpLeft);
	SDL_FreeSurface(sprites->PjumpRight);

	SDL_FreeSurface(sprites->Pladder1);
	SDL_FreeSurface(sprites->Pladder2);
}


void freeEnemySprites(sprites_t* sprites)
{
	SDL_FreeSurface(sprites->kongBarrel);
	SDL_FreeSurface(sprites->kongStand);
	SDL_FreeSurface(sprites->kongThrow);

	SDL_FreeSurface(sprites->barrel1);
	SDL_FreeSurface(sprites->barrel2);

	SDL_FreeSurface(sprites->live);
}

/*
	static double accumulatedBX = 0.0;
	static double accumulatedBNX = 0.0;

	if (barrel.horizontalVelocity >= 0)
	{
		accumulatedBNX = 0.0;

		accumulatedBX += barrel.horizontalVelocity * delta;

		*barrelDistanceX = accumulatedBX;

		if (*barrelDistanceX > 1.0)
			*barrelDistanceX = 1.0;

		if (accumulatedBX >= 1.0)
			accumulatedBX = 0;
	}

	if (barrel.horizontalVelocity < 0)
	{
		accumulatedBNX += barrel.horizontalVelocity * delta;

		*barrelDistanceX = round(accumulatedBNX);

		if (*barrelDistanceX < -1.0)
			*barrelDistanceX = -1.0;

		if (round(accumulatedBNX) <= -1.0)
			accumulatedBNX = 0;
	}
*/