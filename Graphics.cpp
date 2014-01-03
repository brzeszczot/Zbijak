/*
 * Graphics.cpp
 *
 *  Created on: Nov 18, 2012
 *      Author: brzeszczot
 */

#include "Graphics.hpp"

MAPoint2d Graphics::screenSize;
MAPoint2d Graphics::gameAreaSize;
MAPoint2d Graphics::pointerPressedCoords;
MAPoint2d Graphics::pointerDraggedCoords;
MAPoint2d Graphics::pointerReleasedCoords;
MAPoint2d Graphics::map_moving;

Graphics::Graphics()
{
	graph_buffer = maCreatePlaceholder();
	int ssize = maGetScrSize();
	screenSize.x = EXTENT_X(ssize);
	screenSize.y = EXTENT_Y(ssize);
	map_moving.x = map_moving.y = 0;
	maCreateDrawableImage(graph_buffer, screenSize.x, screenSize.y);

	// przygotowanie tla
	gameAreaSize.x = screenSize.x*2;
	gameAreaSize.y = screenSize.y*2;
	MAHandle bg_picture = RES_BG;
	bgSize.x = EXTENT_X(maGetImageSize(bg_picture));
	bgSize.y = EXTENT_Y(maGetImageSize(bg_picture));
	bg_buffer = maCreatePlaceholder();

	maCreateDrawableImage(bg_buffer, gameAreaSize.x, gameAreaSize.y);
	maSetDrawTarget(bg_buffer);

	maSetColor(0xd7edfb);
	maFillRect(0, 0, gameAreaSize.x, gameAreaSize.x);

	// jesli mala rozdzielczosc ekranu to wyswietl tlo troche nizej :)
	top_y = 0;
	if(screenSize.y < LOW_RES_Y)
		top_y = (LOW_RES_Y - screenSize.y)*-1;

	maDrawImage(bg_picture, 0, top_y);

	maDrawImage(bg_buffer, 0, 0);
	maSetDrawTarget(graph_buffer);

	mDefaultFont = maFontLoadDefault(FONT_TYPE_SANS_SERIF, FONT_STYLE_BOLD, 30);
	maFontSetCurrent(mDefaultFont);
	mDefaultFontSmall = maFontLoadDefault(FONT_TYPE_SANS_SERIF, FONT_STYLE_BOLD, 15);
}

void Graphics::printLogo()
{
	MAHandle res_logo = RES_LOGO;
	int size_x = EXTENT_X(maGetImageSize(res_logo));
	maDrawImage(res_logo, (int)(screenSize.x/2)-(size_x/2), 50);
	maDrawImage(RES_INFO, (int)(screenSize.x/2)-(59/2), 220);
/*
	MAHandle font = maFontLoadDefault(FONT_TYPE_SANS_SERIF, FONT_STYLE_BOLD, 10);
	maFontSetCurrent(font);
	maSetColor(0x888888);
	maDrawText(1, screenSize.y-13, "brzeszczot@gmail.com - 2012");
	maFontSetCurrent(mDefaultFont);
*/
}

void Graphics::printMenu(std::string str)
{
	maSetColor(BLACK);
	maFillRect(0, (int)screenSize.y-MENU_HEIGHT, screenSize.x, MENU_HEIGHT);
	maSetColor(0x222222);
	maLine(0, (int)(screenSize.y-MENU_HEIGHT)-1, screenSize.x, (int)(screenSize.y-MENU_HEIGHT)-1);
	maSetColor(0x444444);
	maLine(0, (int)(screenSize.y-MENU_HEIGHT)-2, screenSize.x, (int)(screenSize.y-MENU_HEIGHT)-2);
	maSetColor(0x666666);
	maLine(0, (int)(screenSize.y-MENU_HEIGHT)-3, screenSize.x, (int)(screenSize.y-MENU_HEIGHT)-3);
	maSetColor(WHITE);
	maDrawText(10, (int)(screenSize.y-MENU_HEIGHT)+10, str.c_str());
}

