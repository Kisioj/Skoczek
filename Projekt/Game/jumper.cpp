#include "jumper.h"
#include "hill.h"

extern game ogame;
//extern hill CH;

void jumper::reset_variables(){
	
	ogame.wind_vx = ((rand() % 401)-200.0)/100.0;
	ogame.wind_vy = ((rand() % 401)-200.0)/100.0;

	if(ogame.skocznia->name=="Gawronowo") ogame.FRICTION_FACTOR = 0.06;
	else if(ogame.skocznia->name=="Jurowo") ogame.FRICTION_FACTOR = 0.25;
	
	ogame.skok_zakonczony = false;
	ogame.green_light = false;
	ogame.red_light_start = SDL_GetTicks();
	ogame.wind_change_time = SDL_GetTicks();

	this->position.x = ogame.skocznia->start_x;
	this->position.y = ogame.skocznia->start_y;

	this->alpha = ogame.skocznia->gradients[this->position.x];

	this->x_precise = this->position.x;
	this->y_precise = this->position.y;

	this->flies = false;
	this->landed = false;
	this->on_bar = true;
	this->jumped = false;
	this->jump_speed = 0.0;
	this->jump_length = 0.0;

	this->v = 2.0;

	this->jumper_alpha = -90.0/180.0*PI;

	this->fell_down = 0;

	this->quit = false;
	
}

jumper::jumper(void)
{

	this->position.h = 9;//2
	this->position.w = 64;//40

	this->x_precise = this->position.x;
	this->y_precise = this->position.y;

	this->icon_jumper_bar = ogame.load_image("grafika/skoczek/1.png");
	this->icon_jumper_squat = ogame.load_image("grafika/skoczek/2.png");
	this->icon_jumper_fly = ogame.load_image( "grafika/skoczek/3.png" );
	this->icon_jumper_land = ogame.load_image( "grafika/skoczek/4.png" );
	this->icon_jumper_fall1 = ogame.load_image( "grafika/skoczek/5.png" );
	this->icon_jumper_fall2 = ogame.load_image( "grafika/skoczek/6.png" );
	this->icon_jumper_out = ogame.load_image( "grafika/skoczek/7.png" );

	this->icon_skis_run = ogame.load_image("grafika/skoczek/skis1.png");
	this->icon_skis_fly = ogame.load_image("grafika/skoczek/skis2.png");

	this->flies = false;
	this->landed = false;
	this->on_bar = true;
	this->jumped = false;
	this->jump_speed = 0.0;
	this->jump_length = 0.0;
	
	//x_precise = 0.0;
	//y_precise = 0.0;
	this->v = 2.0;					//velocity
	//alpha_jumper = ogame.gradients[position.x];		//alpha degree of the jumper
	//alpha_skis = ogame.gradients[position.x];			//alpha degree of the skis


	this->height = 1.7;				//height in meters (of Adam Ma³ysz)
	this->weight = 61;				//weight in kilograms (of Adam Ma³ysz)

	this->skis_area = 0.48;
	this->jumper_area = 0.68;

	this->jumper_alpha = -90.0/180.0*PI;

	this->fell_down = 0;

	this->quit = false;
}

jumper::~jumper(void)
{
	SDL_FreeSurface( this->icon_jumper_bar );
	SDL_FreeSurface( this->icon_jumper_squat );
	SDL_FreeSurface( this->icon_jumper_fly );
	SDL_FreeSurface( this->icon_jumper_land );
	SDL_FreeSurface( this->icon_jumper_out );
	SDL_FreeSurface( this->icon_skis_run );
	SDL_FreeSurface( this->icon_skis_fly );
}

