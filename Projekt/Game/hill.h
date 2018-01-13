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
#include "background.h"
#include "slope.h"
using namespace std;

class hill
{
public:
	background *skoczn; //grafika/skocznie/hill.png & big_hill.png
	background *blue;
	background *blue2;
	double rekord;		//rekord skoczni liczony w metrach
	string rekordzista; //nazwa gracza, do ktorego nalezy rekord skoczni
	int level_width;
	int level_height;
	int rozbieg_width;
	int skocznia_przesuniecie;
	int punkt_k;
	string name;

	int can_jump_from;//od jakies odleglosci na rozbiegu skoczek moze skakac
	int slow_down_from;//od jakiej odleglosci po wyladowaniu skoczek ma hamowac
	int stop_from;//na jakiej odleglosci skoczek powinien sie zatrzymac

	int start_x;//x coordinate where jumper starts
	int start_y;//y coordinate where jumper starts

	double gradients[7100]; // = double gradients[level_width]
	list<slope> slopes;


	void render();
	hill(){};
	hill(string tmp_name);
	~hill(void);
};


/* SKOCZNIA MALA GAWRONOWO

//The dimensions of the level
const int level_width = 5160;//1366;
const int level_height = 2280;//2430;//768;

//Width in pixels of the rozbieg
const int rozbieg_width = 1419;
//PRzesuniecie skonczni w dol
const int skocznia_przesuniecie = 80;//230


for(int i = 1; i < level_width; i ++)
	{
		this->gradients[i] = 0.0*RADIAN;//atan(5.0/5.0);
		if(i <= 91){this->gradients[i] = 0.0 * RADIAN;}
		else if(i <= 793){gradients[i] = 37.1 * RADIAN;}
		else if(i <= 873){gradients[i] = 34.7 * RADIAN;}
		else if(i <= 947){gradients[i] = 29.6 * RADIAN;}
		else if(i <= 1024){gradients[i] = 26.6 * RADIAN;}
		else if(i <= 1120){gradients[i] = 23.7 * RADIAN;}
		else if(i <= 1219){gradients[i] = 18.7 * RADIAN;}
		else if(i <= 1334){gradients[i] = 16.0 * RADIAN;}
		else if(i <= 1371){gradients[i] = 13.5 * RADIAN;}
		else if(i <= 1417){gradients[i] = 11.2 * RADIAN;}

		else if(i <= 1591){gradients[i] = 11.0 * RADIAN;}
		else if(i <= 1744){gradients[i] = 17.8 * RADIAN;}
		else if(i <= 1988){gradients[i] = 26.9 * RADIAN;}
		else if(i <= 2524){gradients[i] = 32.32 * RADIAN;}
		else if(i <= 3139){gradients[i] = 33.9 * RADIAN;}
		else if(i <= 3216){gradients[i] = 31.2 * RADIAN;}
		else if(i <= 3519){gradients[i] = 21.1 * RADIAN;}
		else if(i <= 3670){gradients[i] = 17.0 * RADIAN;}
		else if(i <= 3919){gradients[i] = 11.2 * RADIAN;}
	}

	//1 metr = 16.58 pixeli
	slope S01(85.51, 51.31, 2.04, 10.50);
	slope S02(96.01, 53.35, 2.95, 9.18);
	slope S03(105.20, 56.30, 7.48, 14.76);
	slope S04(119.96, 63.78, 20.40, 32.34);
	slope S05(152.29, 84.18, 24.89, 37.09);
	slope S06(189.38, 109.07, 2.80, 4.63);
	slope S07(194.01, 111.88, 7.05, 18.24);
	slope S08(212.26, 118.93, 2.80, 9.13);
	slope S09(221.39, 121.73, 2.99, 15.02);
	slope S10(236.41, 124.72, 0.00, 74.65);

	slopes.push_back(S01);
	slopes.push_back(S02);
	slopes.push_back(S03);
	slopes.push_back(S04);
	slopes.push_back(S05);
	slopes.push_back(S06);
	slopes.push_back(S07);
	slopes.push_back(S08);
	slopes.push_back(S09);
	slopes.push_back(S10);


*/







