/*
 * Engine.cpp
 *
 *  Created on: Nov 17, 2012
 *      Author: brzeszczot
 */

#include "Engine.hpp"

Engine::Engine()
{
	srand(maTime());

	objects = new std::vector<Object*>;
	graphics = new Graphics();

	isMenuOn = true;
	isRun = false;
	game_level = FIRST_LEVEL;
	snowballs_q = SNOWBALLS_Q;
	sledge_q = SLEDGE_Q;
	sledge_counter = 0;
	sledge_hit = 0;
	round_treshold = ROUND;
	life = MAX_LIFE;
	xtime = 0;
	record_points = record_level = 0;
	record_new = false;
	selectedHumanObject = NULL;

	strcpy(buffer, PATH);
	strcat(buffer, CONF_FILE_NAME);

	Run();
}

Engine::~Engine()
{
    for(size_t ii=0;ii<(size_t)objects->size();ii++)
        delete (*objects)[ii];
    objects->clear();
	delete graphics, objects;
}


void Engine::Run()
{
	MAEvent event;
	fps_time = maGetMilliSecondCount();
	readResults();

/*
	MAAudioData mData[3];
	MAAudioInstance mInstance[3];
	for(int ii=0;ii<3;ii++)
	{
		mData[ii] = maAudioDataCreateFromResource(NULL,(AUDIO_MUSIC+ii),0,maGetDataSize((AUDIO_MUSIC+ii)),0);
		mInstance[ii] = maAudioInstanceCreate(mData[ii]);
		maAudioPlay(mInstance[ii]);
	}
*/
/*
	MAAudioData mData = maAudioDataCreateFromResource(NULL,(AUDIO_HIT),0,maGetDataSize((AUDIO_HIT)),0);
	 if(mData<0)
	    maPanic(-1, "Couldn't initiate sound resources");
	MAAudioInstance mInstance = maAudioInstanceCreate(mData);
    if(mInstance<0)
        maPanic(-1, "Couldn't create sound instance");
	int ret = maAudioPlay(mInstance);
    if(ret<0)
        maPanic(-1, "Couldn't play");
    debugPrintInt(ret);
	maSoundSetVolume(50);
*/
/*
        if(mData<0)
            maPanic(-1, "Couldn't initiate sound resources");

        mInstance = maAudioInstanceCreate(mData);
        if(mInstance<0)
            maPanic(-1, "Couldn't create sound instance");

        maAudioStop(mInstance);
        maAudioSetPosition(mInstance, 0);
        maAudioSetVolume(mInstance, 1.0f);
        maAudioSetNumberOfLoops(mInstance,10);
        maAudioPlay(mInstance);
 */
	maSoundPlay(AUDIO_MUSIC, 0, maGetDataSize(AUDIO_MUSIC));
	maSoundSetVolume(50);
	maAudioSetNumberOfLoops(AUDIO_MUSIC,-1);

	isRunning = true;
	while (isRunning)
	{
		if(!isMenuOn)
			doLogic();
		doGraphics();
		//if(objects->size()>40)
		//deleteAllObjects();

		// take events
		while (maGetEvent(&event))
		{
			// keyCode 293 AND NativeCode 82,
			if(event.nativeKey==82)
			{
				isMenuOn = true;
				if(!maSoundIsPlaying())
					maSoundPlay(AUDIO_MUSIC, 0, maGetDataSize(AUDIO_MUSIC));
			}
			if (EVENT_TYPE_CLOSE == event.type)
			{
				maExit(0);
				isRunning = false;
				break;
			}
			else if (EVENT_TYPE_KEY_PRESSED == event.type)
			{
				switch(event.key)
				{
					case MAK_BACK: if(isMenuOn) maExit(0); break;
				}
			}
			else if (EVENT_TYPE_POINTER_DRAGGED == event.type)
			{
				Graphics::pointerDraggedCoords.x = event.point.x;
				Graphics::pointerDraggedCoords.y = event.point.y;
				//graphics->moveMap();
				if(event.point.y >= (Graphics::screenSize.y - STANDING_AREA_HEIGHT*2))
					checkObjectSelection(graphics->MAPoint2d_to_vertex(event.point));

				if(!isMenuOn && event.point.y <= (Graphics::screenSize.y - THROW_AREA_HEIGHT) && selectedHumanObject!=NULL && selectedHumanObject->status == Object::STOP)
				{
					selectedHumanObject->Go(calcFarPoint(selectedHumanObject->coords, graphics->MAPoint2d_to_vertex(event.point)), maGetMilliSecondCount());
					maSoundPlay(AUDIO_THROW, 0, maGetDataSize(AUDIO_THROW));
				}
			}
			else if (EVENT_TYPE_POINTER_PRESSED == event.type)
			{
				Graphics::pointerPressedCoords.x = event.point.x;
				Graphics::pointerPressedCoords.y = event.point.y;
				if(isMenuOn)
				{
					if(event.point.y >= 220 && event.point.y <= (220+59) && event.point.x >= (int)(Graphics::screenSize.x/2)-(59/2) && event.point.x <= (int)(Graphics::screenSize.x/2)+(59/2))
					{
						char buffor[1024];
						char buffor_int[10];
						strcpy(buffor,"Celem gry w każdej rundzie jest zbijanie memów :)\n\nRekordowy poziom: ");
						itoa((int)record_level, buffor_int, 10);
						strcat(buffor,buffor_int);
						strcat(buffor,"\nRekordowe punkty: ");
						itoa((int)record_points, buffor_int, 10);
						strcat(buffor,buffor_int);
						strcat(buffor,"\nz dnia: ");
						strcat(buffor,dateNow);
						strcat(buffor," ");
						strcat(buffor,timeNow);
						strcat(buffor,"\n\nbrzeszczot@gmail.com - 2012");
						maMessageBox("O grze", buffor);
					}
					else
					{
						isMenuOn = false;
						maSoundStop();
						if(!isRun)
							startGame();
					}
				}
				else
					checkObjectSelection(graphics->MAPoint2d_to_vertex(event.point));
			}
			else if (EVENT_TYPE_POINTER_RELEASED == event.type)
			{
				Graphics::pointerReleasedCoords.x = event.point.x;
				Graphics::pointerReleasedCoords.y = event.point.y;
				if(!isMenuOn && selectedHumanObject!=NULL && selectedHumanObject->status == Object::STOP && event.point.y <= (Graphics::screenSize.y - (STANDING_AREA_HEIGHT+selectedHumanObject->size.y)))
				{
					selectedHumanObject->Go(calcFarPoint(selectedHumanObject->coords, graphics->MAPoint2d_to_vertex(event.point)), maGetMilliSecondCount());
					maSoundPlay(AUDIO_THROW, 0, maGetDataSize(AUDIO_THROW));
				}
			}
		}
	}
}

