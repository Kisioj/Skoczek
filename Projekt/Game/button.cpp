#include "button.h"
#include "hill.h"
#include "jumper.h"
//#include "game.h"

extern game ogame;
extern jumper C;
//extern hill CH;



button::button(int x, int y, int w, int h, string s/*, bool visibility*/)
{
	//visible = visibility;

	up.x = 0;    up.y = 0;    up.w = w;    up.h = h;
	down.x = 0;    down.y = h;    down.w = w;    down.h = h;

	box.x = x;	box.y = y;	box.w = w;	box.h = h;

	clip = &up;

	if(s.length() >= 10) icon = ogame.load_image(s); //wszystkie pliki sa ze sciezkami, wiec maja dlugie nazwy
	name = s;
}

button::~button(void)
{
	//SDL_FreeSurface( this->icon );
}

void button::render()
{
	//if(visible)
	if(ogame.state == SETTINGS && ogame.adding_player){
		if(!name.compare("grafika/przyciski/komputer.png")){
			if(ogame.chose_computer) clip = &down;
			else clip = &up;
		}else if(!name.compare("grafika/przyciski/czlowiek.png")){
			if(!ogame.chose_computer) clip = &down;
			else clip = &up;
		}else if(!name.compare("grafika/przyciski/latwy.png")){
			if(ogame.chose_computer && ogame.computer_level==1) clip = &down;
			else clip = &up;
		}else if(!name.compare("grafika/przyciski/sredni.png")){
			if(ogame.chose_computer && ogame.computer_level==2) clip = &down;
			else clip = &up;
		}else if(!name.compare("grafika/przyciski/trudny.png")){
			if(ogame.chose_computer && ogame.computer_level==3) clip = &down;
			else clip = &up;
		}
	}

	ogame.apply_surface( box.x, box.y, icon, ogame.screen, clip );
}

