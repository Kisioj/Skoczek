#include "slope.h"
#include "game.h"

extern game ogame;
//podane w metrach
slope::slope(double x, double y, double h, double w, int przesuniecie)
{
	this->position.x = (int)(x*METER_TO_PIXEL);
	this->position.y = (int)(y*METER_TO_PIXEL) + przesuniecie;
	this->position.h = (int)(h*METER_TO_PIXEL);
	this->position.w = (int)(w*METER_TO_PIXEL);
}

