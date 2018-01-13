#include "game.h"
#include "background.h"
#include "jumper.h"
#include "button.h"
#include "hill.h"


game ogame;
//hill CH;//current_hill  CH("Jurowo")
jumper C;

char savefile[20] = "save.txt";
ofstream w_save;
ifstream r_save;

bool is_empty(std::ifstream& pFile)
{
    return pFile.peek() == std::ifstream::traits_type::eof();
}

void game::odczyt_danych(){

	r_save.open(savefile);
	r_save.seekg(0, ios::end);
	int length = r_save.tellg();

	if(length!=0){
		r_save.seekg(0, ios::beg);

		r_save >> ogame.lista_skoczni["Gawronowo"]->rekordzista >> ogame.lista_skoczni["Gawronowo"]->rekord;
		r_save >> ogame.lista_skoczni["Jurowo"]->rekordzista >> ogame.lista_skoczni["Jurowo"]->rekord;

		gracz G;
		while(!r_save.eof()){
			string name;
			bool is_computer = NULL;
			short int level = NULL;

			r_save >> G.name >> G.is_computer >> G.level;
			//cout << "\n{" << G.name << G.is_computer << G.level << "}";
			lista_zawodnikow.push_back(G);

		}
		r_save.close();
	}
}

void game::zapis_danych(){
	w_save.open(savefile);

	w_save << ogame.lista_skoczni["Gawronowo"]->rekordzista << " " << ogame.lista_skoczni["Gawronowo"]->rekord << " ";
	w_save << ogame.lista_skoczni["Jurowo"]->rekordzista << " " << ogame.lista_skoczni["Jurowo"]->rekord << "\n\n";

	ogame.lista_zawodnikow.sort(porownaj_numery_startowe);
	int i = 0;
	for (list<gracz>::iterator it=ogame.lista_zawodnikow.begin(); it != ogame.lista_zawodnikow.end(); ++it){
		w_save << (*it).name << " " << (*it).is_computer << " " << (*it).level;
		i++; if( i!=ogame.lista_zawodnikow.size() ) w_save << "\n";
	}
	w_save.close();

}



void game::render_Text(int x, int y, SDL_Surface* message, TTF_Font* font, stringstream& stream, SDL_Color& textColor, SDL_Color& outlineColor, int outline_type, bool UTF8){
	
	if(!UTF8) message = TTF_RenderText_Solid( font, stream.str().c_str(), outlineColor );
	else message = TTF_RenderUTF8_Solid( font, stream.str().c_str(), outlineColor );
	if(outline_type==1){//cien
		ogame.apply_surface(x+1,y+1,message,ogame.screen);
		SDL_FreeSurface (message);
	}else if(outline_type==2){
		ogame.apply_surface(x-1,y,message,ogame.screen);
		ogame.apply_surface(x+1,y,message,ogame.screen);
		ogame.apply_surface(x,y-1,message,ogame.screen);
		ogame.apply_surface(x,y+1,message,ogame.screen);
		SDL_FreeSurface (message);
		
	}
	

	if(!UTF8) message = TTF_RenderText_Solid( font, stream.str().c_str(), textColor );
	else message = TTF_RenderUTF8_Solid( font, stream.str().c_str(), textColor );
	
	ogame.apply_surface(x,y,message,ogame.screen);
	SDL_FreeSurface (message); stream.str(""); stream.clear();
}

bool porownaj_wyniki (gracz player_1, gracz player_2)
{
	double wynik1 = player_1.wynik;
	double wynik2 = player_2.wynik;
	if(wynik1>wynik2) return 1;
	else return 0;
}

bool porownaj_numery_startowe (gracz player_1, gracz player_2)
{
	double numer1 = player_1.numer_startowy;
	double numer2 = player_2.numer_startowy;
	if(numer1>numer2) return 0;
	else return 1;
}

void game::render_HUD()
{
	//change wind velocity a little every at least 3000 ticks
	if(SDL_GetTicks() - this->wind_change_time > 3000)
	{
		this->wind_change_time = SDL_GetTicks();
		
		this->wind_vx_change = (rand() % ((int)abs(this->wind_vx*20)+1) ) - (int)(this->wind_vx*10);
		this->wind_vx_change = this->wind_vx_change/100.0;
		if( (this->wind_vx >= 0 && this->wind_vx_change < 0) || (this->wind_vx <= 0 && this->wind_vx_change > 0) )
		{this->wind_vx_change = -this->wind_vx_change;}

		this->wind_vy_change = (rand() % ((int)abs(this->wind_vy*20)+1) ) - (int)(this->wind_vy*10);
		this->wind_vy_change = this->wind_vy_change/100.0;
		if( (this->wind_vy >= 0 && this->wind_vy_change < 0) || (this->wind_vy <= 0 && this->wind_vy_change > 0) )
		{this->wind_vy_change = -this->wind_vy_change;}
	}
	double wind_v = sqrt( pow(this->wind_vx+wind_vx_change, 2.0) + pow(this->wind_vy+wind_vy_change, 2.0) );
	double wind_angle = asin( (wind_vy+wind_vy_change)/wind_v)*180.0/PI ;
	if(this->wind_vy < 0 && this->wind_vx < 0){wind_angle = - 180 - wind_angle;}
	else if(this->wind_vy >= 0 && this->wind_vx < 0){wind_angle = 180 - wind_angle;}

	this->apply_surface(SCREEN_WIDTH-90,10,this->wind_bg,this->screen);

	SDL_Surface* temp_arrow = this->wind_arrow;
	
	temp_arrow = rotozoomSurface(temp_arrow, -wind_angle, 1.0, 1);
	this->apply_surface(SCREEN_WIDTH-72+(int)(22.0-temp_arrow->w/2.0), 20+(int)(22.0-temp_arrow->h/2.0),temp_arrow,this->screen);
	SDL_FreeSurface(temp_arrow);

	stream << ((int)(wind_v*100))/100.0 << " m/s";
	render_Text(SCREEN_WIDTH-80,70,message, font_vgafixe_10,stream,textColor,blackColor,1);

	if(C.flies || C.jumped || C.landed){
		ogame.apply_surface(2,2,ramka,this->screen);
		stream << "Prêdkoœæ na progu: " << ((int)(C.jump_speed*100))/100.0 << " km/h";
		render_Text(5,5,message, font_vgafixe_10,stream,textColor,blackColor,2);
	}
	if (C.landed){
		ogame.apply_surface(2,22,ramka,this->screen);
		stream << "Skok na odleg³oœæ: " << ((int)(C.jump_length*100))/100.0 << " m";
		render_Text(5,23,message, font_vgafixe_10,stream,textColor,blackColor,2);
	}




	if(C.on_bar)
	{
		this->apply_surface(SCREEN_WIDTH-140,10,this->sygnalisator,this->screen);

		if(/*!this->green_light*/ SDL_GetTicks() - red_light_start < 1800)
		{
			SDL_Rect q = {0,32,32,32};
			this->apply_surface(SCREEN_WIDTH-133,17,this->lights,this->screen,&q);
		} else {
			SDL_Rect w = {0,0,32,32};
			this->apply_surface(SCREEN_WIDTH-133,51,this->lights,this->screen,&w);
		}
	}	
}