//returns true if event was handled
bool button::handle_events()
{
    
	//if(!visible) return;

	//The mouse offsets
    int x = 0, y = 0;

/*
	if(ogame.state == SETTINGS){
		if(ogame.chose_computer){
			if(!name.compare("grafika/przyciski/komputer.png")) clip = &down;  
			if(ogame.computer_level==1 &&
				if(!name.compare("grafika/przyciski/latwy.png")) clip = &down;
			}else if(ogame.computer_level==2){
				if(!name.compare("grafika/przyciski/sredni.png")) clip = &down;
			}else if(ogame.computer_level==3){
				if(!name.compare("grafika/przyciski/trudny.png")) clip = &down;
			}
		}else if(!name.compare("grafika/przyciski/czlowiek.png")) clip = &down; 
	}
*/

    //If the mouse moved
    if( ogame.event.type == SDL_MOUSEMOTION )
    {
        //Get the mouse offsets
        x = ogame.event.motion.x;
        y = ogame.event.motion.y;
        
        //If the mouse is over the button
        if( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
        {
            //Set the button sprite
			//if(name.compare("grafika/przyciski/czlowiek.png")&&name.compare("grafika/przyciski/komputer.png")&&
//name.compare("grafika/przyciski/latwy.png")&&name.compare("grafika/przyciski/sredni.png")&&name.compare("grafika/przyciski/trudny.png"))
            clip = &down;    
        }
        //If not
        else
        {
            //Set the button sprite
            clip = &up;
        }    
    }
	//If a mouse button was pressed
    if( ogame.event.type == SDL_MOUSEBUTTONDOWN )
    {
        //If the left mouse button was pressed
        if( ogame.event.button.button == SDL_BUTTON_LEFT )
        {
            //Get the mouse offsets
            x = ogame.event.button.x;
            y = ogame.event.button.y;
        
            //If the mouse is over the button
            if( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
            {
                //Set the button sprite
                //clip = &down;
            }
        }
    }
	//If a mouse button was released
    if( ogame.event.type == SDL_MOUSEBUTTONUP )
    {
        //If the left mouse button was released
        if( ogame.event.button.button == SDL_BUTTON_LEFT )
        { 
            //Get the mouse offsets
            x = ogame.event.button.x;
            y = ogame.event.button.y;
        
            //If the mouse is over the button
            if( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
            {
                cout << name << endl;
				if(ogame.state==MENU){
					if(!name.compare("grafika/przyciski/ustawienia.png")){
						clip = &up;
						ogame.state = SETTINGS;
						SDL_EnableUNICODE( SDL_ENABLE ); //aby dzialala fajnie edycja imion graczy
						ogame.lista_zawodnikow.sort(porownaj_numery_startowe);
						/*if(ogame.full_screen){
							ogame.full_screen = false;
							ogame.screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
						}else{
							ogame.full_screen = true;
							ogame.screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE|SDL_FULLSCREEN );
						}*/

					}else if(!name.compare("grafika/przyciski/zawody.png")){
						clip = &up;
						ogame.state = HILL_SELECT;//GAME
					}else if(!name.compare("grafika/przyciski/pomoc.png")){
						clip = &up;
						ogame.state = HELP;
					}else if(!name.compare("grafika/przyciski/wyjscie.png")){
						exit(0);
					}
				}else if(ogame.state==SETTINGS){
					if(!name.compare("grafika/przyciski/wlaczony.png")){//wylaczony.png
						if(ogame.full_screen){
							SDL_FreeSurface( this->icon );
							icon = ogame.load_image("grafika/przyciski/wylaczony.png");
							
							ogame.full_screen = false;
							ogame.screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
						}else{
							SDL_FreeSurface( this->icon );
							icon = ogame.load_image("grafika/przyciski/wlaczony.png");

							ogame.full_screen = true;
							ogame.screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE|SDL_FULLSCREEN );
						}
					}else if(!name.compare("grafika/przyciski/delete.png")){

						int id = (box.y-124)/25;
						int i = 0;
						for (list<gracz>::iterator it=ogame.lista_zawodnikow.begin(); it != ogame.lista_zawodnikow.end(); ++it){
							i++;
							if(i == id){
								ogame.lista_zawodnikow.erase(it);
								clip = &up;
								break;
							}
						}
					}else if(!name.compare("grafika/przyciski/dodaj zawodnika.png")){
						ogame.adding_player = true;
						return true;

					}else if(!name.compare("grafika/przyciski/czlowiek.png")){
						ogame.chose_computer = false;
						ogame.computer_level = 0;
					}else if(!name.compare("grafika/przyciski/komputer.png")){
						ogame.chose_computer = true;
						ogame.computer_level = 1;
					}else if(!name.compare("grafika/przyciski/latwy.png")){
						ogame.computer_level = 1;
					}else if(!name.compare("grafika/przyciski/sredni.png")){
						ogame.computer_level = 2;
					}else if(!name.compare("grafika/przyciski/trudny.png")){
						ogame.computer_level = 3;

					}else if(!name.compare("grafika/przyciski/dodaj.png")){
						gracz G = { (ogame.chose_computer ? "Komputer" : "Czlowiek"),ogame.chose_computer,ogame.computer_level}; ogame.lista_zawodnikow.push_back(G);
						ogame.adding_player = false;
					}else if(!name.compare("edit")){
						ogame.edit_player_id = 1+(y-149)/25;
						int i = 0;
						for (list<gracz>::iterator it=ogame.lista_zawodnikow.begin(); it != ogame.lista_zawodnikow.end(); ++it){
							i++;
							if(ogame.edit_player_id == i){
								ogame.edit_name = &(*it).name;
								break;
							}
						}
						//cout<<"x:"<<x<<",y:"<<1+(y-149)/25<<endl;
					}
				
				}else if(ogame.state==HILL_SELECT){
					if(!name.compare("grafika/skocznie/gawronowo.png")){
						
						clip = &up;
						ogame.state = GAME;
						if(ogame.full_screen) SDL_ShowCursor(0);
						ogame.skocznia = ogame.lista_skoczni["Gawronowo"];//ogame.Gawronowo;
						C.reset_variables();
						//ogame.skocznia->skoczn->icon = ogame.load_image( ogame.skocznia->skoczn->filename );
						//ogame.skocznia->blue->icon = ogame.load_image( ogame.skocznia->blue->filename );
						//ogame.skocznia->blue2->icon = ogame.load_image( ogame.skocznia->blue2->filename );

						
						ogame.id_zawodnika = 0;
						ogame.lista_zawodnikow.sort(porownaj_numery_startowe);
						int i = 0;
						for (list<gracz>::iterator it=ogame.lista_zawodnikow.begin(); it != ogame.lista_zawodnikow.end(); ++it){
							i++;
							//if(i == 1){ogame.zawodnik = &(*it); ogame.id_zawodnika = 1; }//po nacisnieciu enera zostanie to obsluzone
							//(*it).numer_startowy=i;
							(*it).ogledlosci_skokow[0] = 0;
							(*it).ogledlosci_skokow[1] = 0;
							(*it).wynik = 0;
						}
						ogame.rekord_skoczni_type = 0;
						ogame.seria_skokow = 0;//zmienic na 0, jeden bedzie dopiero po intro skoczni
						ogame.zawody_info = true;//zmienic na true

					}else if(!name.compare("grafika/skocznie/jurowo.png")){
						
						clip = &up;
						ogame.state = GAME;
						if(ogame.full_screen) SDL_ShowCursor(0);
						ogame.skocznia = ogame.lista_skoczni["Jurowo"];//ogame.Jurowo;
						//ogame.skocznia->skoczn->icon = ogame.load_image( ogame.skocznia->skoczn->filename );
						//ogame.skocznia->blue->icon = ogame.load_image( ogame.skocznia->blue->filename );
						//ogame.skocznia->blue2->icon = ogame.load_image( ogame.skocznia->blue2->filename );
						C.reset_variables();
						
						ogame.id_zawodnika = 0;
						ogame.lista_zawodnikow.sort(porownaj_numery_startowe);
						int i = 0;
						for (list<gracz>::iterator it=ogame.lista_zawodnikow.begin(); it != ogame.lista_zawodnikow.end(); ++it){
							i++;
							//if(i == 1){ogame.zawodnik = &(*it); ogame.id_zawodnika = 1; }//po nacisnieciu enera zostanie to obluzone
							//(*it).numer_startowy=i;
							(*it).ogledlosci_skokow[0] = 0;
							(*it).ogledlosci_skokow[1] = 0;
							(*it).wynik = 0;
						}
						ogame.rekord_skoczni_type = 0;
						ogame.seria_skokow = 0;//zmienic na 0, jeden bedzie dopiero po intro skoczni
						ogame.zawody_info = true;//zmienic na true
						
					}
				}
			}
        }
	}
	return false;
}