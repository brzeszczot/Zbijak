/*
 * Graphics.hpp
 *
 *  Created on: Nov 18, 2012
 *      Author: brzeszczot
 */

#ifndef GRAPHICS_HPP_
#define GRAPHICS_HPP_

#define WHITE 	0xAAAAAA
#define BLACK 	0x000000
#define RED		0xCC0000
#define GREEN	0x00AA00
#define MENU_HEIGHT 70
#define LOW_RES_Y 600

#include <MAUtil/Moblet.h>
#include <conprint.h>
#include <maxtoa.h>
#include <MAHeaders.h>
#include <string>
#include <vector>
#include "Adds.hpp"
#include "Object.hpp"
//#include <MAUI/Scaler.h> // , maui.lib // ,  %mosync-home%/include/MAUI/

class Graphics
{
	public:
		Graphics();
		virtual ~Graphics();
		void printOnScreen();
		void printBackground(bool isMenuOn, int gamePoints, int game_level, int sledge_counter, int round_treshold, int sledge_hit, int life);
		void printLogo();
		void printObject(Object *obj);
		void printMenu(std::string str);
		void setResource(Object *obj, int res);
		static Adds::vertex MAPoint2d_to_vertex(MAPoint2d coord);
		void moveMap();
		MAPoint2d bgSize;							// rozmiar pojedynczego obrazka tla
		static MAPoint2d screenSize;				// rozdzielczosc ekranu
		static MAPoint2d gameAreaSize;				// rozdzielczosc pola gry
		static MAPoint2d pointerPressedCoords;		// wspolrzedne po kliknieciu
		static MAPoint2d pointerDraggedCoords;		// wspolrzedne podczas przesuwania
		static MAPoint2d pointerReleasedCoords;		// wspolrzedne zwolnienia przycisku
		MAPoint2d map_moving_temp;
		static MAPoint2d map_moving;
		int top_y;
	private:
		MAHandle graph_buffer;
		MAHandle bg_buffer;
		MAHandle mDefaultFont;
		MAHandle mDefaultFontSmall;
};

#endif /* GRAPHICS_HPP_ */
