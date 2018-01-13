//Include SDL functions and datatypes
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
#include "background.h"
#include "jumper.h"

extern game ogame;




int main( int argc, char* args[] )
{
	#if SDL_VERSION_ATLEAST(2,0,0)
	cout<<"ugh";
	#else
	cout<<"agh";
	#endif
	ogame.cycle();
    return 0;    
}