void Engine::startGame()
{
	snowballs_q = (SNOWBALLS_Q - game_level) + 1;
	if(snowballs_q < ROUND)
		round_treshold = snowballs_q;
	else
		round_treshold = ROUND;

	gamePointsTemp = gamePoints;
	isRun = true;
	sledge_hit = 0;
	createSnowBalls();
	maSoundStop();
}

void Engine::stopGame()
{
	selectedHumanObject = NULL;
	isRun = false;
	deleteAllObjects();
	isMenuOn = true;
	sledge_counter = 0;
	maSoundPlay(AUDIO_MUSIC, 0, maGetDataSize(AUDIO_MUSIC));
}

void Engine::readResults()
{
	FILE *file_r = fopen(buffer, "r");
	if(file_r)
	{
		fscanf (file_r, "%d\n", &record_points);
		fscanf (file_r, "%d\n", &record_level);
		fscanf (file_r, "%s\n", dateNow);
		fscanf (file_r, "%s\n", timeNow);
		fclose (file_r);
	}
}

void Engine::saveResults()
{
	FILE *file = fopen(buffer, "w");
	if(!file)
		maPanic(1, "Nie moge zapisac pliku na karcie SD");
	fprintf(file, "%d\n", gamePoints);
	fprintf(file, "%d\n", game_level);
	getDate();
	fprintf(file, "%s\n", dateNow);
	getTime();
	fprintf(file, "%s\n", timeNow);
	fclose(file);
}

void Engine::getDate()
{
	time_t now = time(0);
	tm * localtm = localtime(&now);

	char buffor_int[5];
	itoa((int)localtm->tm_year+1900, buffor_int, 10);
	strcpy(dateNow,buffor_int);
	strcat(dateNow,"-");
	itoa((int)localtm->tm_mon+1, buffor_int, 10);
	strcat(dateNow,buffor_int);
	strcat(dateNow,"-");
	itoa((int)localtm->tm_mday, buffor_int, 10);
	strcat(dateNow,buffor_int);
}