game::game(void)
{
	message = NULL;
	textColor.r = 255; textColor.g = 255; textColor.b = 255;
	blackColor.r = 0; blackColor.g = 0; blackColor.b = 0;
	stream.str(""); stream.clear();

	edit_player_id = 0;
	edit_name = NULL;
	just_edited = false;
	last_edit_time = SDL_GetTicks();

	narty_po_upadku.position.w = 76; //szerokosc nart
	narty_po_upadku.position.h = 5; // wysokosc nart
	narty_po_upadku.FRICTION_FACTOR = 0.06;
	


	full_screen = true;
	//state = INTRO;//INTRO;//TITLE
	state=INTRO;

	FRICTION_FACTOR = 0.06;
	/* initialize random seed: */
	srand ( SDL_GetTicks() );
	//choose random wind velocity x and y in range from -2.0 to 2.0 m/s
	//((rand() % 401)-200.0)/100.0;
	this->wind_vx = ((rand() % 401)-200.0)/100.0;
	this->wind_vy = ((rand() % 401)-200.0)/100.0;

	double wind_v = sqrt( pow(wind_vx, 2.0) + pow(wind_vy, 2.0) );
	double wind_angle = asin(wind_vy/wind_v)*180/PI ;
	if(this->wind_vy < 0 && this->wind_vx < 0){wind_angle = - 180 - wind_angle;}
	else if(this->wind_vy >= 0 && this->wind_vx < 0){wind_angle = 180 - wind_angle;}

	this->wind_vx_change = 0.0;
	this->wind_vy_change = 0.0;

	cout << "Wind X: " << wind_vx << endl;
	cout << "Wind Y: " << wind_vy << endl;
	cout << "Wind Angle: " << wind_angle << endl;	

	
	this->screen = NULL;
	this->font_vgafixe_10 = NULL;
	this->font_verdana_24 = NULL;

	//Initialize all SDL subgames
	if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 ){exit(1);}    
    
    //Set up the screen8/
    this->screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE|SDL_FULLSCREEN/*|SDL_FULLSCREEN*/ );
	this->wind_arrow = load_image("grafika/hud/wind arrow.png");
	this->wind_bg = load_image("grafika/hud/wind bg.png");
	this->sygnalisator = load_image("grafika/hud/sygnalisator.png");
	this->lights = load_image("grafika/hud/lights.png");
	this->title_screen = load_image("grafika/hud/new title screen.png");
	this->login_screen = load_image("grafika/hud/login screen.png");
	this->help_screen = load_image("grafika/hud/sterowanie_pomoc.png");
	this->new_rekord_icon = load_image("grafika/hud/new.png");
	this->rekord_skoczni = load_image("grafika/hud/rekord.png");
	this->karetka = load_image("grafika/hud/karetka.png");

	this->show_title_screen = true;
	this->green_light = false;
    
    //If there was an error in setting up the screen
    if( this->screen == NULL ){exit(1);}
    
    //Initialize SDL_ttf 
    if( TTF_Init() == -1 ){exit(1);}    

	//Open the font
    this->font_vgafixe_10 = TTF_OpenFont( "grafika/czcionki/vgafixe.ttf", 10 );
	this->font_verdana_24 = TTF_OpenFont( "grafika/czcionki/verdana.ttf", 24 );
	this->font_courier_14 = TTF_OpenFont( "grafika/czcionki/courbd.ttf", 20 );
    //If there was an error in loading the font
    if( this->font_vgafixe_10 == NULL || this->font_verdana_24 == NULL || this->font_courier_14==NULL ){exit(1);}

    //Set the window caption
    SDL_WM_SetCaption( "The Best Ski Jumper"/*"UKSW Ski Jumping 2012"*/, NULL );
}

game::~game(void)
{
    //Close the font that was used
	TTF_CloseFont( font_vgafixe_10 );
	TTF_CloseFont( font_verdana_24 );
	TTF_CloseFont( font_courier_14 );
    
    //Quit SDL_ttf
    TTF_Quit();
    
    //Quit SDL
    SDL_Quit();
}

SDL_Surface* game::load_image( std::string filename ) 
{
    //Temporary storage for the image that's loaded
    SDL_Surface* loadedImage = NULL;
    
    //The optimized image that will be used
    SDL_Surface* optimizedImage = NULL;

	//Load the image
	loadedImage = IMG_Load( filename.c_str() );

	//If nothing went wrong in loading the image
    if( loadedImage != NULL )
    {
        //Create an optimized image
        optimizedImage = SDL_DisplayFormatAlpha( loadedImage );//SDL_DisplayFormat
        
        //Free the old image
        SDL_FreeSurface( loadedImage );
		
		//If the image was optimized just fine
        if( optimizedImage != NULL )
        {
            //Map the color key
            Uint32 colorkey = SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF );
		//Set all pixels of color R 0, G 0xFF, B 0xFF to be transparent
            //SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, colorkey );
        }
	}

	//Return the optimized image
    return optimizedImage;
}

void game::apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip)
{
    //Holds offsets
    SDL_Rect offset;
    
    //Get offsets
    offset.x = x;
    offset.y = y;
    
    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}

