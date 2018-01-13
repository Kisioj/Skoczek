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
#include "game.h"

class camera
{

public:
	SDL_Rect position;
	camera(void);
	void update(SDL_Rect position);
};

	