void Engine::getTime()
{
	time_t now = time(0);
	tm * localtm = localtime(&now);

	char buffor_int[3];
	itoa((int)localtm->tm_hour, buffor_int, 10);
	strcpy(timeNow,buffor_int);
	strcat(timeNow,":");
	itoa((int)localtm->tm_min, buffor_int, 10);
	strcat(timeNow,buffor_int);
}

void Engine::doLogic()
{
	// zaktualizuj obiekty
	int count_sledge_dead = 0;
	int count_snowballs_dead = 0;
	for(int ii=0;ii<objects->size();ii++)
	{
		(*objects)[ii]->Update(maGetMilliSecondCount());
		if((*objects)[ii]->group==Object::SNOWBALL)
		{
			Object *col_obj = checkColision((*objects)[ii]);
			if(col_obj!=NULL)
			{
				col_obj->reset();
				(*objects)[ii]->reset();
				//col_obj->isColision = true;
				createSplash(col_obj->coords);

				// nalicz punkty
				if(col_obj->params.move_speed <= 50)
					gamePoints += POINT_PER_FAST_SLEDGE;
				else if(col_obj->params.move_speed <= 100)
					gamePoints += POINT_PER_MEDIUM_SLEDGE;
				else if(col_obj->params.move_speed <= 9999)
					gamePoints += POINT_PER_SLOW_SLEDGE;

				sledge_hit++;
				maVibrate(200);
				maSoundPlay(AUDIO_HIT, 0, maGetDataSize(AUDIO_HIT));
			}
		}
		// policz sanki ktore juz skonczyly jechac
		if(sledge_counter >= sledge_q && (*objects)[ii]->group == Object::SLEDGE && (*objects)[ii]->status == Object::DEAD)
			count_sledge_dead++;
		// policz zuzyte sniezki
		if((*objects)[ii]->group == Object::SNOWBALL && (*objects)[ii]->status == Object::DEAD)
			count_snowballs_dead++;
	}

	// wyrzuc sanki :)
	int pull = 2000 + (rand() % 2000);
	if(maGetMilliSecondCount() - xtime > pull)
	{
		// wypuszczaj sanki az skonczy sie limit
		if(sledge_counter < sledge_q)
		{
			createObject();		// wyrzuc sanki :)
			sledge_counter++;
			xtime = maGetMilliSecondCount();
		}
	}

	// zakoncz runde jak zuzyja sie zniezki albo jak saneczkaze sie rozjada :)
	if(count_sledge_dead >= sledge_q || count_snowballs_dead >= snowballs_q || sledge_hit>=round_treshold)
	{
		if(round_treshold==0)
		{
			maMessageBox("Super!!!","Gratuluję pomyślnego ukończenia gry! :)");
		}
		else if(sledge_hit>=round_treshold)
		{
			gamePoints += (snowballs_q - count_snowballs_dead);// dodaj punkty z ilosci sniezek jakie zostaly
			game_level++;

			if(game_level >= record_level && gamePoints > record_points)
			{
				if(!record_new)
				{
					record_new = true;
					maMessageBox("Brawo!","Został pobity nowy rekord gry!");
				}
				record_points = gamePoints;
				record_level = game_level;
				saveResults();
			}
			maMessageBox("Brawo!","Trafiono wymaganą ilość memów :)\n\nZapraszam do kolejnej rundy!");
		}
		else
		{
			if(life>1)
				maMessageBox("Ojej...","Nie trafiono wymaganej ilości memów albo zabrakło ci amunicji :(");
			life--;
			gamePoints = gamePointsTemp;

		}

		//////////////// KONIEC GRY !!!!
		if(life<=0)
		{
			game_level = FIRST_LEVEL;
			gamePoints = gamePointsTemp = sledge_counter = sledge_hit = 0;
			life = MAX_LIFE;
			maMessageBox("Przykro mi!","Niestety to już koniec gry :(");
		}

		stopGame();
	}
}

