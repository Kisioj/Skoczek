#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include <string>
#include <sstream>
#include <cmath>
#include <iostream>
#include <list>
//#include "game.h"
using namespace std;

class slope
{
public:
	SDL_Rect position;
	slope(double x, double y, double h, double w, int przesuniecie = 0);
};