void Graphics::printObject(Object *obj)
{
	if(obj->visible)
	{
		if(0)//obj->border)
		{
			const int border_size = 2;
			maSetColor(RED);
			maLine((int)(obj->coords.x-border_size)+map_moving.x, (int)(obj->coords.y-border_size)+map_moving.y, (int)(obj->coords.x+(int)obj->animation.frame_size.x+border_size)+map_moving.x, (int)(obj->coords.y-border_size)+map_moving.y);
			maLine((int)(obj->coords.x-border_size)+map_moving.x, (int)(obj->coords.y+(int)obj->animation.frame_size.y+border_size)+map_moving.y, (int)(obj->coords.x+(int)obj->animation.frame_size.x+border_size)+map_moving.x, (int)(obj->coords.y+(int)obj->animation.frame_size.y+border_size)+map_moving.y);
			maLine((int)(obj->coords.x-border_size)+map_moving.x, (int)(obj->coords.y-border_size)+map_moving.y, (int)(obj->coords.x-border_size)+map_moving.x, (int)obj->coords.y+(int)(obj->animation.frame_size.y+border_size)+map_moving.y);
			maLine((int)(obj->coords.x+(int)obj->animation.frame_size.x+border_size)+map_moving.x, (int)(obj->coords.y-border_size)+map_moving.y, (int)(obj->coords.x+(int)obj->animation.frame_size.x+border_size)+map_moving.x, (int)(obj->coords.y+(int)obj->animation.frame_size.y+border_size)+map_moving.y);
			//maFillRect(obj->coords.x-2, obj->coords.y-2, obj->size.x+4, obj->size.y+4);
		}

		if(obj->animation.isAnimation)
		{
			MARect rect;
			MAPoint2d point;
			point.x = (int)obj->coords.x + map_moving.x;
			point.y = (int)obj->coords.y + map_moving.y;
			rect.left = (int)obj->animation.frame_size.x * (int)obj->animation.frame.x;
			rect.top = (int)obj->animation.frame_size.y * (int)obj->animation.frame.y;
			rect.width = (int)obj->animation.frame_size.x;
			rect.height = (int)obj->animation.frame_size.y;
			maDrawImageRegion(obj->res_file, &rect, &point, TRANS_NONE);
		}
		else
		{
			if(obj->owner != 0 && obj->group == Object::SNOWBALL)
			{
				if(top_y<0)
				{
					if(obj->coords.y <= screenSize.y/9)	obj->visible = false;
					else if(obj->coords.y <= screenSize.y/7)	setResource(obj, RES_SNOWBALL_8);
					else if(obj->coords.y <= screenSize.y/5)	setResource(obj, RES_SNOWBALL_7);
					else if(obj->coords.y <= screenSize.y/4)	setResource(obj, RES_SNOWBALL_6);
					else if(obj->coords.y <= screenSize.y/3)	setResource(obj, RES_SNOWBALL_5);
					else if(obj->coords.y <= screenSize.y/2)	setResource(obj, RES_SNOWBALL_4);
				}
				else
				{
					if(obj->coords.y <= screenSize.y/9)	obj->visible = false;
					else if(obj->coords.y <= screenSize.y/9)	setResource(obj, RES_SNOWBALL_8);
					else if(obj->coords.y <= screenSize.y/8)	setResource(obj, RES_SNOWBALL_7);
					else if(obj->coords.y <= screenSize.y/7)	setResource(obj, RES_SNOWBALL_6);
					else if(obj->coords.y <= screenSize.y/6)	setResource(obj, RES_SNOWBALL_5);
					else if(obj->coords.y <= screenSize.y/5)	setResource(obj, RES_SNOWBALL_4);
					else if(obj->coords.y <= screenSize.y/4)	setResource(obj, RES_SNOWBALL_3);
					else if(obj->coords.y <= screenSize.y/3)	setResource(obj, RES_SNOWBALL_2);
					else if(obj->coords.y <= screenSize.y/2)	setResource(obj, RES_SNOWBALL_1);
				}
			}

			int dir;
			MARect rect;
			MAPoint2d point;
			point.x = (int)obj->coords.x + map_moving.x;
			point.y = (int)obj->coords.y + map_moving.y;
			rect.left = 0;
			rect.top = 0;
			rect.width = (int)obj->size.x;
			rect.height = (int)obj->size.y;
			if(obj->direction==0)
				maDrawImageRegion(obj->res_file, &rect, &point, TRANS_MIRROR);
			else
				maDrawImageRegion(obj->res_file, &rect, &point, TRANS_NONE);

			//maDrawImage(obj->res_file, (int)obj->coords.x + map_moving.x, (int)obj->coords.y + map_moving.y);
		}
	}
}

