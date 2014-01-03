/*
 * Object.hpp
 *
 *  Created on: Nov 18, 2012
 *      Author: brzeszczot
 */

#ifndef OBJECT_HPP_
#define OBJECT_HPP_
#define RES_GROUP_NUM 	3
#define ANIM_FRAMES 	4
#define MIN_SPEED		150
#define MAX_SPEED		30
#define DIRECTION_UP		0
#define DIRECTION_RIGHT		1
#define DIRECTION_DOWN		2
#define DIRECTION_LEFT		3

#include <string>
#include <vector>
#include "Adds.hpp"
#include "Alg.hpp"

class Object
{
    public:
		typedef enum e_stat {STOP, MOVING, FIGHT, WORK, DEAD};
		typedef enum e_group {DEFAULT, SNOWBALL, SLEDGE, SPLASH, BG};
		struct parameters
        {
            int life;               // zycie
            int gold;               // zloto :)
            int skills;				// umiejetnosci
            int visible_dist;       // na jaka odleglosc objekt potrafi widziec
            int move_speed;         // szybkosc poruszania sie
            int range;          	// zasieg
        };
		struct s_animation
		{
			bool isAnimation;		// czy zasób jest obrazkiem czy animacja
			bool animate;			// animuj lub nie animuj
			int timer;				// milisekundy
			int speed;				// co ile milisekund zmien animacje
			Adds::vertex frame;		// x => 0-4 klatka animacji w poziomie, y => 0-4 zestaw klatek w pionie (4 kierunki swiata)
			Adds::vertex frame_size;// rozmiar malej klatki 1 z (4*4)
		};
		s_animation animation;
        Object();
        virtual ~Object();
        void Stop();				// zatrzymaj obiekt
        void Update(int current_time);	// zaktualizuj jego parametry
        void Go(Adds::vertex dest_coord, int current_time);	// idz do okreslonej wspolrzednej
        void Go();					// idz tam gdzie miales isc :)
        void setSpeed();
        void reset();
        void changeMovement();		// zmien pozycje obiektu
        unsigned int getDirection(Adds::vertex p1, Adds::vertex p2);	// oblicz pozycje p2 wzgledem p1 - ret 0-4 kier. swiata
        parameters params;
        e_group group;				// grupa
        e_stat status;				// status objektu
		int res_file;				// przypisany obrazek do obiektu
		size_t owner;				// kto jest wlascicielem obiektu
		Adds::vertex coords;		// wspolrzedne obiektu
		Adds::vertex size;			// rozmiar obrazka
		int start_y;
		int direction;
		int last_direction;
		int isColision;
		Adds::vertex destination_last;	// wspolrzedne gdzie ma sie przemieszczac obiekt - tymczasowa ostatnia
		Adds::vertex destination;	// wspolrzedne gdzie ma sie przemieszczac obiekt
		int destination_current;	// aktualna wspolrzedna z trasy drogi docelowej - czyli index z ponizszego vectora
		std::vector<Adds::vertex> *destination_coords;	// wspolrzedne odcinka destynacji
		int move_timer;				// milisekundy
		bool visible;				// czy widoczny
		bool isWall;				// czy przez obiekt mozna przejsc
		bool border;				// czy wyswietlac obramowanie
		int time;					// aktualny czas w milisekundach
		int live_time;
    private:
		void changeAnim();			// zaktualizuj klatke animacji
};

#endif /* OBJECT_HPP_ */
