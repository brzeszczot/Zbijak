/*
 * Object.cpp
 *
 *  Created on: Nov 18, 2012
 *      Author: brzeszczot
 */

#include "Object.hpp"

Object::Object()
{
	params.life = 100;
	params.gold = 0;
	params.skills = 0;
	params.visible_dist = 20;
	params.move_speed = 10; // co ile ms ruch
	params.range = 30;

	group = DEFAULT;
	status = STOP;
	owner = 0;

	isWall = true;
	isColision = false;
	visible = true;
	border = false;
	destination.x = destination.y = destination_last.x = destination_last.y = 0;
	destination_coords = new std::vector<Adds::vertex>;
	destination_current = 0;
	direction = 0;
	last_direction = start_y = -1;
	move_timer = 0;

	animation.isAnimation = false;
	animation.animate = false;
	animation.speed = 200;
	animation.timer = 0;
	animation.frame.x = 2;
	animation.frame.y = 2;
}

void Object::Update(int current_time)
{
	// usun plame po 3 sekundach
	if(group == SPLASH && status != DEAD && (current_time - live_time)>2000)
		reset();

	time = current_time;
	//if(owner != 0)
	//	setSpeed();
	changeMovement();
	changeAnim();
}

void Object::Stop()
{
	status = STOP;
	animation.animate = false;
	animation.frame.x = animation.frame.y = 2;// klatka neutralna - przodem
}

void Object::Go()
{
	status = MOVING;
	animation.animate = true;
	animation.frame.y = direction;
	//animation.frame.y = getDirection(coords, destination);
}

void Object::Go(Adds::vertex dest_coord, int current_time)
{
	start_y = (int)coords.y;

	destination = dest_coord;
	status = MOVING;
	//move_timer = current_time;
	animation.animate = true;
	//direction = getDirection(coords, destination);
	animation.frame.y = direction;

	destination_coords->clear();
	destination_coords->push_back(coords);
	Algorithms::SectionCalc(coords, destination, 1, destination_coords);
	destination_current = 0;
	//float ret = alg.SectionLength(selectedHumanObject->coords, selectedHumanObject->destination);
}

void Object::reset()
{
	destination_coords->clear();
	destination_current = 0;
	Stop();
	visible = false;
	status = Object::DEAD;
}

void Object::setSpeed()
{
	//if(coords.y <= start_y/8)	params.move_speed = 100;
	if(coords.y <= start_y/6)	params.move_speed = 50;
	else if(coords.y <= start_y/4)	params.move_speed = 30;
	else if(coords.y <= start_y/3)	params.move_speed = 20;
	else if(coords.y <= start_y/2)	params.move_speed = 10;
}

void Object::changeMovement()
{
	if(time - move_timer > params.move_speed && status==MOVING)
	{
		move_timer = time;
		// jesli to nie koniec drogi - ustaw kolejna wspolrzedna drogi
		if(destination_current < destination_coords->size())
		{
			coords = (*destination_coords)[destination_current];
			destination_current+=10;
		}
		else
		{
			destination_coords->clear();
			destination_current = 0;
			Stop();
			visible = false;
			status = DEAD;
		}
	}
}

void Object::changeAnim()
{
	if(time - animation.timer > animation.speed && animation.animate)
	{
		animation.frame.x++;
		if(animation.frame.x>=ANIM_FRAMES)
			animation.frame.x = 0;

		animation.timer = time;
	}
}

unsigned int Object::getDirection(Adds::vertex p1, Adds::vertex p2)
{
	int dif_x = (int)std::max(p2.x, p1.x) - (int)std::min(p2.x, p1.x);
	int dif_y = (int)std::max(p2.y, p1.y) - (int)std::min(p2.y, p1.y);

	if(dif_x <= dif_y && ((p2.x <= p1.x && p2.y <= p1.y) || (p2.x >= p1.x && p2.y <= p1.y)))
		return DIRECTION_UP;
	else if(dif_x <= dif_y && ((p2.x <= p1.x && p2.y >= p1.y) || (p2.x >= p1.x && p2.y >= p1.y)))
		return DIRECTION_DOWN;
	else if(dif_x >= dif_y && ((p2.x <= p1.x && p2.y <= p1.y) || (p2.x <= p1.x && p2.y >= p1.y)))
		return DIRECTION_LEFT;
	else if(dif_x >= dif_y && ((p2.x >= p1.x && p2.y <= p1.y) || (p2.x >= p1.x && p2.y >= p1.y)))
		return DIRECTION_RIGHT;
}

Object::~Object()
{
    destination_coords->clear();
    delete destination_coords;
}