void Graphics::printBackground(bool isMenuOn, int gamePoints, int game_level, int sledge_counter, int round_treshold, int sledge_hit, int life)
{
	//maWait(20);
	maSetDrawTarget(graph_buffer);
	maDrawImage(bg_buffer, map_moving.x, map_moving.y);

	if(!isMenuOn)
	{
		char buffor[10];
		maSetColor(0xbf945c);

		// narysuj prezenty
		switch(life)
		{
			case 3: maDrawImage(RES_GIFT, 86, 299 + top_y);
			case 2: maDrawImage(RES_GIFT, 106, 304 + top_y);
			case 1: maDrawImage(RES_GIFT, 126, 298 + top_y);
		}

		if(screenSize.x > 320)
		{
			maFontSetCurrent(mDefaultFont);
			maSetColor(0xbf945c);
			maDrawImage(RES_PUNKTACJA, 0, 0);
			maDrawImage(RES_RUNDA, (screenSize.x-113)-40, 2);

			itoa((int)gamePoints, buffor, 10);
			maDrawText(165, 2, buffor);
			itoa((int)game_level, buffor, 10);
			maDrawText(screenSize.x-42, 2, buffor);

			// rysuj pozostale do odstrzalu
			for(int ii=0;ii<(round_treshold-sledge_hit);ii++)
				maDrawImage(RES_SPLASH_M, (26*ii)+3, 40);
		}
		else
		{
			maFontSetCurrent(mDefaultFontSmall);
			maSetColor(0xbf945c);
			maDrawText(2, 2, "Punktacja:");
			maDrawText(screenSize.x-80, 2, "Runda:");

			itoa((int)gamePoints, buffor, 10);
			maDrawText(80, 2, buffor);
			itoa((int)game_level, buffor, 10);
			maDrawText(screenSize.x-30, 2, buffor);
		}

		maFontSetCurrent(mDefaultFont);
		//maDrawTextW(5, screenSize.y-23, L"Brzeszczot"); // L
	}

	//maDrawImage(RES_IMAGE_MY, 0, 0);
	//maSetDrawTarget(HANDLE_SCREEN);
	//MARect myRect;
	//maSetClipRect(0,0,100,100);
}

void Graphics::printOnScreen()
{
	///// temp:
//	maSetColor(0x0000FF);
//	maLine(Graphics::pointerPressedCoords.x, Graphics::pointerPressedCoords.y, Graphics::pointerDraggedCoords.x, Graphics::pointerDraggedCoords.y);
	/////////////////

	maDrawImage(graph_buffer, 0, 0);
	maUpdateScreen();
}

void Graphics::moveMap()
{
	int temp_x = std::max(pointerDraggedCoords.x, pointerPressedCoords.x) - std::min(pointerDraggedCoords.x, pointerPressedCoords.x);
	int temp_y = std::max(pointerDraggedCoords.y, pointerPressedCoords.y) - std::min(pointerDraggedCoords.y, pointerPressedCoords.y);
	if(pointerDraggedCoords.x < pointerPressedCoords.x)
		temp_x *= -1;
	if(pointerDraggedCoords.y < pointerPressedCoords.y)
		temp_y *= -1;
	map_moving = map_moving_temp;
	map_moving.x += temp_x;
	map_moving.y += temp_y;
	if(map_moving.x>0)	map_moving.x = 0;
	if(map_moving.y>0)	map_moving.y = 0;
	if((map_moving.x + gameAreaSize.x)<(screenSize.x))	map_moving.x = screenSize.x - gameAreaSize.x;
	if((map_moving.y + gameAreaSize.y)<(screenSize.y))	map_moving.y = screenSize.y - gameAreaSize.y;
}

void Graphics::setResource(Object *obj, int res)
{
	obj->res_file = res;
	obj->size.x = EXTENT_X(maGetImageSize(res));
	obj->size.y = EXTENT_Y(maGetImageSize(res));
	if(obj->animation.isAnimation)
	{
		obj->animation.frame_size.x = (int)(obj->size.x/ANIM_FRAMES);
		obj->animation.frame_size.y = (int)(obj->size.y/ANIM_FRAMES);
	}
	else
	{
		obj->animation.frame_size.x = (int)obj->size.x;
		obj->animation.frame_size.y = (int)obj->size.y;
	}
}

Adds::vertex Graphics::MAPoint2d_to_vertex(MAPoint2d coord)
{
	Adds::vertex vertex;
	vertex.x = coord.x;
	vertex.y = coord.y;
	return vertex;
}

Graphics::~Graphics()
{
	maDestroyPlaceholder(graph_buffer);
	maDestroyPlaceholder(bg_buffer);
}

/*
		MARect rect;
		rect.left = 0;
		rect.top = 0;
		rect.width = 320;
		rect.height = 620;
		MAUI::Scaler::Scaler 	(obj->res_file, &rect, 0.5,0.5,1,MAUI::Scaler::ST_BILINEAR);
*/