double game::collision_check(double x, double y) //detects collisions, returns 0.0 if collision occurs, else returns slope's y
{			
	list<slope>::iterator it;
	
	for(it = ogame.skocznia->slopes.begin(); it!=ogame.skocznia->slopes.end(); it++)
	{
		
		if( x >= it->position.x && x < it->position.x + it->position.w)
		{
			double slope_h = it->position.h;			//height of the slope (one of many slopes)
			double slope_w = it->position.w;			//weight of the slope (one of many slopes)

			double tg = slope_h/slope_w;
				
			double slope_x = x - it->position.x;				//x of the slope object that collises with jumper
			double slope_y = it->position.y + (slope_x * tg);	//y of the lading slope at jumper's x;

			
			if( y >= slope_y )
			{

				list<slope>::iterator it2;
				double suma = 0.0;
				for(it2 = ogame.skocznia->slopes.begin(); it2!=ogame.skocznia->slopes.end(); it2++){
					if( x >= it2->position.x ){
						cout << "x";
						if(x < it2->position.x + it2->position.w){
							
							suma+= sqrt(pow((C.x_precise - it2->position.x)/METER_TO_PIXEL,2.0)+pow((C.position.y - it2->position.y)/METER_TO_PIXEL,2.0));
						}else{
							suma+= sqrt(pow(it2->position.h/METER_TO_PIXEL,2.0)+pow(it2->position.w/METER_TO_PIXEL,2.0));
						}
					}
					
				}
				C.jump_length = suma;
				
				
				C.flies = false;
				C.landed = true;
				cout << "Slopes: " << slope_x << ", " << slope_y << endl;
				if( abs(this->skocznia->gradients[C.position.x]*180/PI - C.jumper_alpha*180/PI) < MIN_JUMPER_ANGLE){
					C.fell_down=2;
					cout << "Skok nieustany! Zawodnik sie przewrocil do przodu!  " <<  this->skocznia->gradients[C.position.x]*180/PI <<"  "<< C.jumper_alpha*180/PI << endl;
					
				}else if( abs(this->skocznia->gradients[C.position.x]*180/PI - C.jumper_alpha*180/PI) > MAX_JUMPER_ANGLE){
					C.fell_down=1;
					cout << "Skok nieustany! Zawodnik sie przewrocil do tylu!" << endl;

				}else if( abs(this->skocznia->gradients[C.position.x]*180/PI - C.skis_alpha*180/PI) < MIN_SKIS_ANGLE || abs(this->skocznia->gradients[C.position.x]*180/PI - C.skis_alpha*180/PI) > MAX_SKIS_ANGLE){
					if( abs(this->skocznia->gradients[C.position.x]*180/PI - C.jumper_alpha*180/PI) < 90) C.fell_down=2;
					else C.fell_down=1;
					cout << "Skok nieustany! Zawodnik sie przewrocil!" << endl;
				}
				if(C.fell_down){
					ogame.FRICTION_FACTOR = 0.8;
					ogame.zawodnik->ogledlosci_skokow[ogame.seria_skokow-1] = 0.0;

					ogame.narty_po_upadku.position.x = C.position.x;
					ogame.narty_po_upadku.position.y = C.position.y;
					ogame.narty_po_upadku.x_precise = C.x_precise;
					ogame.narty_po_upadku.y_precise = C.y_precise;
					ogame.narty_po_upadku.v = C.v;
					

				}else{
					ogame.FRICTION_FACTOR = 0.06;
					ogame.zawodnik->ogledlosci_skokow[ogame.seria_skokow-1] = C.jump_length;
				}
				double odleglosc_skoku = (int)(ogame.zawodnik->ogledlosci_skokow[ogame.seria_skokow-1]*100)/100.0;
				if(!ogame.zawodnik->is_computer && odleglosc_skoku > ogame.skocznia->rekord){
					ogame.rekord_skoczni_type = 1;
					ogame.skocznia->rekord = odleglosc_skoku;
					ogame.skocznia->rekordzista = ogame.zawodnik->name;
				}

				ogame.zawodnik->wynik += odleglosc_skoku ;				

				return slope_y;
			}

		}
	}
	return 0.0;	
}

void game::ai_handle()
{
	int level_zawodnika = 0;
	int i = 0;

	for (list<gracz>::iterator it=lista_zawodnikow.begin(); it != lista_zawodnikow.end(); ++it){
		i++;
		if(i == ogame.id_zawodnika) level_zawodnika = (*it).level;
	}
	//uzywaj zmiennej level_zawodnika, dla kontrolowania przebiegu lotu dla poszczegolnych
	//poziomow komputerow, 1 to komputer latwy, 2 to sredni, 3 to trudny

	//losowosc w momencie wybicia
	int rndm = rand() %10;

	//zroznicowane momenty wybicia, osobno dla kazdej skoczni
	if(this->green_light)
	{
		if(C.on_bar)
		{
			C.on_bar = false;
			C.time = SDL_GetTicks(); 
		} 
		else if(ogame.skocznia->name=="Gawronowo") 
		{
			if(!C.landed && C.position.x >= (ogame.skocznia->rozbieg_width -90 -12 +level_zawodnika*30 +rndm) ) { C.jump(); }
		}
		else if(ogame.skocznia->name=="Jurowo") 
		{
			if(!C.landed && C.position.x >= (ogame.skocznia->rozbieg_width -150 -12 +level_zawodnika*50 +rndm) ) { C.jump(); }
		}
	}

	//gdzie ma zacz¹æ przyjmowac pozycje do ladowania
	int start_landing_from=0;
	if(ogame.skocznia->name=="Gawronowo") 
	{
		switch (level_zawodnika) 
		{
			case 1: start_landing_from = 250; break;
			case 2: start_landing_from = 650; break;
			case 3: start_landing_from = 1000;break;
		}
	}
	else if(ogame.skocznia->name=="Jurowo")
	{
		switch (level_zawodnika) 
		{
			case 1: start_landing_from = 970; break;
			case 2: start_landing_from = 1300;break;
			case 3: start_landing_from = 1700;break;
		}
	}

	//sterowanie kompa w locie
	if(C.flies && (C.x_precise < ogame.skocznia->rozbieg_width + start_landing_from) )
	{
		//zawodnik 1 levelu leci stale tym samym pocz¹tkowym k¹tem
		//zawodnik 2 levelu zmienia k¹t po wybiciu i dalej leci tak samo
		//zawodnik 3 levelu na bie¿¹co dostosowuje k¹t lotu
		if (level_zawodnika == 2)
		{
			C.jumper_alpha = -PI/9;
			C.skis_alpha = 0;
		}
		else if (level_zawodnika == 3)
		{
			//C.jumper_alpha = ogame.skocznia->gradients[C.position.x] - PI/5;
			if ( abs(C.jumper_alpha - (ogame.skocznia->gradients[C.position.x]-PI/5)) > PI/1000 )
			{
				if( abs(C.jumper_alpha-C.skis_alpha) > MIN_ANGLE_VARIANCE)
					C.jumper_alpha += PI/1000;
			}
			//C.skis_alpha = ogame.skocznia->gradients[C.position.x] - PI/7.5;
			if ( C.skis_alpha - (ogame.skocznia->gradients[C.position.x]-PI/7) > PI/1000 )
			{
				C.skis_alpha -= PI/1000;
			}
			else if ( C.skis_alpha - (ogame.skocznia->gradients[C.position.x]-PI/7) < -PI/1000 )
			{
				C.skis_alpha += PI/1000;
			}
		}
	}

	//powolne korygowanie k¹ta skoczka i nart do pozycji idealnej dla l¹dowania
	if( C.x_precise >= ogame.skocznia->rozbieg_width + start_landing_from )
	{
		//cia³o
		if ( 90 - (abs(ogame.skocznia->gradients[C.position.x] - C.jumper_alpha) *180/PI) < 0 )
			C.jumper_alpha += 0.3*PI/180;
		else
			C.jumper_alpha -= 0.3*PI/180;
		//narty
		if ( 20 - (abs(ogame.skocznia->gradients[C.position.x] - C.skis_alpha) *180/PI) < 0 )
			C.skis_alpha += 0.3*PI/180;
		else
			C.skis_alpha -= 0.3*PI/180;
	}

}