void Engine::doGraphics()
{
	// print bg
	graphics->printBackground(isMenuOn, gamePoints, game_level, sledge_counter, round_treshold, sledge_hit, life);
	// wyswielt obiekty
	for(int ii=0;ii<objects->size();ii++)
		graphics->printObject((*objects)[ii]);
	if(selectedHumanObject!=NULL)
		graphics->printObject(selectedHumanObject);

	///////////////////// wskaznic odswiezania strony
/*
	if((maGetMilliSecondCount() - fps_time) > 1000)
	{
		fps_print = fps;
		fps=0;
		fps_time = maGetMilliSecondCount();
	}
	char buffor[32];
	std::strcat(buffor,"FPS: ");
	itoa((int)fps_print, buffor, 10);
	maSetColor(0xFFFFFF);
	maDrawText(2, 2, buffor);
	fps++;
*/
	////////////////////////////
	if(isMenuOn)
		graphics->printLogo();

	// wyswietl wszsytko
	graphics->printOnScreen();
	//printf("%d, %d, %d\n",selectedHumanObject->destination.x,selectedHumanObject->destination.y,0);
}

void Engine::createSnowBalls()
{
	for(int ii=0;ii<snowballs_q;ii++)
	{
		Object *obj = new Object();
		obj->animation.isAnimation = false;
		obj->group = Object::SNOWBALL;
		obj->owner = DEFAULT_HUMAN_OWNER;
		if(graphics->top_y<0)
			graphics->setResource(obj, RES_SNOWBALL_4);
		else
			graphics->setResource(obj, RES_SNOWBALL_1);
		obj->coords.x = (rand() % (int)(Graphics::screenSize.x - (obj->size.x*2)))+(int)(obj->size.x/2);
		obj->coords.y = (Graphics::screenSize.y - (rand() % STANDING_AREA_HEIGHT) - (obj->size.y+1));
		obj->params.move_speed = 10;
		objects->push_back(obj);
	}
}

void Engine::createObject()
{
	Object *obj = new Object();
	short int direction = rand() % 2; 					// 0 - z lewej, 1 - z prawej
	short int move_y = rand() % 30;						// przesuniecia wzgledem Y
	short int move_dir_y = rand() % 2;					// przesuniecie Y: 0 w dol, 1 w gore
	int top_area_y = graphics->top_y + GAME_AREA_TOP;	// obszar roboczy wyskakujacych saneczek
	int bottom_area_y = top_area_y + GAME_AREA_HEIGHT;
	int which_sledge = (int)rand() % POSIBLE_SLEDGE;	// jedna z 6 saneczek

	graphics->setResource(obj, RES_SLEDGE_1+which_sledge);			// ustaw wylosowane saneczki
	obj->coords.x = (direction==0) ? -obj->size.x : Graphics::screenSize.x+1;	// ustaw wsp poczatkowe
	if(move_dir_y==0)															// przesuniecie gore czy dol
		move_y *= -1;
	obj->coords.y = top_area_y + (rand() % GAME_AREA_HEIGHT);		// wylosuj polozenie poczatkowe Y
	obj->group = Object::SLEDGE;									// ustaw grupe SANKI
	obj->owner = DEFAULT_WORLD_OWNER;								// ustaw przynaleznosc
	obj->direction = direction;										// zapamietaj wylosowany kierunek
	obj->params.move_speed = MAX_SPEED + (rand() % MIN_SPEED-((game_level-1)*10));	// ustaw szybkosc poruszania (co ile ms)
	if(obj->params.move_speed<=MAX_SPEED)									// jak juz szybkos bedzie na maksa nie pozwol zejsc ponizej 10 ms
		obj->params.move_speed = MAX_SPEED;
	if(which_sledge==5)												// ja bede najszybszy :>
		obj->params.move_speed = MAX_SPEED-10;

	// wystrzel sanki w odpowiednim kierunku
	Adds::vertex vert;
	vert.x = (direction==0) ? Graphics::screenSize.x+1 : -obj->size.x;	// ustaw wsp docelowa X
	vert.y = obj->coords.y + move_y;									// ustaw wsp docelowa Y
	obj->Go(vert, maGetMilliSecondCount());								// wpraw w ruch obiekt

	// zapamietaj aktualny czyli ostatnio wyrzucone sanki
	lastThrowed = obj;
	// wrzuc do wektowa z obiektami
	objects->push_back(obj);
}

void Engine::createSplash(Adds::vertex vert)
{
	Object *obj = new Object();
	obj->animation.isAnimation = false;
	obj->group = Object::SPLASH;
	obj->owner = DEFAULT_WORLD_OWNER;
	graphics->setResource(obj, RES_SPLASH);
	obj->coords.x = vert.x;
	obj->coords.y = vert.y;
	obj->live_time = maGetMilliSecondCount();
	obj->isWall = false;
	objects->push_back(obj);
}

