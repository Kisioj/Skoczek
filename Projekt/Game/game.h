/*
---SPRAWOZDANIE---
- w grze logo uczelni, nazwa gry, nasze imiona i nazwiska, Warszawa 2013


Wstêp:
	Niniejsze sprawozdanie dotyczy gry skoki narciarskie, które polega na:

1. Rozbudowany opis, opis problemu, wszystkie mozliwe przypadki. (Opis modelu, gry)

2. Algorytmy pisane w punktach, krok po kroku. (nie s³ownie; duzo tresci, malo lania wody)

3. Opis najwazniejszych procedur, kawalki kodu i co tam sie dzieje.
	Tzn. Kod z objasnieniami (komentarzami) a pod spodem opis?
	Nie moze byc czegos takiego, jak:
	2 linijki kodu, 15 linijek tekstu, 2 linijki kodu, 15 linijek tekstu etc.
	Albo opis na poczatku, albo na koncu kodu.

4. System plików


*/

#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_rotozoom.h"
#include "SDL_mixer.h"
#include <string>
#include <sstream>
#include <cmath>
//#include <iostream>
#include <fstream>
#include <list>
#include <map>
#include <ctime>
#include <cstdlib>
#include "slope.h"
#include "camera.h"
#include "hill.h"
using namespace std;

//physic constants
const double RADIAN = 0.0174532925;
const double PI = 3.1415926535;
const double G = 9.81;

//The attributes of the screen
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//przelicznik z metrow na pixele
const double METER_TO_PIXEL = 16.58;

//maxymalny kat pomiedzy nartami a narciarzem w locie
const double MAX_ANGLE_VARIANCE = PI/2; //90 stopni, wczesniej ustawilem PI/3
//minimalny kat pomiedzy nartami a narciarzem w locie
const double MIN_ANGLE_VARIANCE = PI/18; //10 stopni

//zakres kataw przy ktorych nasz skoczek sie nie wywali przy ladowaniu
//w stopniach
const double MIN_JUMPER_ANGLE = 60.0;
const double MAX_JUMPER_ANGLE = 110.0;

const double MIN_SKIS_ANGLE = -10.0;
const double MAX_SKIS_ANGLE = 50.0;

enum state {INTRO, TITLE, TO_MENU, MENU, GAME, HELP, SETTINGS, HILL_SELECT };

struct narty { // po upadku
	SDL_Rect position;
	double x_precise,y_precise;
	double FRICTION_FACTOR;
	double v;
	double alpha;

};

struct gracz{
	
	string name;	  //player will be able to modify this
	bool is_computer; //false-human, true-compuer
	short int level;  //1-easy,2-medium,3-hard
	double ogledlosci_skokow[2]; //odleglosc skoku w 1 serii - indeks [0] i 2giej serii - indeks [1]
	double wynik; //laczny wynik odleglosci w 2 seriach
	int numer_startowy; //ID, od 1 do 10
 };

class game
{
public:
	bool full_screen;
	state state; //1 = "title screen", 2 = "menu",3 = "game"// etc
	hill* skocznia; //Gawronowo lub Jurowo
	gracz* zawodnik;
	narty narty_po_upadku; //struktura potrzebna do renderowania odjezdzajacych nart po upadku zawodnika
	int id_zawodnika; //id skaczacego wlasnie zawodnia, poczatkowo 1
	int seria_skokow; //seria 1, a po niej seria 2ga
	map<string,hill*> lista_skoczni;
	list<gracz> lista_zawodnikow;

	bool skok_zakonczony; //domyslnie false,  true kiedy jeden zawodnik oddal juz skok, a kolejny jeszcze nie wystartowal
	bool zawody_info; //true kiedy na ekranie wyswietlaja sie informacje o zawodnikach i skokach w 1 i 2giej serii

	//for timer's purposes only
	int start;
	int red_light_start;

	SDL_Surface* screen;
	
	SDL_Surface* wind_bg;
	SDL_Surface* wind_arrow;
	SDL_Surface* sygnalisator;
	SDL_Surface* lights;
	SDL_Surface* title_screen;
	SDL_Surface* login_screen;
	SDL_Surface* help_screen;

	SDL_Surface* ramka;
	SDL_Surface* ramka2;
	SDL_Surface* ramka_przed_zawodami;
	SDL_Surface* ramka_po_1_serii;
	SDL_Surface* ramka_po_2_serii;
	SDL_Surface* new_rekord_icon; //wyswietla sie po 1 lub 2 serii, jezeli rekord zostal pobity w obecnych zawodach
	SDL_Surface* rekord_skoczni;
	short int rekord_skoczni_type;//0 - zaladowany z pliku, 1 - wlasnie pobity, 2 - pobity w tych zawodach

	double FRICTION_FACTOR;
	
	bool adding_player;    //true jezeli uzytkownik wlasnie dodaje nowego gracza
	bool chose_computer;   //domyslnie true oraz jezeli zaznaczono, ze dodany gracz ma byc komputerem
	int computer_level;    //0-czlowiek, 1 - latwy, 2 - sredni, 3 - trudny

	double wind_vx;			//horizontal velocity of the wind
	double wind_vy;			//vertical velocity of the wind
	int wind_change_time;	//used to calculate wind every x seconds
	double wind_vx_change;	//wind's velocity x can change a little over a time
	double wind_vy_change;	//wind's velocity y van change a little over a time

	bool green_light;			//false at the start - red light
	bool show_title_screen;	//returns true if user is on the title screen, else returns false

	//potrzebne do wyswietlania napisow na ekranie
	TTF_Font *font_vgafixe_10;
	TTF_Font *font_verdana_24;
	TTF_Font *font_courier_14;
	SDL_Surface* message;// = NULL;
	SDL_Color textColor;// = { 255, 255, 255 };
	SDL_Color blackColor;// = {0,0,0};
	stringstream stream;

	//potrzebne do edytowania imion graczy
	SDL_Surface* karetka; //grafika 'karetki'
	int edit_player_id; //id gracza, ktorego imie jest obecnie edytowane
	string* edit_name; //wskaznik do imiona gracza obecnie edytowanego
	bool just_edited;  //zmienia sie na true kiedy dodamy lub usuniemy z nazwy litere
	int last_edit_time; //SDL_GetTicks(), przeciwko zbyt szybkiej zmianie imion - spam


	SDL_Event event;

	game(void);
	~game(void);

	SDL_Surface* load_image( std::string filename );
	void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL);

	double collision_check(double x, double y);
	void keyboard_handle();
	void ai_handle();
	void cycle();
	void render_HUD();
	void render_Text(int x, int y, SDL_Surface* message, TTF_Font* font, stringstream& stream, SDL_Color& textColor, SDL_Color& outlineColor, int outline_type = 2, bool UTF8 = false);
	//outline_type: 0 - bez obwódki, 1 - cieñ, 2 - obramowanie

	//metody sluzace do obslugi plikow zewnetrznych
	void odczyt_danych();
	void zapis_danych();
};


bool porownaj_numery_startowe (gracz player_1, gracz player_2);