void game::keyboard_handle(){
	Uint8 *keystates = SDL_GetKeyState( NULL );
	/*
	if(ogame.state==SETTINGS){
		if( ogame.event.type == SDL_KEYDOWN ){
			if(ogame.event.key.keysym.sym ==  SDLK_BACKSPACE){
				cout << "BACKSPACE!" << endl;
			}
		}
	} else {
	*/
	if( keystates[ SDLK_SPACE ] )
	{
		//cout << "SPACE" << endl;
		if(this->green_light)
		{
			if(C.on_bar){
				C.on_bar = false;
				C.time = SDL_GetTicks();
			} else if(!C.landed && C.position.x >= ogame.skocznia->can_jump_from) {
				C.jump();
			}
		}
	}
	
	//If up is pressed
	if( keystates[ SDLK_UP ] )
	{
		
		if( C.jumped && abs(C.skis_alpha-C.jumper_alpha) < MAX_ANGLE_VARIANCE)
		{
			C.jumper_alpha-=0.3*PI/180;
			if(abs(C.jumper_alpha)>=PI)
			{
				C.jumper_alpha=-C.jumper_alpha;
			}
		}

	}

	
	//If down is pressed
	if( keystates[ SDLK_DOWN ] )
	{
		if( C.jumped && abs(C.skis_alpha-C.jumper_alpha) > MIN_ANGLE_VARIANCE)
		{
			C.jumper_alpha+=0.3*PI/180;
			if(abs(C.jumper_alpha)>=PI)
			{
				C.jumper_alpha=-C.jumper_alpha;
			}
		}
	}

	//If left is pressed
	if( keystates[ SDLK_LEFT ] )
	{
		if( C.jumped && abs(C.skis_alpha-C.jumper_alpha) > MIN_ANGLE_VARIANCE)
		{
			C.skis_alpha-=0.3*PI/180;
			if(abs(C.skis_alpha)>=PI)
			{
				C.skis_alpha=-C.skis_alpha;
			}
		}
	}

	//If right is pressed
	if( keystates[ SDLK_RIGHT ] )
	{
		if( C.jumped && abs(C.skis_alpha-C.jumper_alpha) < MAX_ANGLE_VARIANCE)
		{
			C.skis_alpha+=0.3*PI/180;
			if(abs(C.skis_alpha)>=PI)
			{
				C.skis_alpha=-C.skis_alpha;
			}
		}
	}


	//}
}



