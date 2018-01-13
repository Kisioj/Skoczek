#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
//#include "SDL_rotozoom.h"
#include <string>
#include <sstream>
#include <cmath>
#include <iostream>
#include <list>
#include "game.h"
#include "camera.h"
//#include "button.h"
using namespace std;


class jumper
{
public:
	SDL_Rect position;

	SDL_Surface* icon_jumper_squat;
	SDL_Surface* icon_jumper_bar;

	SDL_Surface* icon_jumper_fly;
	SDL_Surface* icon_jumper_land;
	SDL_Surface* icon_jumper_fall1;
	SDL_Surface* icon_jumper_fall2;
	SDL_Surface* icon_jumper_out;

	SDL_Surface* icon_skis_fly;
	SDL_Surface* icon_skis_run;

	bool on_bar;				//true before jumper starts sliding
	bool flies;					//true when flying, false when sliding
	bool landed;				//true when already landed
	bool jumped;				//true when user jumped
	double jump_speed;			//speed that jumper had when jumped
	double jump_length;			//length of the jump in meters
	unsigned int fell_down;				//true if jumper fell down after landing, otherwise false
	
	double x_precise,y_precise;	//precise coordinates
	double v;					//velocity
	//double vx;					//velocity x
	//double vy;					//velocity y

	int time;					//initialized when user presses space, number of ticks when last moved, needed for delta_time

	double height;				//height in meters
	double weight;				//weight in kilograms

	double skis_area;			//pole powierzchni ca³kowitej nart = 2.4m * 0.1m * 2 = 0.48m^2
	double jumper_area;			//pole powierzchni calkowitej skoczka = 1.7m(Ma³ysz) * 0.4m = 0.68m^2

	double jumper_alpha;		//angle between jumper and horizont
	double skis_alpha;			//angle between skis and horizont
	double alpha;				//angle between movement direction and horitont


	bool quit;					//tells if jumper closed the game
	camera cam;

	//list<button> ustawienia;

public:
	jumper(void);
	~jumper(void);
	void move();
	void jump();
	void render();
	void reset_variables();
};