void jumper::move()
{
	if(!this->on_bar)
	{
		double t = (SDL_GetTicks() - time) / 1000.0; //delta time
		time = SDL_GetTicks();
		if(t > 0.1){t = 0.00;}	//it means that window was moved, then game should be stopped, should be if(t > 0.01)

		double a = G * sin(this->alpha);

		//odleg³oœæ jak¹ pokona skoczek
		double s = this->v * t + a * pow(t,2.0) / 2;
		

		if(this->position.x >= ogame.skocznia->slow_down_from){
			ogame.FRICTION_FACTOR = 2.0;
		}

		if(this->position.x >= ogame.skocznia->stop_from)
		{
			s = 0.0;
			a = 0.0;
			this->v = 0.0;
			this->x_precise = this->position.x;
			this->y_precise = this->position.y;
		}
		if(!v){
			ogame.skok_zakonczony = true;
			if(ogame.id_zawodnika==ogame.lista_zawodnikow.size()) {
				if(ogame.rekord_skoczni_type==1) ogame.rekord_skoczni_type=2;
				ogame.zawody_info = true;
			}
		}
		
		//user should slide from the hill if not jumped
		if(!this->flies && !this->landed && !this->jumped && this->position.x >= ogame.skocznia->rozbieg_width-1)
		{
			this->flies = true;
			this->jump_speed = this->v*3.6;
		}


		if(this->flies)
		{
			//rozk³adamy wektor prêdkoœci v na wektory sk³adowe vx i vy
			double vx = this->v * cos(this->alpha);
			double vy = this->v * sin(this->alpha);


			///////////////////////////////
			////////SI£A CIÊ¯KOŒCI/////////
			///////////////////////////////
			double weight_F = this->weight * G;
			double weight_ay = weight_F / this->weight;
			vy += weight_ay*t;
			//----------------KONIEC SI£Y CIÊ¯KOŒCI-----------------//



			///////////////////////////////
			/////////SI£A WIATRU///////////
			///////////////////////////////			
			double ro = 1.2; //gêstoœæ powietrza
			
			//wind_v - skladowa wektora predkosci w kierunku przeciwnym do kierunku lotu skoczka
			double wind_v = sqrt( pow(ogame.wind_vx+ogame.wind_vx_change, 2.0) + pow(ogame.wind_vy+ogame.wind_vy_change, 2.0) );
			double wind_angle = asin( (ogame.wind_vy+ogame.wind_vy_change)/wind_v);
			if(ogame.wind_vy < 0 && ogame.wind_vx < 0){wind_angle = - (180*PI/180) - wind_angle;}
			else if(ogame.wind_vy >= 0 && ogame.wind_vx < 0){wind_angle = (180*PI/180) - wind_angle;}
			
			wind_v *= abs(cos(wind_angle-this->alpha));
			if(ogame.wind_vy<0){wind_v=-wind_v;}
			//----------------KONIEC WIATRU-----------------//
			



			///////////////////////////////
			//////////SI£A NOŒNA///////////
			///////////////////////////////	
			double Cx = 10.0; //wspolczynnik sily nosnej
			double lift_F = 0;
			double kat = 0;

			//w przypadku, gdy skoczek nie wyskocy z progu, nie osiaga sily nosnej
			if(!this->jumped) {Cx = 0;} 
			
			kat = abs(this->alpha - jumper_alpha);
			if(kat < PI/6) Cx *= kat*3;
			else if(kat < PI/3) Cx *= (PI - kat*3);
			else Cx=0;	
			lift_F = abs(sin(this->alpha - jumper_alpha)) * (this->v - wind_v) * ro * jumper_area * Cx;

			kat = abs(this->alpha - skis_alpha);
			if(kat < PI/6) Cx *= kat*3;
			else if(kat < PI/3) Cx *= (PI - kat*3);
			else Cx=0;	
			lift_F += abs(sin(this->alpha - skis_alpha)) * (this->v - wind_v) * ro * skis_area * Cx;

			double lift_ay = lift_F * abs(sin(jumper_alpha)) / this->weight;
			double lift_ax = lift_F * abs(cos(jumper_alpha)) / this->weight;

			vy -= lift_ay * t;
			vx += lift_ax * t;
			//----------------KONIEC SI£Y NOŒNEJ-----------------//




			//znow skladamy wektor predkosci
			this->v = sqrt( pow(vx, 2.0) + pow(vy, 2.0) );

			this->alpha = asin(vy/v);


			///////////////////////////////
			//////OPÓR AERODYNAMICZNY//////
			///////////////////////////////	
			double S = abs(sin(this->alpha - skis_alpha)) * skis_area + abs(sin(this->alpha -jumper_alpha)) * jumper_area;
			
			//double Cd = 1.8;
			double Cd = (3.0 - 3.0*( abs(90.0 - fmod((abs(this->alpha - skis_alpha)*180.0/PI),180.0))/90.0 )) * skis_area/(skis_area+jumper_area);
			Cd+= (3.0 - 3.0*( abs(90.0 - fmod((abs(this->alpha - jumper_alpha)*180.0/PI),180.0))/90.0 )) * jumper_area/(skis_area+jumper_area);
			
			double drag_F = 0.5 * ro * pow(this->v,2.0) * S * Cd;

			double drag_a = drag_F / this->weight;

			//cout << "Cd("<<Cd<<") Opor(" <<drag_a << ") S(" <<S << ")" << endl;	

			this->v -= drag_a*t;
			//----------------KONIEC OPORU AERODYNAMICZNEGO-----------------//



			//vy = this->v * sin(this->alpha);
			//cout << "before alpha:" << this->alpha*180.0/PI << endl;
			//this->alpha = asin(vy/v);
			//cout << "vy: " << vy << " v:" << v << endl;
			//cout << "after alpha:" << this->alpha*180.0/PI << endl;
		}
		else
		{
			
			//a = v/t
			//T = FRICTION_FACTOR * Fn
			//

			//this->v += G * (sin(this->alpha) - FRICTION_FACTOR * cos(this->alpha) ) * t;
			//this->v += a*t - ogame.FRICTION_FACTOR * this->v * cos(this->alpha) * t;//Michala Gawrona
			double tarcie = ogame.FRICTION_FACTOR * G * cos(this->alpha) * t;
			if(tarcie > v + a*t) tarcie = v + a*t;
			this->v += a*t - tarcie;

			this->alpha = ogame.skocznia->gradients[this->position.x];
		}
		

		this->x_precise += cos(this->alpha) * s * METER_TO_PIXEL;
		this->y_precise += sin(this->alpha) * s * METER_TO_PIXEL;

		//check if our guy is trying to leave map bounds
		if( (int)this->x_precise + this->position.w > ogame.skocznia->level_width){this->x_precise = ogame.skocznia->level_width - this->position.w;}
		else if( (int)this->x_precise < 0){this->x_precise = 0.0;}
		if( (int)this->y_precise > ogame.skocznia->level_height){this->y_precise = ogame.skocznia->level_height;}
		else if( (int)this->y_precise - this->position.h < 0){this->y_precise = this->position.h;}

		//check for collisions and do not let for them
		if(this->flies)
		{
			while( double slope_y = ogame.collision_check(this->x_precise, this->y_precise) )
			{
				
				if(!slope_y){this->y_precise-=1.0;}
				
				else break;
			}
		}
		
		//cout << "x: " << this->position.x << ", y:" << this->position.y << endl;
		this->position.x = (int)this->x_precise;
		this->position.y = (int)this->y_precise;

		if(this->fell_down){
			double s_nart = ogame.narty_po_upadku.v * t + a * pow(t,2.0) / 2;
			
			double tarcie_nart = ogame.narty_po_upadku.FRICTION_FACTOR * G * cos(ogame.narty_po_upadku.alpha) * t;
			if(tarcie_nart > ogame.narty_po_upadku.v + a*t) tarcie_nart = ogame.narty_po_upadku.v + a*t;
			ogame.narty_po_upadku.v += a*t - tarcie_nart;

			ogame.narty_po_upadku.alpha = ogame.skocznia->gradients[ogame.narty_po_upadku.position.x];

			ogame.narty_po_upadku.x_precise += cos(ogame.narty_po_upadku.alpha) * s_nart * METER_TO_PIXEL;
			ogame.narty_po_upadku.y_precise += sin(ogame.narty_po_upadku.alpha) * s_nart * METER_TO_PIXEL;

			if( (int)ogame.narty_po_upadku.x_precise + ogame.narty_po_upadku.position.w > ogame.skocznia->level_width){ogame.narty_po_upadku.x_precise = ogame.skocznia->level_width - ogame.narty_po_upadku.position.w;}
			
			ogame.narty_po_upadku.position.x = (int)ogame.narty_po_upadku.x_precise;
			ogame.narty_po_upadku.position.y = (int)ogame.narty_po_upadku.y_precise;

		}


	}

}



