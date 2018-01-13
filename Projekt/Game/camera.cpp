#include "camera.h"

extern game ogame;



camera::camera(void){
		this->position.x = 0;
		this->position.y = 0;
		this->position.w = SCREEN_WIDTH;
		this->position.h = SCREEN_HEIGHT;
}


void camera::update(SDL_Rect position){

    this->position.x = ( position.x + position.w / 2 ) - this->position.w / 2;
    this->position.y = ( position.y + position.h / 2 ) - this->position.h / 2;
    
    //Keep the camera in bounds.
    if( this->position.x < 0 )
   {
        this->position.x = 0;    
    }
    if( this->position.y < 0 )
    {
        this->position.y = 0;    
    }
	if( this->position.x > ogame.skocznia->level_width - this->position.w )
    {
        this->position.x = ogame.skocznia->level_width - this->position.w;    
    }
	if( this->position.y > ogame.skocznia->level_height - this->position.h )
    {
		this->position.y = ogame.skocznia->level_height - this->position.h;    
    } 
}