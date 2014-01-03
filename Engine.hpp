/*
 * Engine.hpp
 *
 *  Created on: Nov 17, 2012
 *      Author: brzeszczot
 */

#ifndef ENGINE_HPP_
#define ENGINE_HPP_
//   /data/data/com.mosync./files/MAStore
#define PATH "/data/data/com.brzeszczot.zbijak/files/" //"/mnt/sdcard/"
#define CONF_FILE_NAME "zbijak"

#define DEFAULT_WORLD_OWNER 0
#define DEFAULT_HUMAN_OWNER 1

#define POINT_PER_SLOW_SLEDGE 	2		// ilosc punktow dla wolnych sanek
#define POINT_PER_MEDIUM_SLEDGE 4		// srednio szybkich
#define POINT_PER_FAST_SLEDGE 	6		// szybkich

#define POSIBLE_SLEDGE			6		// ilosc rodzajow sanek

#define SNOWBALLS_Q 			30		// ilosc sniezek na runde - moze dynamicznie malec w zaleznosci od kolejnej rundy (30)
#define SLEDGE_Q 				30		// ilosc wyrzucanych sanek na runde			(30)
#define ROUND 					10		// poczatkowy prog ile trzeba trafic sanek 	(10)
#define FIRST_LEVEL				1		// numer pierwszej rundy
#define MAX_LIFE				3		// ilosc prob nieudanego przejscia rundy

#define STANDING_AREA_HEIGHT 	30
#define THROW_AREA_HEIGHT 		200
#define GAME_AREA_TOP 			300
#define GAME_AREA_HEIGHT 		145

//#include <mastdlib.h>
#include <MAUtil/Moblet.h>
//#include <DateTime.h> // map.lib
#include <ctime>
#include <MAHeaders.h>
#include <ma.h>
#include <conprint.h>
#include <maxtoa.h>
#include <string>
#include <vector>
#include "Adds.hpp"
#include "Object.hpp"
#include "Graphics.hpp"
#include "Polygon.hpp"

using namespace MAUtil;

class Engine: public Moblet
{
	public:
		Engine();
		virtual ~Engine();
		void Run();									// uruchom program
		void debugPrintInt(int var);				// print integer na ekran
		bool checkObjectSelection(Adds::vertex coord);	// sprawdz ktory obiekt zaznaczono
		Graphics *graphics;							// wksaznik do obiektu grafiki
		std::vector<Object*> *objects;				// kontener z obiekatami
		Object *selectedHumanObject;				// trzymaj wskaznik aktualnie zaznaczonego obiektu
		Object *lastThrowed;						// ostatnie wypuszczone saneczki
	private:
		bool isRunning;								// warunek glownej petli
		bool isRun;									// gra w toku
		int gamePoints;								// punktacja zdobyta w grze
		int gamePointsTemp;							// temp
		int snowballs_q;							// ilosc sniezek do rzucania
		int sledge_q;								// ilosc sanek na poziom
		int sledge_counter;							// zliczaj wyrzucone sanki
		int sledge_hit;								// zliczaj trafione
		int round_treshold;							// prog ile trzeba zestrzelic sanek na runde
		int life;
		int xtime;									// aktualny timestamp
		bool isMenuOn;								// czy menu jest wlaczone
		int game_level;								// poziom gry od zera
		int fps, fps_time, fps_print;				// zmienne pomocnicze do pomiaru klatek na sekunde
		char buffer[256];
		int record_points;
		int record_level;
		bool record_new;
		char dateNow[11];
		char timeNow[6];
		void doLogic();								// logika gry
		void doGraphics();							// grafika
		void createObject();						// wyrzucaj sanki na ekran
		void createSnowBalls();						// wygeneruj sniezki
		void createSplash(Adds::vertex vert);		// wygeneruj plame po zderzeniu
		void deleteAllObjects();					// usun wszsytkie obiekty - koniec rundy
		void startGame();
		void stopGame();
		void saveResults();
		void readResults();
		void getDate();
		void getTime();
		Object* checkColision(Object* obj);			// sprawdzanie kolizji
		bool checkOverlap(Adds::vertex vert, Object *obj);					// spr nakladania sie dwoch prostokatow
		Adds::vertex calcFarPoint(Adds::vertex start, Adds::vertex end);	// oblicz do kad ma leciec sniezka
		void getCornerCoord(Adds::vertex coords, Adds::vertex size, unsigned int corner, Adds::vertex &vert);	// zwroc wspolrzedne kazdego z rogo prostokata
};

#endif