void jumper::render()
{
	int x = this->position.x - cam.position.x;
	int y = this->position.y - cam.position.y;

	SDL_Surface* temp_jumper;
	SDL_Surface* temp_ski;

	/*
	double j_kat_sr_pkt = atan ( abs((61 - 72 / 2) * 1.0) / abs((28 - 78 / 2)) * 1.0 ); 
	double j_odleg_srodek_punkt = abs((61 - 72 / 2)) / sin(j_kat_sr_pkt);
	j_kat_sr_pkt = PI- j_kat_sr_pkt;

	double s_kat_sr_pkt = atan ( abs((4 - 10 / 2) * 1.0) / abs((23 - 78 / 2)) * 1.0 );
	double s_odleg_srodek_punkt = abs((4 - 10 / 2)) / sin(s_kat_sr_pkt);
	s_kat_sr_pkt = PI- s_kat_sr_pkt; */

	double j_kat_sr_pkt = 1.9853;
	double j_odleg_srodek_punkt = 27.313;

	double s_kat_sr_pkt = 3.07917;
	double s_odleg_srodek_punkt = 16.0312;

	//on the bar
	if(this->on_bar)
	{
		temp_jumper = this->icon_jumper_bar;
		temp_ski = this->icon_skis_run;

		s_kat_sr_pkt += this->alpha;

		temp_jumper = rotozoomSurface(temp_jumper, 0, 1, 1);
		temp_ski = rotozoomSurface(temp_ski, -this->alpha*180.0/PI, 1, true);
	}

	//sliding down the inrun or flies without jump
	else if((!this->flies && !this->landed) || (this->flies && !this->jumped))
	{
		temp_jumper = this->icon_jumper_squat;
		temp_ski = this->icon_skis_run;

		j_kat_sr_pkt += this->alpha;
		s_kat_sr_pkt += this->alpha;
		
		temp_jumper = rotozoomSurface(temp_jumper, -this->alpha*180.0/PI, 1, 1);
		temp_ski = rotozoomSurface(temp_ski, -this->alpha*180.0/PI, 1, true);
	}

	//flies after jump
	else if(this->flies && this->jumped)
	{
		temp_jumper = this->icon_jumper_fly;
		temp_ski = this->icon_skis_fly;
	
		j_kat_sr_pkt += this->jumper_alpha + PI/2;
		s_kat_sr_pkt += this->skis_alpha;
		
		temp_jumper = rotozoomSurface(temp_jumper, -this->jumper_alpha*180.0/PI -90, 1, 1);	
		temp_ski = rotozoomSurface(temp_ski, -this->skis_alpha*180.0/PI, 1, true);							
	}

	//landed successfully
	else if(this->landed && !this->fell_down && this->position.x <= ogame.skocznia->slow_down_from )
	{
		temp_jumper = this->icon_jumper_land;
		temp_ski = this->icon_skis_run;	

		j_kat_sr_pkt += this->alpha;
		s_kat_sr_pkt += this->alpha;
		
		temp_jumper = rotozoomSurface(temp_jumper, -this->alpha*180.0/PI, 1, 1);
		temp_ski = rotozoomSurface(temp_ski, -this->alpha*180.0/PI, 1, true);								
	}

	//slowing down on outrun after successful landing
	else if(this->landed && !this->fell_down && this->position.x > ogame.skocznia->slow_down_from )
	{
		temp_jumper = this->icon_jumper_out;
		temp_ski = this->icon_skis_run;

		s_kat_sr_pkt += this->alpha;
		
		temp_jumper = rotozoomSurface(temp_jumper, 0, 1, 1);
		temp_ski = rotozoomSurface(temp_ski, -this->alpha*180.0/PI, 1, true);
	} 

	//fell down on his back
	else if(this->landed && (this->fell_down==1 || this->fell_down==3) )
	{
		temp_jumper = this->icon_jumper_fall1;
		temp_ski = this->icon_skis_run;

		j_kat_sr_pkt += this->alpha;
		s_kat_sr_pkt += ogame.narty_po_upadku.alpha;
		
		temp_jumper = rotozoomSurface(temp_jumper, -this->alpha*180.0/PI, 1, 1);
		temp_ski = rotozoomSurface(temp_ski, -ogame.narty_po_upadku.alpha*180.0/PI, 1, true);
	} 

	//fell down on his chest
	else if(this->landed && this->fell_down==2)
	{
		temp_jumper = this->icon_jumper_fall2;
		temp_ski = this->icon_skis_run;

		j_kat_sr_pkt += this->alpha;
		s_kat_sr_pkt += ogame.narty_po_upadku.alpha;
		
		temp_jumper = rotozoomSurface(temp_jumper, -this->alpha*180.0/PI, 1, 1);
		temp_ski = rotozoomSurface(temp_ski, -ogame.narty_po_upadku.alpha*180.0/PI, 1, true);
	} 


	double x_offset = cos(j_kat_sr_pkt) * j_odleg_srodek_punkt + temp_jumper->w/2; 
	double y_offset = sin(j_kat_sr_pkt) * j_odleg_srodek_punkt + temp_jumper->h/2; 
	ogame.apply_surface(x - (int)x_offset, y - (int)y_offset, temp_jumper, ogame.screen);
	
	x_offset = cos(s_kat_sr_pkt) * s_odleg_srodek_punkt + temp_ski->w/2; 
	y_offset = sin(s_kat_sr_pkt) * s_odleg_srodek_punkt + temp_ski->h/2;

	if(this->fell_down){
		x = ogame.narty_po_upadku.position.x - cam.position.x;
		y = ogame.narty_po_upadku.position.y - cam.position.y;
	}
	ogame.apply_surface(x - (int)x_offset, y - (int)y_offset, temp_ski, ogame.screen);

	SDL_FreeSurface(temp_jumper);
	SDL_FreeSurface(temp_ski);

}



