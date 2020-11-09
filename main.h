#include <SDL2/SDL_mixer.h>
#include "../Library/loadpng.h"
#include <GL/glut.h>
#include <time.h>

#define MAX_COMBO 10

#include "sound.c"

void Mouse_Game(int, int, int, int);
void Timer(int);
int Timer_Block_Fall();
int Timer_Block_Zoom();
void Mouse_Menu(int, int, int, int);
void Passive_Menu(int, int);
void Main_Menu();
void Timer_Game_Over(int);
