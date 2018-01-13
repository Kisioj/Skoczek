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
using namespace std;



class button
{
	public:
		//attribues that show where on the sprite is button up state and down state (could be different icons)
		SDL_Rect up;
		SDL_Rect down;

		//The attributes of the button
		SDL_Rect box;
	    
		//The part of the button sprite sheet that will be shown
		SDL_Rect* clip;

		SDL_Surface* icon;
		string name;

		bool visible;
public:
	 //Handles events and set the button's sprite region
    bool handle_events();
    
    //Shows the button on the screen
    void render();
	button(int x, int y, int w, int h, string s/*, bool visibility = true*/);
	~button(void);
};