Adds::vertex Engine::calcFarPoint(Adds::vertex start, Adds::vertex end)
{
	Adds::vertex vert;
	int line_m_length;

	line_m_length = (int)(Graphics::screenSize.y / THROW_AREA_HEIGHT);

	vert.x = ((std::max(start.x, end.x) - std::min(start.x, end.x))*line_m_length);
	vert.y = 0;
	if(end.x<start.x) vert.x = end.x - vert.x;
	else if(end.x>start.x) vert.x = end.x + vert.x;
	else vert.x = end.x;
	return vert;
}

void Engine::deleteAllObjects()
{
    for(size_t ii=0;ii<(size_t)objects->size();ii++)
    {
    	(*objects)[ii]->destination_coords->clear();
    	//delete (*objects)[ii]->destination_coords;
    	delete (*objects)[ii];
    }
    objects->clear();
}

bool Engine::checkObjectSelection(Adds::vertex coord)
{
	int treshold = 5;
	for(int ii=objects->size();ii>=0;ii--)
	{
		if((coord.x >= ((*objects)[ii]->coords.x + graphics->map_moving.x) - treshold && coord.x <= ((*objects)[ii]->coords.x + graphics->map_moving.x) + (*objects)[ii]->animation.frame_size.x + treshold) &&
			(coord.y >= ((*objects)[ii]->coords.y + graphics->map_moving.y) - treshold && coord.y <= ((*objects)[ii]->coords.y + graphics->map_moving.y) + (*objects)[ii]->animation.frame_size.y + treshold))// && (*objects)[ii]!=selectedHumanObject)
		{
			// sprawdz czy jestesmy na pewno wlascicielem danego obiektu
			if((*objects)[ii]->owner==DEFAULT_HUMAN_OWNER && (*objects)[ii]->status == Object::STOP)
			{
				// odznacz poprzednio zaznaczony obiekt
				if((*objects)[ii]!=selectedHumanObject)
				{
					if(selectedHumanObject!=NULL)
						selectedHumanObject->border = false;
					// zaznacz wskazany obiekt i zapamietaj jego wskaznik
					(*objects)[ii]->border = true;
					// zapisz zaznaczony obiekt (rowniez w pomocnicznej zmiennej)
					selectedHumanObject = (*objects)[ii];
					return true;
				}
				/*
				else
				{
					if(selectedHumanObject!=NULL)
						selectedHumanObject->border = false;
					// zaden obiekt nie jest zaznaczony
					//selectedHumanObject = NULL;
				}
				*/
			}
		}
	}
	return false;
}

Object* Engine::checkColision(Object *obj)
{
	// kolizja moze byc tylko z obiektem ktory jest nie do przejscia i ktory nie rusza sie
	if(obj->isWall && obj->status==Object::MOVING)
	{
		for(int ii=0;ii<objects->size();ii++)
		{
			Object *obj2 = (*objects)[ii];
			if(obj2->group==Object::SLEDGE)
			{
				Adds::vertex vert;
				for(int jj=0;jj<4;jj++)
				{
					getCornerCoord(obj->coords, obj->size, jj, vert);
					// sprawdz czy sniezka jest w obszarze gry
					if(vert.x>=0 && vert.x<=Graphics::screenSize.x)
					{
						if(checkOverlap(vert, obj2) && obj!=obj2 && obj->status!=Object::DEAD && obj2->status!=Object::DEAD)
							return obj2;
					}
				}
			}
		}
	}
	return NULL;
}

bool Engine::checkOverlap(Adds::vertex vert, Object *obj)
{
	if(vert.x >= obj->coords.x && vert.y >= obj->coords.y && vert.x <= ((int)obj->coords.x + (int)obj->size.x) && vert.y <= ((int)obj->coords.y + (int)obj->size.y))
		return true;
	return false;
}

void Engine::getCornerCoord(Adds::vertex coords, Adds::vertex size, unsigned int corner, Adds::vertex &vert)
{
	switch(corner)
	{
		case 0: vert.x = (int)coords.x; vert.y = (int)coords.y; break;
		case 1: vert.x = (int)(coords.x + size.x); vert.y = (int)coords.y; break;
		case 2: vert.x = (int)(coords.x + size.x); vert.y = (int)(coords.y + size.y); break;
		case 3: vert.x = (int)coords.x; vert.y = (int)(coords.y + size.y); break;
	}
}

void Engine::debugPrintInt(int var)
{
	char buffor[32];
	strcpy(buffor,"");
	itoa((int)var, buffor, 10);
	maMessageBox("Debug",buffor);
}