/* SKOCZNIA MAMUCIA JUROWO

const int level_width = 7069;
const int level_height = 3448;
const int rozbieg_width = 2290;
const int skocznia_przesuniecie = 0;

// skocznia mamucia K-180	
for(int i = 1; i < level_width; i ++)
	{

	    if(i <= 26.75 * METER_TO_PIXEL){this->gradients[i] = 0.0 * RADIAN;}
		else if(i <= 100.94 * METER_TO_PIXEL){gradients[i] = 36.3 * RADIAN;}
		else if(i <= 105.81 * METER_TO_PIXEL){gradients[i] = 33.8 * RADIAN;}
		else if(i <= 110.73 * METER_TO_PIXEL){gradients[i] = 30.5 * RADIAN;}
		else if(i <= 115.65 * METER_TO_PIXEL){gradients[i] = 26.7 * RADIAN;}
		else if(i <= 120.56 * METER_TO_PIXEL){gradients[i] = 21.7 * RADIAN;}
		else if(i <= 126.35 * METER_TO_PIXEL){gradients[i] = 17.0 * RADIAN;}
		else if(i <= 138.18 * METER_TO_PIXEL){gradients[i] = 12.8 * RADIAN;}
		else if(i <= 148.18 * METER_TO_PIXEL){gradients[i] = 10.0 * RADIAN;}
		else if(i <= 156.99 * METER_TO_PIXEL){gradients[i] = 18.0 * RADIAN;}

		else if(i <= 171.47 * METER_TO_PIXEL){gradients[i] = 27.0 * RADIAN;} // 1
		else if(i <= 189.12 * METER_TO_PIXEL){gradients[i] = 32.1 * RADIAN;} // 2
		else if(i <= 211.68 * METER_TO_PIXEL){gradients[i] = 36.5 * RADIAN;} // 3
		else if(i <= 276.02 * METER_TO_PIXEL){gradients[i] = 36.5 * RADIAN;} // 4
		else if(i <= 290.13 * METER_TO_PIXEL){gradients[i] = 32.6 * RADIAN;} // 5
		else if(i <= 308.16 * METER_TO_PIXEL){gradients[i] = 21.4 * RADIAN;} // 6
		else if(i <= 316.44 * METER_TO_PIXEL){gradients[i] = 16.6 * RADIAN;} // 7
		else if(i <= 330.55 * METER_TO_PIXEL){gradients[i] = 11.2 * RADIAN;} // 8
		else if(i <= 339.63 * METER_TO_PIXEL){gradients[i] = 1.5 * RADIAN;} // 9
		else {gradients[i] = 0.0 * RADIAN;}
	}

	//1 metr = 16.58 pixeli
	//po³ozenie x,y
	slope S01(138.18, 90.95, 1.76, 10.0);
	slope S02(148.18, 92.71, 2.86, 8.82);
	slope S03(156.99, 95.57, 7.36, 14.48);
	slope S04(171.47, 102.93, 11.06, 17.65);
	slope S05(189.12, 113.99, 16.69, 22.56);
	slope S06(211.68, 130.68, 47.61, 64.34);
	slope S07(276.02, 178.30, 9.03, 14.12);
	slope S08(290.13, 187.33, 7.08, 18.03);
	slope S09(308.16, 194.41, 2.47, 8.28);
	slope S10(316.44, 196.88, 2.79, 14.11);
	slope S11(330.55, 199.67, 0.23, 9.08);
	slope S12(339.63, 199.90, 0, 86.72);

	slopes.push_back(S01);
	slopes.push_back(S02);
	slopes.push_back(S03);
	slopes.push_back(S04);
	slopes.push_back(S05);
	slopes.push_back(S06);
	slopes.push_back(S07);
	slopes.push_back(S08);
	slopes.push_back(S09);
	slopes.push_back(S10);
	slopes.push_back(S11);
	slopes.push_back(S12);

*/