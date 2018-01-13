#include "background.h"
#include "jumper.h"
#include "game.h"

extern game ogame;
extern jumper C;

background::background(int x, int y, string filename, double ssx, double ssy,  bool x_l, bool y_l)
{
	this->x_loop = x_l;
	this->y_loop = y_l;

	this->position.x = x;
	this->position.y = y;
	this->icon = ogame.load_image( filename );
	this->ssx_factor = ssx;
	this->ssy_factor = ssy;

	this->filename = filename;
}

background::~background(void)
{
	SDL_FreeSurface( this->icon );
}

void background::render()
{
	SDL_Rect tmp_cam = C.cam.position;

	tmp_cam.x=(int)(tmp_cam.x/this->ssx_factor);
	tmp_cam.y=(int)(tmp_cam.y/this->ssy_factor);

	tmp_cam.y-=this->position.y;
	tmp_cam.x-=this->position.x;

	

	ogame.apply_surface(0,0,this->icon,ogame.screen,&tmp_cam);
}