void game::cycle()
{
    //The timer start/stop flag, this will be needed for pausing the game
    bool running = true;

	this->lista_skoczni.insert(pair<string,hill*>("Jurowo",new hill("Jurowo"
		) ) );
	this->lista_skoczni.insert(pair<string,hill*>("Gawronowo",new hill("Gawronowo") ) );
	
	r_save.open(savefile);
	if(r_save.good() && !is_empty(r_save)){ 
		r_save.close();
		odczyt_danych();
		
	}else{
		gracz G1 = {"Czlowiek",false,0}; lista_zawodnikow.push_back(G1);
		/*
		gracz G2 = {"Czlowiek",false,0}; lista_zawodnikow.push_back(G2);
		gracz G3 = {"Komputer",true,3}; lista_zawodnikow.push_back(G3);
		gracz G4 = {"Komputer",true,1}; lista_zawodnikow.push_back(G4);
		gracz G5 = {"Komputer",true,1}; lista_zawodnikow.push_back(G5);
		gracz G6 = {"Komputer",true,2}; lista_zawodnikow.push_back(G6);
		gracz G7 = {"Komputer",true,1}; lista_zawodnikow.push_back(G7);
		gracz G8 = {"Komputer",true,3}; lista_zawodnikow.push_back(G8);
		gracz G9 = {"Komputer",true,3}; lista_zawodnikow.push_back(G9);
		*/
		gracz G10 = {"Komputer",true,2}; lista_zawodnikow.push_back(G10);
		

		
	}

	adding_player = false;
	chose_computer = false;
	skok_zakonczony = false;
	id_zawodnika = 1;
	rekord_skoczni_type = 0;
	int i =0;
	for (list<gracz>::iterator it=lista_zawodnikow.begin(); it != lista_zawodnikow.end(); ++it){
		i++;
		if(i == 1) zawodnik = &(*it);
		(*it).numer_startowy=i;
	}


	ramka = SDL_CreateRGBSurface( SDL_SRCALPHA|SDL_SWSURFACE|SDL_SRCCOLORKEY, 250, 20, 32, 0, 0, 0, 0 );
	ramka = SDL_DisplayFormat( ramka );//SDL_DisplayFormat
    SDL_SetAlpha(ramka, SDL_SRCALPHA|SDL_RLEACCEL , 150);

	ramka2 = SDL_CreateRGBSurface( SDL_SRCALPHA|SDL_SWSURFACE|SDL_SRCCOLORKEY, 300, 23, 32, 0, 0, 0, 0 );
	ramka2 = SDL_DisplayFormat( ramka2 );//SDL_DisplayFormat
    SDL_SetAlpha(ramka2, SDL_SRCALPHA|SDL_RLEACCEL , 150);

	//640/2 - 147/2 = 320 - 76 = 244px from left
	ramka_przed_zawodami = SDL_CreateRGBSurface( SDL_SRCALPHA|SDL_SWSURFACE|SDL_SRCCOLORKEY, 153, 23, 32, 0, 0, 0, 0 );
	ramka_przed_zawodami = SDL_DisplayFormat( ramka_przed_zawodami );
    SDL_SetAlpha(ramka_przed_zawodami, SDL_SRCALPHA|SDL_RLEACCEL , 150);

	//640/2 - 242/2 = 320 - 124 = 179px from left
	ramka_po_1_serii = SDL_CreateRGBSurface( SDL_SRCALPHA|SDL_SWSURFACE|SDL_SRCCOLORKEY, 260, 23, 32, 0, 0, 0, 0 );
	ramka_po_1_serii = SDL_DisplayFormat( ramka_po_1_serii );
    SDL_SetAlpha(ramka_po_1_serii, SDL_SRCALPHA|SDL_RLEACCEL , 150);

	//640/2 - 453/2 = 320 - 226 = 94px from left
	ramka_po_2_serii = SDL_CreateRGBSurface( SDL_SRCALPHA|SDL_SWSURFACE|SDL_SRCCOLORKEY, 453, 23, 32, 0, 0, 0, 0 );
	ramka_po_2_serii = SDL_DisplayFormat( ramka_po_2_serii );
    SDL_SetAlpha(ramka_po_2_serii, SDL_SRCALPHA|SDL_RLEACCEL , 150);
	
	list<button> menu;
	menu.push_back( button(250, 100, 131, 22, "grafika/przyciski/zawody.png") );
	menu.push_back( button(250, 140, 131, 22, "grafika/przyciski/ustawienia.png") );
	menu.push_back( button(250, 180, 131, 22, "grafika/przyciski/pomoc.png") );
	menu.push_back( button(250, 220, 131, 22, "grafika/przyciski/wyjscie.png") );


	list<button> ustawienia;
	ustawienia.push_back( button(338, 46, 120, 30, "grafika/przyciski/wlaczony.png") );//wylaczony.png
	ustawienia.push_back( button(219, 188, 202, 34, "grafika/przyciski/dodaj zawodnika.png") );
	ustawienia.push_back( button(279, 208, 82, 34, "grafika/przyciski/dodaj.png") );

	ustawienia.push_back( button(180, 176, 130, 22, "grafika/przyciski/czlowiek.png") );
	ustawienia.push_back( button(330, 176, 130, 22, "grafika/przyciski/komputer.png") );

	ustawienia.push_back( button(462, 176, 84, 16, "grafika/przyciski/latwy.png") );
	ustawienia.push_back( button(462, 176+18, 84, 16, "grafika/przyciski/sredni.png") );
	ustawienia.push_back( button(462, 176+36, 84, 16, "grafika/przyciski/trudny.png") );

	for(int i = 1; i<=10; i++) ustawienia.push_back( button(460, 124+25*i, 23, 23, "grafika/przyciski/delete.png") );
	for(int i = 1; i<=10; i++) ustawienia.push_back( button(206, 124+25*i, 98, 23, "edit") );

	//rozmiar niewidzialnego buttona: 98x23
	//punkt zaczepienia niewidzialnego buttona: <206,149>


	list<button> menu_hills;
	menu_hills.push_back( button(225, 91, 191, 121, "grafika/skocznie/gawronowo.png") );
	menu_hills.push_back( button(225, 269, 191, 121, "grafika/skocznie/jurowo.png") );

    if( SDL_Flip( ogame.screen ) == -1 ){exit(1);}

	start = SDL_GetTicks();


	//While the user hasn't quit
	while( C.quit == false )
	{
		//While there's an event to handle
        while( SDL_PollEvent( &ogame.event ) )
		{
			if(state==MENU){
				for (list<button>::iterator it=menu.begin(); it != menu.end(); ++it)
					(*it).handle_events();

			}else if(state==SETTINGS){
				int max = lista_zawodnikow.size();
				int i = 0;
				int j = 0;
				for (list<button>::iterator it=ustawienia.begin(); it != ustawienia.end(); ++it){
					if(!(*it).name.compare("grafika/przyciski/delete.png")){
						if(i < max && max>1){
							i++;
							(*it).handle_events();
						}
					}
					else if(!(*it).name.compare("grafika/przyciski/dodaj zawodnika.png")){
						if(!ogame.adding_player && max < 10) {if((*it).handle_events()) break;}
					
					}else if(!(*it).name.compare("grafika/przyciski/czlowiek.png")||!(*it).name.compare("grafika/przyciski/komputer.png")||!(*it).name.compare("grafika/przyciski/dodaj.png")){
						if(ogame.adding_player) (*it).handle_events();
					
					}else if( !(*it).name.compare("grafika/przyciski/latwy.png")|| !(*it).name.compare("grafika/przyciski/sredni.png")|| !(*it).name.compare("grafika/przyciski/trudny.png") ){
						if(ogame.adding_player && ogame.chose_computer) (*it).handle_events();

					}else if(!(*it).name.compare("edit")){
						if(j < max ){
							j++;
							(*it).handle_events();
						}

					}else (*it).handle_events();
					
					
				}
			}else if(state==HILL_SELECT){
				for (list<button>::iterator it=menu_hills.begin(); it != menu_hills.end(); ++it)
					(*it).handle_events();
			}
			
            //If a key was pressed
            if( ogame.event.type == SDL_KEYDOWN )
			{
				if( ogame.event.key.keysym.sym == SDLK_ESCAPE)
				{
					if(ogame.state==GAME)
					{	
						ogame.zapis_danych();
						ogame.state=HILL_SELECT;
						if(ogame.full_screen) SDL_ShowCursor(1);
						
						//SDL_FreeSurface( ogame.skocznia->skoczn->icon);
						//SDL_FreeSurface( ogame.skocznia->blue->icon);
						//SDL_FreeSurface( ogame.skocznia->blue2->icon);

					}else if(ogame.state==HILL_SELECT || ogame.state ==HELP) {
						ogame.state=MENU;

					}else if(ogame.state==SETTINGS){
						ogame.state=MENU;
						SDL_EnableUNICODE( SDL_DISABLE );
						ogame.edit_name = NULL;
						ogame.edit_player_id = 0;
						int i = 0;
						for (list<gracz>::iterator it=lista_zawodnikow.begin(); it != lista_zawodnikow.end(); ++it){
							i++;
							(*it).numer_startowy=i;
							if( (*it).name.length() < 3){
								if( (*it).is_computer ) (*it).name = "Komputer";
								else (*it).name = "Czlowiek";
							}
						}
						ogame.zapis_danych();

					} else C.quit = true;
				}
				if(	ogame.state==SETTINGS && ogame.edit_player_id){
					
					if( ogame.event.key.keysym.sym ==SDLK_BACKSPACE && (ogame.edit_name->length() != 0) ){
						//cout << (char)event.key.keysym.unicode;
						ogame.edit_name->erase( ogame.edit_name->length() - 1 );
						ogame.just_edited = true;
					}
					
					if(ogame.edit_name->length() < 8){
						if( ( event.key.keysym.unicode >= (Uint16)'A' ) && ( event.key.keysym.unicode <= (Uint16)'Z' ) )
						{
							//cout << (char)event.key.keysym.unicode;
							(*ogame.edit_name) += (char)event.key.keysym.unicode;
							ogame.just_edited = true;
						}
						else if( ( event.key.keysym.unicode >= (Uint16)'a' ) && ( event.key.keysym.unicode <= (Uint16)'z' ) )
						{
							//cout << (char)event.key.keysym.unicode;
							(*ogame.edit_name) += (char)event.key.keysym.unicode;
							ogame.just_edited = true;
						}
					}

					if(ogame.event.key.keysym.sym ==  SDLK_RETURN ){
						ogame.edit_name = NULL;
						ogame.edit_player_id = 0;
					}
					
				}
				if(ogame.state == GAME || ogame.state == TITLE){
					if ( ogame.event.key.keysym.sym ==  SDLK_RETURN )
					{
						cout << "ENTER!" << endl;
						if(ogame.state == GAME){
							if(!zawody_info){
								if(ogame.zawodnik->is_computer){
									if(!ogame.skok_zakonczony){
										if(!C.landed) {
											double odleglosc_skoku = 0.0;
											if(ogame.skocznia->name=="Gawronowo"){
												if(ogame.zawodnik->level == 1)
													odleglosc_skoku = (rand() % 1000 + 4000)/100.0;
												else if(ogame.zawodnik->level == 2)
													odleglosc_skoku = (rand() % 1000 + 6500)/100.0;
												else if(ogame.zawodnik->level == 3)
													odleglosc_skoku = (rand() % 1000 + 8500)/100.0;
											}else if(ogame.skocznia->name=="Jurowo"){
												if(ogame.zawodnik->level == 1)
													odleglosc_skoku = (rand() % 1000 + 7000)/100.0;
												else if(ogame.zawodnik->level == 2)
													odleglosc_skoku = (rand() % 1000 + 9000)/100.0;
												else if(ogame.zawodnik->level == 3)
													odleglosc_skoku = (rand() % 1000 + 14000)/100.0;
											}
											ogame.zawodnik->ogledlosci_skokow[ogame.seria_skokow-1] = odleglosc_skoku;
											ogame.zawodnik->wynik += ogame.zawodnik->ogledlosci_skokow[ogame.seria_skokow-1];
										}
										skok_zakonczony = true;
									}
								}

								if(ogame.skok_zakonczony){
									if(ogame.rekord_skoczni_type==1) ogame.rekord_skoczni_type=2;
									if(id_zawodnika==lista_zawodnikow.size()) zawody_info = true;
									else {
										skok_zakonczony = false;
										int i = 0;
										if(id_zawodnika==lista_zawodnikow.size()) id_zawodnika = 0;
										for (list<gracz>::iterator it=lista_zawodnikow.begin(); it != lista_zawodnikow.end(); ++it){
											if(id_zawodnika==i) {zawodnik = &(*it); id_zawodnika++;  break; }
											i++;
										}
										C.reset_variables();
									}
									
								}
							} else {
								if(seria_skokow==2) {
									ogame.zapis_danych();
									ogame.state=HILL_SELECT;
									if(ogame.full_screen) SDL_ShowCursor(1);
								}else{
									if(seria_skokow==1) ogame.lista_zawodnikow.reverse();
									zawody_info = false;
									skok_zakonczony = false;
									int i = 0;
									if(id_zawodnika==lista_zawodnikow.size()) id_zawodnika = 0;
									for (list<gracz>::iterator it=lista_zawodnikow.begin(); it != lista_zawodnikow.end(); ++it){
										if(id_zawodnika==i) {zawodnik = &(*it); id_zawodnika++;  break; }
										i++;
									}
									C.reset_variables();
									seria_skokow++;
								}
							}

						}else if(ogame.state == TITLE)
						{
							ogame.state = TO_MENU;
						
						}
					}
				}

			}
            //If the user has Xed out the window
            else if( ogame.event.type == SDL_QUIT ) 
            {
                //Quit the program
                C.quit = true;
            }
		}

		//renderowanie menu
		if(ogame.state==INTRO){
			static int i;
			static int phase = 0;
			if(phase==0){if(SDL_GetTicks() - start > 100) {phase = 1; i=0;}}
			else if(phase==1){if(SDL_GetTicks() - start > 3000) {phase = 2; i=0;}}
			else if(phase==2){if(SDL_GetTicks() - start > 250) {phase = 3; i=0;}}
			if(phase){
				if(i < 240)
				{
					if(SDL_GetTicks() - start > 10){
						start = SDL_GetTicks();
						i++;
						if(phase==1){
							ogame.apply_surface(0,0,this->title_screen,this->screen);

							SDL_Rect rect1 = {0,0,640,240-i};
							SDL_FillRect(this->screen, &rect1, SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 ));
							
							SDL_Rect rect2 = {0,240+i,640,240-i};
							SDL_FillRect(this->screen, &rect2, SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 ));
						}else if(phase==2){
							ogame.apply_surface(0,0,this->title_screen,this->screen);

							SDL_Rect rect1 = {0,0,640,i};
							SDL_FillRect(this->screen, &rect1, SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 ));
							
							SDL_Rect rect2 = {0,480-i,640,i};
							SDL_FillRect(this->screen, &rect2, SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 ));

						}else if(phase==3){
							ogame.apply_surface(0,0,this->login_screen,this->screen);

							SDL_Rect rect1 = {0,0,640,240-i};
							SDL_FillRect(this->screen, &rect1, SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 ));
							
							SDL_Rect rect2 = {0,240+i,640,240-i};
							SDL_FillRect(this->screen, &rect2, SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 ));
						}

						if( SDL_Flip( ogame.screen ) == -1 ){exit(1);}
					}
				}else if(phase==3) ogame.state=MENU;//TITLE
			}

		} else if(ogame.state==TITLE){
			if(SDL_GetTicks() - start > 1000){
				start = SDL_GetTicks();

				static bool shines = false;
				if(shines) textColor.b = 255;
				else textColor.b = 0;
				shines = (shines ? false : true);

				stream << "aby kontynuwaÄ‡, wciÅ›nij ENTER";
				ogame.render_Text(135,215,message, ogame.font_verdana_24,stream,blackColor,textColor,2,true); //na odwrot kolory

				textColor.b = 255;
				if( SDL_Flip( ogame.screen ) == -1 ){exit(1);}
			}
		} else if(ogame.state==TO_MENU){

			ogame.state = MENU;

		} else if(ogame.state==MENU){
			ogame.apply_surface(0,0,this->login_screen,this->screen);

			stream << "The Best Ski Jumper";
			ogame.render_Text(205,35,message, ogame.font_verdana_24,stream,textColor,blackColor,2,true);

			for (list<button>::iterator it=menu.begin(); it != menu.end(); ++it)
				(*it).render();
			
			if( SDL_Flip( ogame.screen ) == -1 ){exit(1);}

		} else if(ogame.state==SETTINGS){
			ogame.apply_surface(0,0,this->login_screen,this->screen);	

			ogame.apply_surface(158,124,ramka2,this->screen);
			stream << "id  nazwa        poziom";
			ogame.render_Text(159,125,message, ogame.font_courier_14,stream,textColor,blackColor,2,true);

			int i = 0;
			for (list<gracz>::iterator it=lista_zawodnikow.begin(); it != lista_zawodnikow.end(); ++it){
				i++;
				
	

				ogame.apply_surface(158,124+i*25,ramka2,this->screen);

				stream << i << (i==10?". ":".  ");// << (*it).name;
				
				stringstream tmp;
				tmp << (*it).name;
				while(tmp.str().length()<8) tmp << " ";
				stream << tmp.str();
				tmp.str(""); tmp.clear();



				if( (*it).level==1 ) stream << "     latwy";
				else if( (*it).level==2 ) stream << "     sredni";
				else if( (*it).level==3 ) stream << "     trudny";
				else stream << "       -";
				ogame.render_Text(159,125+i*25,message, ogame.font_courier_14,stream,textColor,blackColor,2,true);
			}

			
			stream << "PeÅ‚ny ekran:";
			ogame.render_Text(183,45,message, ogame.font_verdana_24,stream,textColor,blackColor,2,true);

			stream << "Lista zawodnikÃ³w";
			ogame.render_Text(216,85,message, ogame.font_verdana_24,stream,textColor,blackColor,2,true);

		
			int max = lista_zawodnikow.size();
			i = 0;
			for (list<button>::iterator it=ustawienia.begin(); it != ustawienia.end(); ++it){
				if(!(*it).name.compare("grafika/przyciski/delete.png")){
					if(i < max && max>1){
						i++;
						(*it).render();
					}
				}
				else if(!(*it).name.compare("grafika/przyciski/dodaj zawodnika.png")){
					(*it).box.y = 188+23*max;
					if(!ogame.adding_player && max < 10) (*it).render();
				
				}else if(!(*it).name.compare("grafika/przyciski/dodaj.png")){
					(*it).box.y = 208+23*max;
					if(ogame.adding_player && max < 10) (*it).render();

				}else if(!(*it).name.compare("grafika/przyciski/czlowiek.png")){
					(*it).box.y = 176+23*max;
					if(ogame.adding_player) (*it).render();
				}else if(!(*it).name.compare("grafika/przyciski/komputer.png")){
					(*it).box.y = 176+23*max;
					if(ogame.adding_player) (*it).render();

				}else if(!(*it).name.compare("grafika/przyciski/latwy.png")){
					(*it).box.y = 176+23*max;
					if(ogame.adding_player && ogame.chose_computer) (*it).render();
				}else if(!(*it).name.compare("grafika/przyciski/sredni.png")){
					(*it).box.y = 176+17+23*max;
					if(ogame.adding_player && ogame.chose_computer) (*it).render();
				}else if(!(*it).name.compare("grafika/przyciski/trudny.png")){
					(*it).box.y = 176+34+23*max;
					if(ogame.adding_player && ogame.chose_computer) (*it).render();

				}else if((*it).name.length()>=10) (*it).render();//nazwa pliku ze sciezka, napewno bedize miec ponad 10 znakow
			}
			stream << "kilknij na nazwê zawodnika, aby j¹ zmieniæ!";
			ogame.render_Text(5,445,message, ogame.font_vgafixe_10,stream,textColor,blackColor,2);

			stream << "wciœnij escape, aby wróciæ do menu g³ównego";
			ogame.render_Text(5,463,message, ogame.font_vgafixe_10,stream,textColor,blackColor,2);

			if(ogame.edit_player_id){
				static int klatka = 0;
				klatka++;
				if(ogame.just_edited) {klatka = 0; ogame.just_edited = false;}
				if(klatka<=250) ogame.apply_surface(208+12*ogame.edit_name->length(),125+25*ogame.edit_player_id,this->karetka,this->screen);
				klatka%=500;
			}

			/*
			if(ogame.edit_player_id){
				if(SDL_GetTicks() - ogame.last_edit_time > 100){
					ogame.last_edit_time = SDL_GetTicks();
					this->keyboard_handle();
				}
			}
			*/

			if( SDL_Flip( ogame.screen ) == -1 ){exit(1);}
		}else if(ogame.state==HELP){
			ogame.apply_surface(0,0,this->help_screen,this->screen);

			stream << "wciœnij escape, aby wróciæ do menu g³ównego";
			ogame.render_Text(5,463,message, ogame.font_vgafixe_10,stream,textColor,blackColor,2);

			if( SDL_Flip( ogame.screen ) == -1 ){exit(1);}

		}else if(ogame.state==HILL_SELECT){
			ogame.apply_surface(0,0,this->login_screen,this->screen);

			stream << "Wybierz skoczniÄ™";
			ogame.render_Text(215,35,message, ogame.font_verdana_24,stream,textColor,blackColor,2,true);

			stream << "wciœnij escape, aby wróciæ do menu g³ównego";
			ogame.render_Text(5,463,message, ogame.font_vgafixe_10,stream,textColor,blackColor,2);
			
			for (list<button>::iterator it=menu_hills.begin(); it != menu_hills.end(); ++it)
				(*it).render();
			
			if( SDL_Flip( ogame.screen ) == -1 ){exit(1);}

		} else if(ogame.state==GAME){
		ogame.skocznia->render();
		
		int tmp_x = 5;
		int tmp_y = 463;

		if(ogame.skok_zakonczony || ogame.zawody_info){
			tmp_y = 445;

			if(ogame.id_zawodnika != ogame.lista_zawodnikow.size() && !ogame.zawody_info) stream << "wciœnij enter, aby przejœæ dalej";
			else {
				if(ogame.seria_skokow==0) stream << "wciœnij enter, aby rozpocz¹æ zawody";
				else if(ogame.seria_skokow==1) stream << "wciœnij enter, aby przejœæ do kolejnej serii";
				else if(ogame.seria_skokow==2) stream << "wciœnij enter, aby zakoñczyæ zawody";
			}

			ogame.render_Text(5,463,message, ogame.font_vgafixe_10,stream,textColor,blackColor,2);


			/*
			if(!ogame.zawody_info && ogame.skok_zakonczony){
				if(ogame.rekord_skoczni_type==1) {
					static int klatka = 0;
					klatka++;
					if(klatka<=125) ogame.apply_surface(109,95 - 70,ogame.rekord_skoczni,this->screen);
					klatka%=250;
					cout <<"{XXX}";
				}
			}
			*/

			if(zawody_info){		
				int szerokosc_napisu = 0;
				if(ogame.skocznia->name=="Gawronowo") szerokosc_napisu = 256;
				else if(ogame.skocznia->name=="Jurowo") szerokosc_napisu = 199;
				int x_location = 320 - szerokosc_napisu/2;//chcemy zeby napis byl zawsze na srodku ekranu

				stream << "Skocznia " << ogame.skocznia->name;
				ogame.render_Text(x_location,35,message, ogame.font_verdana_24,stream,textColor,blackColor,2,true);

				stream << "K" << ogame.skocznia->punkt_k << "";
				ogame.render_Text(x_location+szerokosc_napisu,35,message, ogame.font_vgafixe_10,stream,textColor,blackColor,2);

				
				x_location = 5;
				if(ogame.rekord_skoczni_type == 2) { 
					ogame.apply_surface(5,449,ogame.new_rekord_icon,this->screen);
					x_location = 33;
				}

				stream << "rekord skoczni: ";
				if(ogame.skocznia->rekord>0) stream << ogame.skocznia->rekordzista << " - " << ogame.skocznia->rekord << "m";
				else stream << "brak";
				ogame.render_Text(x_location,445,message, ogame.font_vgafixe_10,stream,textColor,blackColor,2);

				


				szerokosc_napisu = 209;
				x_location = 320 - szerokosc_napisu/2;
				stream << "Lista zawodnikÃ³w";
				ogame.render_Text(x_location,85,message, ogame.font_verdana_24,stream,textColor,blackColor,2,true);

				x_location = 0;
				if(ogame.seria_skokow==0) x_location=238;
				else if(ogame.seria_skokow==1) x_location=196;
				else if(ogame.seria_skokow==2) x_location=94;
				
				ogame.lista_zawodnikow.sort(porownaj_wyniki);
				
				if(ogame.seria_skokow==0) ogame.apply_surface(x_location-1,124,ramka_przed_zawodami,this->screen);
				else if(ogame.seria_skokow==1) ogame.apply_surface(x_location-1,124,ramka_po_1_serii,this->screen);
				else if(ogame.seria_skokow==2) ogame.apply_surface(x_location-1,124,ramka_po_2_serii,this->screen);

				stream << "    zawodnik";
				if(ogame.seria_skokow==1) stream<<"   skok1";
				else if(ogame.seria_skokow==2) stream<<"   skok1   skok2   wynik";
				ogame.render_Text(x_location,125,message, ogame.font_courier_14,stream,textColor,blackColor,2,true);

				int i = 0;
				for (list<gracz>::iterator it=lista_zawodnikow.begin(); it != lista_zawodnikow.end(); ++it){
					i++;
					
					if(ogame.seria_skokow==0) ogame.apply_surface(x_location-1,124+i*25,ramka_przed_zawodami,this->screen);
					else if(ogame.seria_skokow==1) ogame.apply_surface(x_location-1,124+i*25,ramka_po_1_serii,this->screen);
					else if(ogame.seria_skokow==2) ogame.apply_surface(x_location-1,124+i*25,ramka_po_2_serii,this->screen);
					
					stream << i << (i==10?". ":".  "); /* << (*it).name;*/
					
					stringstream tmp;
					tmp << (*it).name;
					while(tmp.str().length()<8) tmp << " ";
					stream << tmp.str();
					tmp.str(""); tmp.clear();


					if(ogame.seria_skokow==1) stream << "   " << (int)((*it).ogledlosci_skokow[0]*100)/100.0;
					else if(ogame.seria_skokow==2){
						//stream << "   " << (int)((*it).ogledlosci_skokow[0]*100)/100.0;
						//stream << "   " << (int)((*it).ogledlosci_skokow[1]*100)/100.0;
						//stream << "   " << (*it).wynik;

						stream << "   ";
						
						stringstream tmp;
						tmp << (int)((*it).ogledlosci_skokow[0]*100)/100.0;
						while(tmp.str().length()<8) tmp << " ";
						stream << tmp.str(); //wyswietlenie odleglosci pierwszego skoku
						tmp.str(""); tmp.clear();

						tmp << (int)((*it).ogledlosci_skokow[1]*100)/100.0;
						while(tmp.str().length()<8) tmp << " ";
						stream << tmp.str(); //wyswietlenie odleglosci drugiego skoku
						tmp.str(""); tmp.clear(); 

						stream << (*it).wynik;
					}
					ogame.render_Text(x_location,125+i*25,message, ogame.font_courier_14,stream,textColor,blackColor,2,true);
				}
			}

			


		}else if(zawodnik->is_computer && ogame.green_light){
			tmp_y = 445;

			stream << "wciœnij enter, aby pomin¹æ ogl¹danie";
			ogame.render_Text(5,463,message, ogame.font_vgafixe_10,stream,textColor,blackColor,2);
		}

		if( ( !ogame.skok_zakonczony || ogame.id_zawodnika != ogame.lista_zawodnikow.size() )&& !ogame.zawody_info){
			//stream << "obecnie skacze #"<< id_zawodnika<<" "<<zawodnik->name;
			stream << "zawodnik: "<<zawodnik->name << " (" << (zawodnik->is_computer?"komputer":"cz³owiek") << ")";
			ogame.render_Text(tmp_x,tmp_y,message, ogame.font_vgafixe_10,stream,textColor,blackColor,2);
		}



		
		int time = SDL_GetTicks() - start;
		if(!this->green_light && SDL_GetTicks() - red_light_start > 2000){this->green_light=true;} 
		if(time > 1){
			start = SDL_GetTicks(); // eventualnie
			
			if(state==GAME){
				if(!zawody_info){
					if(!zawodnik->is_computer) this->keyboard_handle();
					else this->ai_handle();
				}
			}

			if( ( !ogame.skok_zakonczony || ogame.id_zawodnika != ogame.lista_zawodnikow.size() ) && !ogame.zawody_info){
				

				C.move();

				C.cam.update(C.position);
				
				C.render();

				ogame.render_HUD();

				
				if(ogame.rekord_skoczni_type==1) {
					static int klatka = 0;
					klatka++;
					if(klatka<=250) ogame.apply_surface(140,50,ogame.rekord_skoczni,this->screen);
					klatka%=500;
					//cout << "{YYY}";
				}
				


			} else if(ogame.zawody_info) {
				SDL_Rect rect = {0,0,0,0};
				C.cam.update(rect);
			}
		
			
			//Update the screen
			if( SDL_Flip( ogame.screen ) == -1 ){exit(1);}}
		}
	}   

}