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

using namespace std;

class background
{
private:

	SDL_Rect position;
	

	double ssx_factor;		//scrolling speed on x, first_plan = 1.0, static = 0.0
	double ssy_factor;		//scrolling speed on y, first_plan = 1.0, static = 0.0

	bool x_loop;			//true if loopable in x, else false
	bool y_loop;			//true if loopable in y, else false

public:
	SDL_Surface *icon;
	string filename;
	void render();
	background(int x, int y, string filename, double ssx, double ssy, bool x_l = false, bool y_l = false);
	~background(void);
};