void jumper::jump()
{
	if(!this->flies)
	{
		this->flies = true;
		this->jumped = true;
			
		double vx = this->v * cos(this->alpha);
		double vy = this->v * sin(this->alpha);

		//vx -= 1;  // podczas wyskoku, z powodu zwiekszonego tarcia predkosc maleje o od 0.6 do 1.8 km/h
		//vy -= 3;
		
		this->jump_speed = this->v*3.6;
		//cout << "JUMP!" << " with SPEED: " << this->v*3.6 << "km/h" << endl;
		cout << "Jump at " << this->position.x << endl;

		this->v = sqrt( pow(vx, 2.0) + pow(vy, 2.0) );
		 //podczas idelanego wyskoku z progru, skoczek podnosi kat toru ruchu o 5 stopni
		this->alpha = asin(vy/v) - 0.127;//0.087
		//v-=2;//przez duzy docisk

		//narazie ustawiamy na sztywno katy nart i skoczka
		this->jumper_alpha = -asin(4.0/9.0);
		this->skis_alpha = this->alpha;//-asin(2.0/9.0);
	}
}



//311.06metra - 5160pixeli



//1 metr = 16.5884395 pixeli

//od poczatku skoczni:
/*
5.54 metra - 90 stopni
42.33 metra - 37.1 stopni
4.81 metra - 34.7 stopni
4.46 metra - 29.6 stopni
4.66 metra - 26.6 stopni
5.77 metra - 23.7 stopni
5.97 metra - 18.7 stopni
6.93 metra - 16.0 stopni

2.28 metra - 13.5 stopni
2.76 metra - 11.2 stopni
*/


