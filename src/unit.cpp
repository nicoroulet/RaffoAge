#include "unit.h"
#include "fastsqrt.h"

#include <iostream>


#include <sys/stat.h>
#include <string>
#include <fstream>

inline bool exists (const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good();
}


// inline bool exists (const std::string& name) {
//   struct stat buffer;
//   return (stat (name.c_str(), &buffer) == 0);
// }

Unit::Unit(Sprite *sprite) :
	selected(false),
	sprite(sprite)
	{}

// Unit::Unit(Unit &u) {
// 	std::cerr << "Unit copy constructor called!\n";
// }

// Unit::Unit(Unit &&u) :
// 	sprite(std::move(u.sprite)),
// 	pos_x(u.pos_x),
// 	pos_y(u.pos_y),
// 	dst_x(u.dst_x),
// 	dst_y(u.dst_y),
// 	selected(u.selected)
// 	{}

// Unit& operator=(Unit u) {
// 	swap(*this, u);
// 	return *this;
// }

// void swap(Unit &u1, Unit &u2) {

// }

// void Unit::create_unit(UnitType * t, ALLEGRO_BITMAP * backbuffer) {
// 	std::cerr << "creating unit: " << t->name;
// 	sprite.create_sprite(t->bmp, backbuffer, t->framesize);
// 	type = t;
// 	selected = false;
// 	std::cerr << "... OK\n";
// }

// void Unit::delete_unit() {
// 	sprite.delete_sprite();
// }

void Unit::move(int x, int y) {
	// TODO: optimize this function
	dst_x = x - sprite->get_framesize()/2;
	dst_y = y - sprite->get_framesize()/2;
	float d_x = dst_x - pos_x;
	float d_y = dst_y - pos_y;
	float tang = d_y / (d_x + 0.0001f);
	if (tang < 0) tang = - tang;
	Direction direction;
	if (tang < 0.41421f) {
		if (d_x > 0) direction = LEFT;
		else direction = RIGHT;
	}
	else if (tang > 2.41421f) {
		if (d_y < 0) direction = UP;
		else direction = DOWN;
	}
	else if (d_x > 0 && d_y < 0) direction = UP_LEFT;
	else if (d_x > 0 && d_y > 0) direction = DOWN_LEFT;
	else if (d_x < 0 && d_y < 0) direction = UP_RIGHT;
	else direction = DOWN_RIGHT;
	sprite->change_direction(direction);
	sprite->change_status(WALK);
}

void Unit::clear() {
	// if (dst_x - pos_x || dst_y - pos_y)
		sprite->clear();
}

void Unit::draw() {
	float d_x = dst_x - pos_x;
	float d_y = dst_y - pos_y;

	float dist = fastsqrt(d_x * d_x + d_y * d_y);
	pos_x += d_x * ((/*type->*/speed < dist)? /*type->*/speed : dist) / dist;
	pos_y += d_y * ((/*type->*/speed < dist)? /*type->*/speed : dist) / dist;
	if (speed >= dist) {
		sprite->change_status(IDLE);
	}
	sprite->draw(pos_x, pos_y, selected);

}

void Unit::select() {
	selected = true;
}

void Unit::unselect() {
	selected = false;
}

void Unit::set_position(int x, int y) {
	pos_x = x-1;
	pos_y = y-1;
	dst_x = x;
	dst_y = y;
	draw();
	// sprite->draw(pos_x, pos_y);
}

int Unit::y() {
	return pos_y;
}

int Unit::x() {
	return pos_x;
}

bool Unit::operator<(Unit & other) {
	return pos_y + sprite->get_framesize() < other.pos_y + other.sprite->get_framesize();
}

bool Unit::is_clicked(int x, int y) {
	return sprite->is_clicked(x, y);
}

bool Unit::is_broadly_clicked(int x, int y) {
	return sprite->is_broadly_clicked(x, y);
}

bool Unit::intersects(rectangle & rect) {
	rectangle rect2 = sprite->get_rectangle();
	return rect.intersect(rect2).is_valid();
}

float Unit::speed = 30;
// TODO delete in destructor
Pirate::Pirate() :
	Unit(new PirateSprite())
	{}

ALLEGRO_BITMAP *Pirate::unit_type_bitmap_ptr = NULL;

void Pirate::initialize() {
	if (!unit_type_bitmap_ptr) {
		if (exists("/home/nico/Documents/programacion/RaffoAge/res/units/Pirate2.png")) {
			std::cout << "\n exists \n";
		} else {
			std::cout << "\n not exists \n";
		}
		al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
		ALLEGRO_BITMAP *bmp = al_load_bitmap("res/units/Pirate2.png");
		unit_type_bitmap_ptr = bmp;
		std::cerr << "initialized pirate bitmap to " << bmp;
	}
	PirateSprite::initialize();
}

ALLEGRO_BITMAP *Pirate::unit_type_bitmap() {
	return unit_type_bitmap_ptr;
}